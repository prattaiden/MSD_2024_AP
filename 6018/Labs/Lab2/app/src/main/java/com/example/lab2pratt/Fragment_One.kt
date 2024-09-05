package com.example.lab2pratt

import android.annotation.SuppressLint
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button

class Fragment_One : Fragment() {

    @SuppressLint("MissingInflatedId")
    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_one, container, false)
        val draw_button: Button = view.findViewById(R.id.drawButton)

        //setting button in the first segment click method
        draw_button.setOnClickListener{
            parentFragmentManager.beginTransaction()
                .replace(R.id.frameLayout, Fragment_Two())
                .addToBackStack(null)
                .commit()
        }

        return view
    }
}