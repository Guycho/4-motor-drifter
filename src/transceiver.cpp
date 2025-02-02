#include "transceiver.h"

Transceiver::Transceiver() {}

Transceiver::~Transceiver() {}

void Transceiver::init(const TransceiverConfig &config) {
    m_esp_now_handler = config.esp_now_handler;
    m_update_delay_ms = config.update_delay_ms;
    m_data_timer.start();
}

void Transceiver::run() {
    if (!m_data_timer.hasPassed(m_update_delay_ms, true)) {
        return;
    }
    m_remote_data = m_esp_now_handler->get_data();
    if (!(m_remote_data.length() == 0 || !verify_checksum(m_remote_data))) {
        m_remote_controller_data = parse_remote_data(m_remote_data);
    }
    send_data();
}

RemoteControllerData Transceiver::get_remote_data() {
    RemoteControllerData remote_data = m_remote_controller_data;
    m_remote_controller_data.new_data = false;
    return remote_data;
}

void Transceiver::set_telemetry_data(const TelemetryData &data) { m_telemetry_data = data; }

bool Transceiver::verify_checksum(const String &data) {
    JsonDocument m_json_data;
    deserializeJson(m_json_data, data);
    // Extract the checksum from the JSON document
    uint8_t received_checksum = m_json_data["c"];
    // Remove the checksum from the JSON document
    m_json_data.remove("c");
    String json;
    serializeJson(m_json_data, json);
    
    uint8_t calculated_checksum = Calcs::calc_checksum(json);
    if (received_checksum != calculated_checksum) {
        return false;  // Checksum mismatch;
    }
    return true;
}

RemoteControllerData Transceiver::parse_remote_data(const String &data) {
    JsonDocument m_json_data;
    deserializeJson(m_json_data, data);
    // Extract the bitmask from the JSON document
    uint32_t bitmask = m_json_data["b"];
    RemoteControllerData remote_data;
    // Unpack the bitmask
    remote_data.throttle = (((bitmask >> 0) & 0x1FF) / 2.555) - 100;  // 9 bits for throttle
    remote_data.steering = (((bitmask >> 9) & 0x1FF) / 2.555) - 100;  // 9 bits for steering
    // compensate for resolution loss in the remote
    if (abs(remote_data.throttle) < 0.3) {
        remote_data.throttle = 0;
    }
    if (abs(remote_data.steering) < 0.3) {
        remote_data.steering = 0;
    }
    remote_data.left_arrow = (bitmask >> 18) & 0x1;
    remote_data.right_arrow = (bitmask >> 19) & 0x1;
    remote_data.up_arrow = (bitmask >> 20) & 0x1;
    remote_data.down_arrow = (bitmask >> 21) & 0x1;
    remote_data.sel = (bitmask >> 22) & 0x1;
    remote_data.ch = (bitmask >> 23) & 0x1;
    remote_data.plus = (bitmask >> 24) & 0x1;
    remote_data.minus = (bitmask >> 25) & 0x1;
    remote_data.left_trim_l = (bitmask >> 26) & 0x1;
    remote_data.left_trim_r = (bitmask >> 27) & 0x1;
    remote_data.right_trim_l = (bitmask >> 28) & 0x1;
    remote_data.right_trim_r = (bitmask >> 29) & 0x1;
    remote_data.edge_switch = (bitmask >> 30) & 0x1;
    remote_data.bottom_switch = (bitmask >> 31) & 0x1;
    remote_data.new_data = true;
    return remote_data;
}

void Transceiver::send_data() {
    JsonDocument m_json_data;
    String json;

    const uint8_t bitmask_size = sizeof(typeof(m_telemetry_data));
    m_json_data["s"] = bitmask_size;

    std::bitset<bitmask_size> bitmask;

    uint8_t *data_ptr = reinterpret_cast<uint8_t *>(&m_telemetry_data);
    for (size_t i = 0; i < sizeof(typeof(m_telemetry_data)); ++i) {
        std::bitset<8> byte(data_ptr[i]);
        bitmask |= (std::bitset<bitmask_size>(byte.to_ulong()) << (i * 8));
    }

    m_json_data["b"] = bitmask.to_ullong();

    serializeJson(m_json_data, json);

    m_json_data["c"] = Calcs::calc_checksum(json);
    serializeJson(m_json_data, json);
    m_esp_now_handler->send_data(json);
}
