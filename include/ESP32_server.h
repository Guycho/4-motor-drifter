#ifndef ESP32_SERVER_H
#define ESP32_SERVER_H

#include <WebServer.h>
#include <WiFi.h>

struct ESP32ServerConfig {
    const char* ssid;
    const char* password;
    int port;
};

class ESP32Server {
   public:
    ESP32Server();
    ~ESP32Server();

    void init(const ESP32ServerConfig& config);
    void handle_client();

   private:
    const char* m_ssid;
    const char* m_password;
    int m_port;
    WebServer m_server;

    void handle_root();
    void handle_not_found();
};

#endif  // ESP32SERVER_HPP