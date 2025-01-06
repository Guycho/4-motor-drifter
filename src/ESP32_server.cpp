#include "ESP32_server.h"

ESP32Server::ESP32Server() : m_control(nullptr) {}

ESP32Server::~ESP32Server() {}

void ESP32Server::init(const ESP32ServerConfig& config) {
    m_control = config.control;

    WiFi.softAP(config.ssid, config.password);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    m_server.on("/", HTTP_GET, std::bind(&ESP32Server::handle_root, this));
    m_server.on("/data", HTTP_GET, std::bind(&ESP32Server::handle_data, this));
    m_server.onNotFound(std::bind(&ESP32Server::handle_not_found, this));
    m_server.begin();
    Serial.println("Server started");
}

void ESP32Server::handle_root() { m_server.send(200, "text/plain", "ESP32 Server"); }

void ESP32Server::handle_data() {
    String json;
    serializeJson(json_data, json);

    m_server.send(200, "application/json", json);
}

void ESP32Server::update_data() {
    ControlPrintData control_data = m_control->get_print_data();
    m_max_g_force =
      max(m_max_g_force, max(abs(control_data.mavlink_data.inertial_data.acceleration.x),
                           abs(control_data.mavlink_data.inertial_data.acceleration.y)));
    m_max_rpm = max(max(max(max(m_max_rpm, control_data.mavlink_data.four_motor_speed.motor1_rpm),
                          control_data.mavlink_data.four_motor_speed.motor2_rpm),
                      control_data.mavlink_data.four_motor_speed.motor3_rpm),
      control_data.mavlink_data.four_motor_speed.motor4_rpm);
    json_data["throttle"] = control_data.throttle;
    json_data["steering"] = control_data.steering;
    json_data["arm_enabled"] = control_data.arm_enabled;
    json_data["steering_mode"] = control_data.steering_mode == 0 ? "Normal" : "Gyro";
    json_data["drive_mode"] = control_data.drive_mode == 0   ? "AWD"
                              : control_data.drive_mode == 1 ? "CS"
                                                             : "RWD";
    json_data["motor1_rpm"] = control_data.mavlink_data.four_motor_speed.motor1_rpm;
    json_data["motor1_throttle"] = abs(control_data.wheels_mixer_data.motor_speed[0]);
    json_data["motor2_rpm"] = control_data.mavlink_data.four_motor_speed.motor2_rpm;
    json_data["motor2_throttle"] = abs(control_data.wheels_mixer_data.motor_speed[1]);
    json_data["motor3_rpm"] = control_data.mavlink_data.four_motor_speed.motor3_rpm;
    json_data["motor3_throttle"] = abs(control_data.wheels_mixer_data.motor_speed[2]);
    json_data["motor4_rpm"] = control_data.mavlink_data.four_motor_speed.motor4_rpm;
    json_data["motor4_throttle"] = abs(control_data.wheels_mixer_data.motor_speed[3]);
    json_data["g_force_x"] = control_data.mavlink_data.inertial_data.acceleration.x;
    json_data["g_force_y"] = control_data.mavlink_data.inertial_data.acceleration.y;
    json_data["max_g_force"] = m_max_g_force;
    json_data["rotational_rate"] = control_data.mavlink_data.inertial_data.gyro.z;
    json_data["max_rpm"] = m_max_rpm;
}
void ESP32Server::handle_not_found() { m_server.send(404, "text/plain", "File Not Found"); }

void ESP32Server::handle_client() {
    m_server.handleClient();
    update_data();
}
