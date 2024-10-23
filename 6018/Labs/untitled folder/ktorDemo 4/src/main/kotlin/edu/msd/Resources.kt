package edu.msd


import io.ktor.http.*
import io.ktor.resources.*
import io.ktor.server.application.*
import io.ktor.server.request.*
import io.ktor.server.resources.*
import io.ktor.server.resources.Resources
import io.ktor.server.resources.delete
import io.ktor.server.resources.post
import io.ktor.server.response.*
import io.ktor.server.routing.*

import kotlinx.coroutines.Dispatchers
import kotlinx.serialization.Serializable
import org.jetbrains.exposed.sql.*
import org.jetbrains.exposed.sql.SqlExpressionBuilder.eq
import org.jetbrains.exposed.sql.transactions.experimental.newSuspendedTransaction

fun Application.configureResources() {
    install(Resources)
    routing{
        //get all the posts
        get<Posts> {
            //handler for /books
            call.respond(
                newSuspendedTransaction(Dispatchers.IO) {
                    Post.selectAll().map{
                        PostData(it[Post.id].value, it[Post.content], it[Post.timestamp])
                    }

                }
            )
        }

        //create a new POST post
        post<Posts> {
            val postData = call.receive<NewPostData>()
            val postId = newSuspendedTransaction(Dispatchers.IO) {
                Post.insertAndGetId {
                    it[content] = postData.content
                    it[timestamp] = System.currentTimeMillis() // or use a provided timestamp
                }
            }
            val createdTimestamp = newSuspendedTransaction(Dispatchers.IO) {
                Post.select { Post.id eq postId }
                    .map { it[Post.timestamp] }
                    .singleOrNull()
            }

            // Respond with both the post ID and timestamp
            call.respondText("Post created with ID: $postId and timestamp: $createdTimestamp ", status = HttpStatusCode.Created)

        }

        // GET a post by ID
        get<Posts.PostId> { postId ->
            val id = postId.id
            val post = newSuspendedTransaction(Dispatchers.IO) {
                Post.select { Post.id eq id }
                    .map { PostData(it[Post.id].value, it[Post.content], it[Post.timestamp]) }
                    .singleOrNull()
            }

            if (post != null) {
                call.respond(post)
            } else {
                call.respondText("Post not found", status = HttpStatusCode.NotFound)
            }
        }


        get<Posts.Since>{since ->
            val timestamp = since.timestamp
            call.respond(
                newSuspendedTransaction(Dispatchers.IO) {
                    Post.select { Post.timestamp greaterEq timestamp }
                        .map{PostData(it[Post.id].value, it[Post.content], it[Post.timestamp])}
                }
            )
        }

        delete<Posts.PostId> { postId ->
            val id = postId.id
            val delete = newSuspendedTransaction(Dispatchers.IO) {
                Post.deleteWhere { Post.id eq id }
            }
            if(delete > 0){
                call.respond("Post with ID $id deleted")
            }else{
                call.respond("Post with ID $id not found")
            }
        }



    }
}


@Serializable
data class PostData(val id: Int, val content: String, val timestamp: Long)

@Serializable
data class NewPostData(val content: String)

@Resource("/posts")
class Posts {
    @Resource("{id}") // corresponds to /posts/{id}
    class PostId(val parent: Posts = Posts(), val id: Int)

    @Resource("since/{timestamp}") // posts/since/{timestamp}
    class Since (val parent: Posts = Posts(), val timestamp: Long)
}

