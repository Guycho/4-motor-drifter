#ifndef TRANSCEIVER_H
#define TRANSCEIVER_H

#include <ArduinoJson.h>
#include <WiFi.h>
#include <Chrono.h>
#include "ESP_now_handler.h"
#include "config.h"


struct TransceiverConfig {
    uint16_t update_delay_ms;
    ESPNowHandler *esp_now_handler;
};

struct TelemetryData {
    float throttle;
    float steering;
    uint8_t arm_enabled;
    uint8_t steering_mode;
    uint8_t drive_mode;
    uint16_t motors_rpm[Config::num_wheels];
    float motors_throttle[Config::num_wheels];
    float battery_voltage;
    float steering_side[Config::num_steering];
    float acceleration_x;
    float acceleration_y;
    float gyro_z;
};

class Transceiver {
   public:
    Transceiver();
    ~Transceiver();

    void init(const TransceiverConfig &config);
    void update_data();
    void send_data();
    void set_telemetry_data(TelemetryData &telemetry_data);
    String get_remote_data();

   private:
    float two_decimals(float value);
    Chrono m_data_timer;
    ESPNowHandler *m_esp_now_handler;

    TelemetryData m_telemetry_data;
    String m_remote_data;
    uint16_t m_update_delay_ms;
};

#endif  // ESP32SERVER_HPP