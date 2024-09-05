package com.example.lab2pratt

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.lifecycle.ViewModelProvider

class Fragment_Two : Fragment() {

    private lateinit var customView: customViewPratt
    private lateinit var drawingViewModel: DrawingViewModel

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        return inflater.inflate(R.layout.fragment_two, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        drawingViewModel = ViewModelProvider(requireActivity()).get(DrawingViewModel::class.java)
        customView = view.findViewById(R.id.customViewPratt)

        // custom view update
        customView.setStrokes(drawingViewModel.strokes)

        // view model updating with new Strokes
        customView.onStrokeDrawn = { stroke ->
            drawingViewModel.strokes.add(stroke)
        }
    }
}
