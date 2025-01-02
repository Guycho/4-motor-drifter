#ifndef ESP32_SERVER_H
#define ESP32_SERVER_H

#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>
#include <WebServer.h>
#include <WiFi.h>
#include "control.h"

struct ESP32ServerConfig {
    const char* ssid;
    const char* password;
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

    Control *m_control;

    void handle_root();
    void handle_data();
    void handle_file(const char* path);
    void handle_not_found();
};

#endif  // ESP32SERVER_HPP