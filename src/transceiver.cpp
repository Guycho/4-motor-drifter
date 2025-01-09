#include "transceiver.h"

Transceiver::Transceiver() {}

Transceiver::~Transceiver() {}

void Transceiver::init(const TransceiverConfig& config) {
    m_esp_now_handler = config.esp_now_handler;
    m_update_delay_ms = config.update_delay_ms;
    m_data_timer.start();
}

void Transceiver::update_data() {
    if (!m_data_timer.hasPassed(m_update_delay_ms, true)) {
        return;
    }
    send_data();
    m_remote_data = m_esp_now_handler->get_data();
    // Serial.println(m_remote_data);
}

void Transceiver::send_data() {
    JsonDocument m_json_data;
    m_json_data["t"] = two_decimals(m_telemetry_data.throttle);
    m_json_data["s"] = two_decimals(m_telemetry_data.steering);
    m_json_data["a"] = m_telemetry_data.arm_enabled;
    m_json_data["sm"] = m_telemetry_data.steering_mode;
    m_json_data["dm"] = m_telemetry_data.drive_mode;
    m_json_data["1r"] = m_telemetry_data.motors_rpm[0];
    m_json_data["2r"] = m_telemetry_data.motors_rpm[1];
    m_json_data["3r"] = m_telemetry_data.motors_rpm[2];
    m_json_data["4r"] = m_telemetry_data.motors_rpm[3];
    m_json_data["1t"] = two_decimals(abs(m_telemetry_data.motors_throttle[0]));
    m_json_data["2t"] = two_decimals(abs(m_telemetry_data.motors_throttle[1]));
    m_json_data["3t"] = two_decimals(abs(m_telemetry_data.motors_throttle[2]));
    m_json_data["4t"] = two_decimals(abs(m_telemetry_data.motors_throttle[3]));
    m_json_data["b"] = two_decimals(m_telemetry_data.battery_voltage);
    m_json_data["rs"] = two_decimals(m_telemetry_data.steering_side[0]);
    m_json_data["ls"] = two_decimals(m_telemetry_data.steering_side[1]);
    m_json_data["fx"] = two_decimals(m_telemetry_data.acceleration_x);
    m_json_data["fy"] = two_decimals(m_telemetry_data.acceleration_y);
    m_json_data["rr"] = two_decimals(m_telemetry_data.gyro_z);
    String json;
    serializeJson(m_json_data, json);
    m_esp_now_handler->send_data(json);
}

void Transceiver::set_telemetry_data(TelemetryData &telemetry_data) {
    m_telemetry_data = telemetry_data;
}

float Transceiver::two_decimals(float value) { return round(value * 100) / 100; }

String Transceiver::get_remote_data() { return m_remote_data; }
