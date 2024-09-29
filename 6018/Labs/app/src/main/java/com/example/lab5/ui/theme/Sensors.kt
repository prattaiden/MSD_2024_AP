package com.example.lab5.ui.theme

import android.hardware.Sensor
import android.hardware.SensorEvent
import android.hardware.SensorEventListener
import android.hardware.SensorManager
import android.util.Log
import kotlinx.coroutines.channels.awaitClose
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.channelFlow

class SensorUtils {

    //getting the gyroscope data
    fun getGyroData(gyroscope: Sensor, sensorManager: SensorManager): Flow<FloatArray> {
        return getSensorData(gyroscope, sensorManager)
    }

    //getting the gravity data
    fun getGravityData(gravitySensor: Sensor, sensorManager: SensorManager): Flow<FloatArray> {
        return getSensorData(gravitySensor, sensorManager)
    }

    //get any sensor data function
    private fun getSensorData(sensor: Sensor, sensorManager: SensorManager): Flow<FloatArray> {
        return channelFlow {
            val listener = object : SensorEventListener {
                override fun onSensorChanged(event: SensorEvent?) {
                    if (event !== null) {
                        Log.e("Sensor event!", event.values.toString())
                        val success = channel.trySend(event.values).isSuccess
                        Log.e("Success?", success.toString())
                    }
                }

                override fun onAccuracyChanged(sensor: Sensor?, accuracy: Int) {

                }
            }

            sensorManager.registerListener(listener, sensor, SensorManager.SENSOR_DELAY_GAME)

            awaitClose {
                sensorManager.unregisterListener(listener)
            }
        }
    }
}
