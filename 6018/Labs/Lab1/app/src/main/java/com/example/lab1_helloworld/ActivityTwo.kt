package com.example.lab1_helloworld

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity

class ActivityTwo : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_two)

        val displayTextView: TextView = findViewById(R.id.display_text)

        val buttonText = intent.getStringExtra("button_text")
        val backgroundColor = intent.getStringExtra("background_color")


        displayTextView.text = buttonText
        backgroundColor?.let {
            window.decorView.setBackgroundColor(android.graphics.Color.parseColor(it))
        }

        val backButton: Button = findViewById(R.id.button6)
        backButton.setOnClickListener {
            val intent = Intent(this, MainActivity::class.java)
            startActivity(intent)
        }
    }
}