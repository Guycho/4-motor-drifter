#include "transceiver.h"

Transceiver::Transceiver() : m_control(nullptr) {}

Transceiver::~Transceiver() {}

void Transceiver::init(const TransceiverConfig& config) {
    m_control = config.control;
    m_esp_now_handler = config.esp_now_handler;
    m_update_delay_ms = config.update_delay_ms;

    m_data_timer.start();
   
    Serial.println("Server started");
}

void Transceiver::update_data() {
    if(!m_data_timer.hasPassed(m_update_delay_ms, true)) {
        return;
    }
    ControlPrintData control_data = m_control->get_print_data();
    m_max_g_force = max(m_max_g_force, max(abs(control_data.mavlink_data.inertial_data.acceleration.x),
                                        abs(control_data.mavlink_data.inertial_data.acceleration.y)));
    m_max_rpm = max(max(max(max(m_max_rpm, control_data.mavlink_data.four_motor_speed.motor1_rpm),
                                control_data.mavlink_data.four_motor_speed.motor2_rpm),
                                control_data.mavlink_data.four_motor_speed.motor3_rpm),
                                control_data.mavlink_data.four_motor_speed.motor4_rpm);
    JsonDocument m_json_data;
    m_json_data["t"] = two_decimals(control_data.throttle);
    m_json_data["s"] = two_decimals(control_data.steering);
    m_json_data["a"] = uint8_t(control_data.arm_enabled);
    m_json_data["sm"] = control_data.steering_mode;
    m_json_data["dm"] = control_data.drive_mode;
    m_json_data["1r"] = control_data.mavlink_data.four_motor_speed.motor1_rpm;
    m_json_data["1t"] = two_decimals(abs(control_data.wheels_mixer_data.motor_speed[0]));
    m_json_data["2r"] = control_data.mavlink_data.four_motor_speed.motor2_rpm;
    m_json_data["2t"] = two_decimals(abs(control_data.wheels_mixer_data.motor_speed[1]));
    m_json_data["3r"] = control_data.mavlink_data.four_motor_speed.motor3_rpm;
    m_json_data["3t"] = two_decimals(abs(control_data.wheels_mixer_data.motor_speed[2]));
    m_json_data["4r"] = control_data.mavlink_data.four_motor_speed.motor4_rpm;
    m_json_data["4t"] = two_decimals(abs(control_data.wheels_mixer_data.motor_speed[3]));
    m_json_data["b"] = two_decimals(control_data.mavlink_data.battery_voltage);
    m_json_data["rs"] = two_decimals(control_data.steering_mixer_data.motor_speed[0]);
    m_json_data["ls"] = two_decimals(control_data.steering_mixer_data.motor_speed[1]);
    m_json_data["fx"] = two_decimals(control_data.mavlink_data.inertial_data.acceleration.x);
    m_json_data["fy"] = two_decimals(control_data.mavlink_data.inertial_data.acceleration.y);
    m_json_data["mf"] = two_decimals(m_max_g_force);
    m_json_data["rr"] = two_decimals(control_data.mavlink_data.inertial_data.gyro.z);
    m_json_data["mr"] = m_max_rpm;

    // m_json_data["motor1_rpm"] = 1500;
    // m_json_data["motor2_rpm"] = 5000;
    // m_json_data["motor3_rpm"] = 7500;
    // m_json_data["motor4_rpm"] = 10000;
    String json;
    serializeJson(m_json_data, json);

    m_esp_now_handler->send_data(json);
}

float Transceiver::two_decimals(float value){
    return round(value * 100) / 100;
}
