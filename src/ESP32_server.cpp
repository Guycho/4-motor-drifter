#include "ESP32_server.h"

ESP32Server::ESP32Server() {}

ESP32Server::~ESP32Server() {}

void ESP32Server::init(const ESP32ServerConfig& config) {
    m_ssid = config.ssid;
    m_password = config.password;
    m_port = config.port;

    WiFi.softAP(m_ssid, m_password);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    m_server.on("/", HTTP_GET, std::bind(&ESP32Server::handle_root, this));
    m_server.onNotFound(std::bind(&ESP32Server::handle_not_found, this));
    m_server.begin();
    Serial.println("Server started");
}

void ESP32Server::handle_client() {
    m_server.handleClient();
    // Serial.println("Client handled");
}

void ESP32Server::handle_root() {
    m_server.send(200, "text/plain", "Hello, world!");
    Serial.println("root accesses");
}

void ESP32Server::handle_not_found() {
    m_server.send(404, "text/plain", "404: Not Found");
    Serial.println("Not found accessed");
}