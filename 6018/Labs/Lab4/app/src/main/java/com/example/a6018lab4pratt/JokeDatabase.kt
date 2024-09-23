package com.example.a6018lab4pratt

import android.content.Context
import androidx.room.Dao
import androidx.room.Database
import androidx.room.Insert
import androidx.room.Query
import androidx.room.Room
import kotlinx.coroutines.flow.Flow
import androidx.room.RoomDatabase

@Database(entities = [JokeEntity::class], version = 1)
abstract class JokeDatabase : RoomDatabase() {
    abstract fun jokeDao(): JokeDAO

    companion object {
        @Volatile
        private var INSTANCE: JokeDatabase? = null

        fun getDatabase(context: Context): JokeDatabase {
            return INSTANCE ?: synchronized(this) {
                val instance = Room.databaseBuilder(
                    context.applicationContext,
                    JokeDatabase::class.java,
                    "joke_database"
                ).build()
                INSTANCE = instance
                instance
            }
        }
    }
}

@Dao
interface JokeDAO {
    @Insert
    suspend fun addJoke(joke: JokeEntity)

    @Query("SELECT * FROM jokes ORDER BY id DESC LIMIT 1")
    fun latestJoke(): Flow<JokeEntity>

    @Query("SELECT * FROM jokes ORDER BY id DESC")
    fun getAllJokes(): Flow<List<JokeEntity>>

    @Query("DELETE FROM jokes")
    suspend fun deleteAllJokes()
}
