package com.example.lab2pratt

import androidx.lifecycle.ViewModel

class DrawingViewModel : ViewModel() {
    val strokes = mutableListOf<Stroke>()
}