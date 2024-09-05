package com.example.lab2pratt

import androidx.lifecycle.ViewModel

class DrawingViewModel : ViewModel() {
    //list of strokes in the view model
    val strokes = mutableListOf<Stroke>()
}