package com.example.lab1_helloworld

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Button

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val OrangeButton : Button = findViewById(R.id.button)
        val BlueButton : Button = findViewById(R.id.button2)
        val RedButton : Button = findViewById(R.id.button3)
        val GreenButton : Button = findViewById(R.id.button4)
        val PurpleButton : Button = findViewById(R.id.button5)

        OrangeButton.setOnClickListener{
            val intent = Intent(this, ActivityTwo::class.java)
            intent.putExtra("button_text", "Orange")
            intent.putExtra("background_color", "#FFA500")
            startActivity(intent)
        }

        BlueButton.setOnClickListener {
            val intent = Intent(this, ActivityTwo::class.java).apply {
                putExtra("button_text", "Blue")
                putExtra("background_color", "#0000FF") // Blue color
            }
            startActivity(intent)
        }

        RedButton.setOnClickListener {
            val intent = Intent(this, ActivityTwo::class.java).apply {
                putExtra("button_text", "Red")
                putExtra("background_color", "#FF0000") // Red color
            }
            startActivity(intent)
        }

        GreenButton.setOnClickListener {
            val intent = Intent(this, ActivityTwo::class.java).apply {
                putExtra("button_text", "Green")
                putExtra("background_color", "#008000") // Green color
            }
            startActivity(intent)
        }

        PurpleButton.setOnClickListener {
            val intent = Intent(this, ActivityTwo::class.java).apply {
                putExtra("button_text", "Purple")
                putExtra("background_color", "#800080") // Purple color
            }
            startActivity(intent)
        }
    }
}