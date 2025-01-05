#include "ESP32_server.h"

ESP32Server::ESP32Server() : m_control(nullptr) {}

ESP32Server::~ESP32Server() {}

void ESP32Server::init(const ESP32ServerConfig& config) {
    m_control = config.control;

    WiFi.softAP(config.ssid, config.password);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    m_server.on("/", HTTP_GET, std::bind(&ESP32Server::handle_root, this));
    m_server.on("/raphael.min.js", HTTP_GET,
      std::bind(&ESP32Server::handle_file, this, "/raphael.min.js"));
    m_server.on("/justgage.min.js", HTTP_GET,
      std::bind(&ESP32Server::handle_file, this, "/justgage.min.js"));
    m_server.on("/styles.css", HTTP_GET, std::bind(&ESP32Server::handle_file, this, "/styles.css"));
    m_server.on("/scripts.js", HTTP_GET, std::bind(&ESP32Server::handle_file, this, "/scripts.js"));
    m_server.on("/data", HTTP_GET, std::bind(&ESP32Server::handle_data, this));
    m_server.onNotFound(std::bind(&ESP32Server::handle_not_found, this));
    m_server.begin();
    Serial.println("Server started");
}

void ESP32Server::handle_client() { m_server.handleClient(); }

void ESP32Server::handle_root() {
    File file = SPIFFS.open("/index.html", "r");
    if (!file) {
        Serial.println("Failed to open file");
        m_server.send(500, "text/plain", "Internal Server Error");
        return;
    }

    String html = file.readString();
    file.close();

    m_server.send(1000, "text/html", html);
}

void ESP32Server::handle_file(const char* path) {
    File file = SPIFFS.open(path, "r");
    if (!file) {
        Serial.println("Failed to open file");
        m_server.send(404, "text/plain", "File Not Found");
        return;
    }

    String contentType = "text/plain";
    if (String(path).endsWith(".js")) contentType = "application/javascript";
    if (String(path).endsWith(".css")) contentType = "text/css";
    if (String(path).endsWith(".html")) contentType = "text/html";

    m_server.streamFile(file, contentType);
    file.close();
}

void ESP32Server::handle_data() {
    ControlPrintData control_data = m_control->get_print_data();
    StaticJsonDocument<1024> doc;
    m_max_g_force =
      max(m_max_g_force, max(abs(control_data.mavlink_data.inertial_data.acceleration.x),
                           abs(control_data.mavlink_data.inertial_data.acceleration.y)));
    m_max_rpm = max(max(max(max(m_max_rpm, control_data.mavlink_data.four_motor_speed.motor1_rpm),
                                   control_data.mavlink_data.four_motor_speed.motor2_rpm),
                                   control_data.mavlink_data.four_motor_speed.motor3_rpm),
                                   control_data.mavlink_data.four_motor_speed.motor4_rpm); 
    doc["throttle"] = control_data.throttle;
    doc["steering"] = control_data.steering;
    doc["arm_enabled"] = control_data.arm_enabled;
    doc["steering_mode"] = control_data.steering_mode == 0 ? "Normal" : "Gyro";
    doc["drive_mode"] = control_data.drive_mode == 0   ? "AWD"
                        : control_data.drive_mode == 1 ? "CS"
                                                       : "RWD";
    doc["motor1_rpm"] = control_data.mavlink_data.four_motor_speed.motor1_rpm;
    doc["motor1_throttle"] = abs(control_data.wheels_mixer_data.motor_speed[0]);
    doc["motor2_rpm"] = control_data.mavlink_data.four_motor_speed.motor2_rpm;
    doc["motor2_throttle"] = abs(control_data.wheels_mixer_data.motor_speed[1]);
    doc["motor3_rpm"] = control_data.mavlink_data.four_motor_speed.motor3_rpm;
    doc["motor3_throttle"] = abs(control_data.wheels_mixer_data.motor_speed[2]);
    doc["motor4_rpm"] = control_data.mavlink_data.four_motor_speed.motor4_rpm;
    doc["motor4_throttle"] = abs(control_data.wheels_mixer_data.motor_speed[3]);
    doc["g_force_x"] = control_data.mavlink_data.inertial_data.acceleration.x;
    doc["g_force_y"] = control_data.mavlink_data.inertial_data.acceleration.y;
    doc["max_g_force"] = m_max_g_force;
    doc["rotational_rate"] = control_data.mavlink_data.inertial_data.gyro.z;
    doc["max_rpm"] = 5421;  // Add max RPM value

    String json;
    serializeJson(doc, json);

    m_server.send(1000, "application/json", json);
}

void ESP32Server::handle_not_found() { m_server.send(404, "text/plain", "File Not Found"); }