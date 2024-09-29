package com.example.lab5

import GravityViewModel
import android.annotation.SuppressLint
import android.graphics.Color
import android.hardware.Sensor
import android.hardware.SensorManager
import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.BoxWithConstraints
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.offset
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.runtime.livedata.observeAsState
import androidx.lifecycle.LiveData
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.lifecycle.ViewModel
import com.example.lab5.ui.theme.Lab5Theme
import com.example.lab5.ui.theme.SensorUtils


class MainActivity : ComponentActivity() {
    private lateinit var sensorManager: SensorManager
    private lateinit var gravitySensor: Sensor
    private lateinit var sensorUtils: SensorUtils
    private lateinit var viewModel : GravityViewModel

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        sensorManager = getSystemService(SENSOR_SERVICE) as SensorManager
        gravitySensor = sensorManager.getDefaultSensor(Sensor.TYPE_GRAVITY)!!
        sensorUtils = SensorUtils()
        viewModel = GravityViewModel()

        setContent {
            GravitySensorScreen(sensorUtils, gravitySensor, sensorManager,viewModel)
        }

    }

    @SuppressLint("UnusedBoxWithConstraintsScope")
    @Composable
    fun Marble(viewModel: GravityViewModel){
        val xOffset by viewModel.xOffset.observeAsState(0f)
        val yOffset by viewModel.yOffset.observeAsState(0f)

        BoxWithConstraints(modifier = Modifier.fillMaxSize()) {
            Box(
                modifier = Modifier
                    .offset(xOffset.dp, yOffset.dp)
                    .size(40.dp)
                    .background(color = androidx.compose.ui.graphics.Color.Green, shape = CircleShape)
            )
        }
    }


    @Composable
    fun GravitySensorScreen(
        sensorUtils: SensorUtils,
        gravitySensor: Sensor,
        sensorManager: SensorManager,
        viewModel: GravityViewModel
    ) {
        val gravityData by sensorUtils.getGravityData(gravitySensor, sensorManager)
            .collectAsState(initial = floatArrayOf(0f, 0f, 0f))

        val orientation = when {
            gravityData[2] > 9 -> "Face Up"
            gravityData[2] < -9 -> "Face Down"
            gravityData[1] > 7 -> "Portrait"
            gravityData[0] > 7 -> "Landscape (Left)"
            gravityData[0] < -7 -> "Landscape (Right)"
            else -> "Unknown Orientation"
        }

        viewModel.updateOffsets(gravityData[0], gravityData[1])

        Surface(
            modifier = Modifier.fillMaxWidth(),
            color = MaterialTheme.colorScheme.background
        ) {
            Column {
                Text(
                    text = "X: ${gravityData[0]}",
                    style = MaterialTheme.typography.bodyLarge
                )
                Text(
                    text = "Y: ${gravityData[1]}",
                    style = MaterialTheme.typography.bodyLarge
                )
                Text(
                    text = "Z: ${gravityData[2]}",
                    style = MaterialTheme.typography.bodyLarge
                )
//                Text(
//                    text = "Orientation: $orientation",
//                    style = MaterialTheme.typography.headlineLarge
//                )

                Marble(viewModel)
            }
        }
    }

}