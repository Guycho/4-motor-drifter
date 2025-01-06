package com.example.drifter_telemetry

import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import okhttp3.*
import org.json.JSONObject
import java.io.IOException

class MainActivity : AppCompatActivity() {

    private lateinit var gForceView: GForceView
    private lateinit var leftWheelLine: View
    private lateinit var rightWheelLine: View
    private lateinit var rotationalRateGauge: RotationalRateGaugeView
    private val client = OkHttpClient()
    private val handler = Handler(Looper.getMainLooper())
    private val fetchInterval: Long = 100 // Fetch data every 100 milliseconds

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        gForceView = findViewById(R.id.gForceView)
        leftWheelLine = findViewById(R.id.leftWheelLine)
        rightWheelLine = findViewById(R.id.rightWheelLine)
        rotationalRateGauge = findViewById(R.id.rotationalRateGauge)

        // Start fetching data continuously
        startFetchingData()
    }

    private fun startFetchingData() {
        handler.post(fetchDataRunnable)
    }

    private val fetchDataRunnable = object : Runnable {
        override fun run() {
            fetchJsonData()
            handler.postDelayed(this, fetchInterval)
        }
    }

    private fun fetchJsonData() {
        val url = "http://192.168.4.1/data" // Replace with your ESP32 IP address
        Log.d("MainActivity", "Fetching data from $url")
        val request = Request.Builder().url(url).build()

        client.newCall(request).enqueue(object : Callback {
            override fun onFailure(call: Call, e: IOException) {
                e.printStackTrace()
                Log.e("MainActivity", "Failed to fetch data: ${e.message}")
            }

            override fun onResponse(call: Call, response: Response) {
                if (response.isSuccessful) {
                    val jsonData = response.body?.string()
                    Log.d("MainActivity", "Fetched data: $jsonData")
                    runOnUiThread {
                        updateUI(jsonData)
                    }
                } else {
                    Log.e("MainActivity", "Failed to fetch data: ${response.message}")
                }
            }
        })
    }

    private fun updateUI(jsonData: String?) {
        if (jsonData == null) return

        try {
            val jsonObject = JSONObject(jsonData)

            val gForceX = jsonObject.getDouble("g_force_x").toFloat()
            val gForceY = jsonObject.getDouble("g_force_y").toFloat()
            val leftSteeringAngle = jsonObject.getDouble("left_steering").toFloat()
            val rightSteeringAngle = jsonObject.getDouble("right_steering").toFloat()
            val rotationalRate = jsonObject.getDouble("rotational_rate").toFloat()

            gForceView.updateGForce(gForceX, gForceY)
            leftWheelLine.rotation = leftSteeringAngle
            rightWheelLine.rotation = rightSteeringAngle
            rotationalRateGauge.updateRotationalRate(rotationalRate)
        } catch (e: Exception) {
            e.printStackTrace()
            Log.e("MainActivity", "Failed to parse JSON data: ${e.message}")
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        // Stop fetching data when the activity is destroyed
        handler.removeCallbacks(fetchDataRunnable)
    }
}