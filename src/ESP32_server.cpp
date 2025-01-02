#include "ESP32_server.h"

ESP32Server::ESP32Server() {}

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
    m_server.on("/data", HTTP_GET, std::bind(&ESP32Server::handle_data, this));
    m_server.onNotFound(std::bind(&ESP32Server::handle_not_found, this));
    m_server.begin();
    Serial.println("Server started");
}

void ESP32Server::handle_client() {
    m_server.handleClient();
}

void ESP32Server::handle_root() {
    File file = SPIFFS.open("/index.html", "r");
    if (!file) {
        Serial.println("Failed to open file");
        m_server.send(500, "text/plain", "Internal Server Error");
        return;
    }

    String html = file.readString();
    file.close();

    m_server.send(200, "text/html", html);
}

void ESP32Server::handle_data() {

    // Replace with actual data fetching logic
    ControlPrintData control_data = m_control->get_print_data();
    StaticJsonDocument<1024> doc;
    doc["throttle"] = control_data.throttle;
    doc["steering"] = control_data.steering;
    doc["arm_enabled"] = control_data.arm_enabled;
    doc["steering_mode"] = control_data.steering_mode;
    doc["drive_mode"] = control_data.drive_mode;
 
    String json;
    serializeJson(doc, json);

    m_server.send(200, "application/json", json);
}

void ESP32Server::handle_not_found() {
    m_server.send(404, "text/plain", "404: Not Found");
}