package com.example.a6018lab4pratt

import androidx.room.Entity
import androidx.room.PrimaryKey

@Entity(tableName = "jokes")
data class JokeEntity(
    @PrimaryKey(autoGenerate = true) val id: Int = 0,
    val joke: String
)
