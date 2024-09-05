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

data class Stroke(val path: Path, val paint: Paint)

class customViewPratt(context: Context, attrs: AttributeSet) : View(context, attrs) {

    private var path = Path()
    private val myPaint = Paint().apply {
        color = Color.BLACK
        strokeWidth = 5f
        style = Paint.Style.STROKE
        isAntiAlias = true
    }

    private val strokes = mutableListOf<Stroke>()


    var onStrokeDrawn: ((Stroke) -> Unit)? = null

    init {
        setWillNotDraw(false)
    }

    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)
        for (stroke in strokes) {
            canvas.drawPath(stroke.path, stroke.paint)
        }
        canvas.drawPath(path, myPaint)
    }

    @SuppressLint("ClickableViewAccessibility")
    override fun onTouchEvent(event: MotionEvent): Boolean {
        when (event.action) {
            MotionEvent.ACTION_DOWN -> {
                path = Path()
                path.moveTo(event.x, event.y)

                val newStroke = Stroke(path, Paint(myPaint))
                onStrokeDrawn?.invoke(newStroke)

                invalidate()
            }
            MotionEvent.ACTION_MOVE -> {
                path.lineTo(event.x, event.y)
                invalidate()
            }
            MotionEvent.ACTION_UP -> {
                path.lineTo(event.x, event.y)
                val stroke = Stroke(path, Paint(myPaint))
                strokes.add(stroke)
                onStrokeDrawn?.invoke(stroke)
                invalidate()
            }
        }
        return true
    }

    // View Model set strokes
    fun setStrokes(strokes: List<Stroke>) {
        this.strokes.clear()
        this.strokes.addAll(strokes)
        invalidate()
    }
}
