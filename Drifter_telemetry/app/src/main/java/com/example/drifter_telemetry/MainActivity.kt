package com.example.drifter_telemetry

import android.Manifest
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothSocket
import android.content.pm.PackageManager
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.view.View
import android.widget.ProgressBar
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import org.json.JSONObject
import java.io.IOException
import java.io.InputStream
import java.util.*

// Import the generated R class
import com.example.drifter_telemetry.R

class MainActivity : AppCompatActivity() {

    private val bluetoothAdapter: BluetoothAdapter? = BluetoothAdapter.getDefaultAdapter()
    private lateinit var bluetoothSocket: BluetoothSocket
    private lateinit var inputStream: InputStream
    private val handler = Handler(Looper.getMainLooper())
    private val fetchInterval: Long = 50 // Fetch data every 50 milliseconds (20Hz)
    private val deviceAddress = "A0:DD:6C:03:9A:EE" // Replace with your ESP32 Bluetooth MAC address
    private val uuid: UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB") // Standard SerialPortService ID

    // Declare views
    private lateinit var gForceView: GForceView
    private lateinit var leftWheelLine: View
    private lateinit var rightWheelLine: View
    private lateinit var rotationalRateGauge: RotationalRateGaugeView
    private lateinit var armedTextView: TextView
    private lateinit var steeringModeTextView: TextView
    private lateinit var driveModeTextView: TextView
    private lateinit var motor1RpmBar: ProgressBar
    private lateinit var motor2RpmBar: ProgressBar
    private lateinit var motor3RpmBar: ProgressBar
    private lateinit var motor4RpmBar: ProgressBar

    private val REQUEST_ENABLE_BT = 1
    private val buffer = StringBuilder()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Request Bluetooth permissions
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED ||
            ActivityCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_SCAN) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, arrayOf(Manifest.permission.BLUETOOTH_CONNECT, Manifest.permission.BLUETOOTH_SCAN), REQUEST_ENABLE_BT)
        } else {
            initializeBluetooth()
        }

        // Initialize views
        gForceView = findViewById(R.id.gForceView)
        leftWheelLine = findViewById(R.id.leftWheelLine)
        rightWheelLine = findViewById(R.id.rightWheelLine)
        rotationalRateGauge = findViewById(R.id.rotationalRateGauge)
        armedTextView = findViewById(R.id.armedTextView)
        steeringModeTextView = findViewById(R.id.steeringModeTextView)
        driveModeTextView = findViewById(R.id.driveModeTextView)
        motor1RpmBar = findViewById(R.id.motor1RpmBar)
        motor2RpmBar = findViewById(R.id.motor2RpmBar)
        motor3RpmBar = findViewById(R.id.motor3RpmBar)
        motor4RpmBar = findViewById(R.id.motor4RpmBar)

        // Start fetching data continuously
        startFetchingData()
    }

    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<String>, grantResults: IntArray) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        if (requestCode == REQUEST_ENABLE_BT) {
            if (grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                initializeBluetooth()
            } else {
                // Permission denied, show a message to the user
                Log.e("MainActivity", "Bluetooth permissions denied")
            }
        }
    }

    private fun initializeBluetooth() {
        val device: BluetoothDevice = bluetoothAdapter!!.getRemoteDevice(deviceAddress)
        bluetoothSocket = device.createRfcommSocketToServiceRecord(uuid)
        bluetoothAdapter.cancelDiscovery()
        try {
            bluetoothSocket.connect()
            inputStream = bluetoothSocket.inputStream
            Log.d("MainActivity", "Bluetooth connected")
        } catch (e: IOException) {
            e.printStackTrace()
            Log.e("MainActivity", "Failed to connect Bluetooth: ${e.message}")
            try {
                bluetoothSocket.close()
            } catch (closeException: IOException) {
                closeException.printStackTrace()
            }
        }
    }

    private fun startFetchingData() {
        handler.post(fetchDataRunnable)
    }

    private val fetchDataRunnable = object : Runnable {
        override fun run() {
            Log.d("MainActivity", "Running fetchDataRunnable")
            fetchBluetoothData()
            handler.postDelayed(this, fetchInterval)
        }
    }

    private fun fetchBluetoothData() {
        try {
            val buffer = ByteArray(1024)
            val bytes = inputStream.read(buffer)
            val jsonData = String(buffer, 0, bytes)
            Log.d("MainActivity", "Fetched raw data: $jsonData")
            processReceivedData(jsonData)
        } catch (e: IOException) {
            e.printStackTrace()
            Log.e("MainActivity", "Failed to fetch data: ${e.message}")
        }
    }

    private fun processReceivedData(data: String) {
        buffer.append(data)

        var startIndex = 0
        var endIndex: Int

        while (true) {
            endIndex = buffer.indexOf("\n", startIndex)
            if (endIndex == -1) break

            val jsonObject = buffer.substring(startIndex, endIndex).trim()
            if (jsonObject.isNotEmpty()) {
                try {
                    runOnUiThread {
                        updateUI(jsonObject)
                    }
                } catch (e: Exception) {
                    e.printStackTrace()
                    Log.e("MainActivity", "Failed to parse JSON data: ${e.message}")
                }
            }
            startIndex = endIndex + 1
        }

        // Remove processed data from buffer
        buffer.delete(0, startIndex)
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
            val armed = jsonObject.getString("arm_enabled")
            val steeringMode = jsonObject.getString("steering_mode")
            val driveMode = jsonObject.getString("drive_mode")
            val motor1Rpm = jsonObject.getInt("motor1_rpm")
            val motor2Rpm = jsonObject.getInt("motor2_rpm")
            val motor3Rpm = jsonObject.getInt("motor3_rpm")
            val motor4Rpm = jsonObject.getInt("motor4_rpm")

            gForceView.updateGForce(gForceX, gForceY)
            leftWheelLine.rotation = leftSteeringAngle
            rightWheelLine.rotation = rightSteeringAngle
            rotationalRateGauge.updateRotationalRate(rotationalRate)
            armedTextView.text = armed
            steeringModeTextView.text = steeringMode
            driveModeTextView.text = driveMode
            motor1RpmBar.progress = motor1Rpm
            motor2RpmBar.progress = motor2Rpm
            motor3RpmBar.progress = motor3Rpm
            motor4RpmBar.progress = motor4Rpm
        } catch (e: Exception) {
            e.printStackTrace()
            Log.e("MainActivity", "Failed to parse JSON data: ${e.message}")
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        // Stop fetching data when the activity is destroyed
        handler.removeCallbacks(fetchDataRunnable)
        try {
            bluetoothSocket.close()
        } catch (e: IOException) {
            e.printStackTrace()
        }
    }
}