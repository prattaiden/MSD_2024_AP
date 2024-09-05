package com.example.lab2pratt

import android.annotation.SuppressLint
import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.graphics.Path
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.View
import androidx.lifecycle.ViewModelProvider

data class Stroke(val path: Path, val paint: Paint)
class customViewPratt(context: Context, attrs: AttributeSet) : View(context, attrs) {

    private var path = Path()
    // view model in a custom view model to store the strokes for when going back and forth
    private val viewModel: DrawingViewModel = ViewModelProvider(context as MainActivity).get(DrawingViewModel::class.java)

    private val myPaint = Paint().apply {
        color = Color.BLACK
        strokeWidth = 5f
        style = Paint.Style.STROKE
        isAntiAlias = true
    }

    init {
        setWillNotDraw(false)
    }

    override fun onDraw(canvas: Canvas){
        super.onDraw(canvas)
        for(stroke in viewModel.strokes){
            canvas.drawPath(stroke.path, stroke.paint)
        }
        canvas.drawPath(path, myPaint)
    }

    @SuppressLint("ClickableViewAccessibility")
    override fun onTouchEvent(event: MotionEvent): Boolean{
        when (event.action) {
            MotionEvent.ACTION_DOWN -> {
                path = Path()
                path.moveTo(event.x, event.y)

                viewModel.strokes.add(Stroke(path, myPaint))
                invalidate()
            }
            MotionEvent.ACTION_MOVE -> {
                path.lineTo(event.x, event.y)
                invalidate()
            }
            MotionEvent.ACTION_UP -> {
                path.lineTo(event.x, event.y)
                invalidate()
            }
        }
        return true
    }



}
