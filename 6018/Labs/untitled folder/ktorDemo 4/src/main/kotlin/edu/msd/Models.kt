package edu.msd

import org.jetbrains.exposed.dao.id.IntIdTable
import org.jetbrains.exposed.sql.Table


object User : IntIdTable() {} //we have just one field, ID, primary, int, auto increment

object Post: IntIdTable(){
//    val title = varchar("title", 100).uniqueIndex()
    val content = varchar("content", 255)
    val timestamp = long("created_at")
}

//object Likes: Table(){
//    val user = reference("user_id", User.id)
//    val book = reference("book_id", Post.id)
//    override val primaryKey = PrimaryKey(arrayOf(user, book))
//
//}

