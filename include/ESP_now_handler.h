#ifndef ESPNOWHANDLER_H
#define ESPNOWHANDLER_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

class ESPNowHandler {
   public:
    ESPNowHandler(const uint8_t *peerMacAddress);
    void init();
    void send_data(const String &data);
    void on_data_sent(const uint8_t *mac_addr, esp_now_send_status_t status);
    void on_data_recv(const uint8_t *mac_addr, const uint8_t *data, int data_len);
    String get_data();

   private:
    uint8_t peerMacAddress[6];
    static void on_data_sent_static(const uint8_t *mac_addr, esp_now_send_status_t status);
    static void on_data_recv_static(const uint8_t *mac_addr, const uint8_t *data, int data_len);
    static ESPNowHandler *instance;

    String m_data;
};

#endif  // ESPNOWHANDLER_H