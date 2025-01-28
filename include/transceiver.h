#ifndef TRANSCEIVER_H
#define TRANSCEIVER_H

#include <ArduinoJson.h>
#include <Chrono.h>
#include <WiFi.h>

#include "ESP_now_handler.h"
#include "config.h"

struct RemoteControllerData {
    float throttle = 0;
    float steering = 0;
    bool left_arrow = 0;
    bool right_arrow = 0;
    bool up_arrow = 0;
    bool down_arrow = 0;
    bool sel = 0;
    bool ch = 0;
    bool plus = 0;
    bool minus = 0;
    bool left_trim_l = 0;
    bool left_trim_r = 0;
    bool right_trim_l = 0;
    bool right_trim_r = 0;
    bool edge_switch = 0;
    bool bottom_switch = 0;
    bool new_data = 0;
};

struct DataToSend {
    bool arm_state;
    uint8_t steering_mode;
    uint8_t drive_mode;
    uint8_t battery_status;
};

struct TransceiverConfig {
    uint16_t update_delay_ms;
    ESPNowHandler *esp_now_handler;
};

class Transceiver {
   public:
    Transceiver();
    ~Transceiver();

    void init(const TransceiverConfig &config);
    void run();
    RemoteControllerData get_remote_data();
    void set_data_to_send(const DataToSend &data);

   private:
    void send_data();
    bool verify_checksum(const String &data);
    RemoteControllerData parse_remote_data(const String &data);
    Chrono m_data_timer;
    ESPNowHandler *m_esp_now_handler;

    RemoteControllerData m_remote_controller_data;
    DataToSend m_data_to_send;
    String m_remote_data;
    uint16_t m_update_delay_ms;
};

#endif  // ESP32SERVER_HPP