#ifndef ESP32_SERVER_H
#define ESP32_SERVER_H

#include <ArduinoJson.h>
#include <WebServer.h>
#include <WiFi.h>
#include <Chrono.h>
#include "control.h"

struct ESP32ServerConfig {
    const char* ssid;
    const char* password;
    uint16_t update_delay_ms;
    Control *control;
};

class ESP32Server {
   public:
    ESP32Server();
    ~ESP32Server();

    void init(const ESP32ServerConfig& config);
    void handle_client();

   private:
    WebServer m_server;
    JsonDocument m_json_data;
    Chrono m_data_timer;
    Control *m_control;

    void handle_root();
    void handle_data();
    void handle_not_found();
    void update_data();

    uint16_t m_update_delay_ms;
    float m_max_g_force = 5.0;
    uint16_t m_max_rpm = 5000;
};

#endif  // ESP32SERVER_HPP