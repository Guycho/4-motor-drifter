#include "ESP_now_handler.h"

ESPNowHandler *ESPNowHandler::instance = nullptr;

ESPNowHandler::ESPNowHandler(const uint8_t *peerMacAddress) {
    memcpy(this->peerMacAddress, peerMacAddress, 6);
    instance = this;
}

void ESPNowHandler::init() {
    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    // Set Wi-Fi to long-range mode
    esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_LR);

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register the send callback
    esp_now_register_send_cb(on_data_sent_static);

    // Register the receive callback
    esp_now_register_recv_cb(on_data_recv_static);

    // Register peer
    esp_now_peer_info_t peerInfo;
    memset(&peerInfo, 0, sizeof(peerInfo));
    memcpy(peerInfo.peer_addr, peerMacAddress, 6);
    peerInfo.channel = 0;  // Use the same channel as the sender
    peerInfo.encrypt = false;

    // Add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }
}

void ESPNowHandler::send_data(const String &data) {
    esp_err_t result = esp_now_send(peerMacAddress, (uint8_t *)data.c_str(), data.length());

    if (result == ESP_OK) {
        Serial.println("Sent with success");
    } else {
        const char *errorName = esp_err_to_name(result);
        Serial.print("Error sending data: ");
        Serial.println(errorName);
    }
}

void ESPNowHandler::on_data_sent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("Last Packet Send Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void ESPNowHandler::on_data_recv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
    Serial.print("Received data from: ");
    for (int i = 0; i < 6; i++) {
        Serial.printf("%02x", mac_addr[i]);
        if (i < 5) Serial.print(":");
    }
    Serial.print(" - Data: ");
    Serial.write(data, data_len);
    Serial.println();
}

void ESPNowHandler::on_data_sent_static(const uint8_t *mac_addr, esp_now_send_status_t status) {
    if (instance) {
        instance->on_data_sent(mac_addr, status);
    }
}

void ESPNowHandler::on_data_recv_static(const uint8_t *mac_addr, const uint8_t *data,
  int data_len) {
    if (instance) {
        instance->on_data_recv(mac_addr, data, data_len);
    }
}