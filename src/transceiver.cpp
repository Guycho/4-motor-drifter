#include "transceiver.h"

Transceiver::Transceiver() {}

Transceiver::~Transceiver() {}

void Transceiver::init(const TransceiverConfig &config)
{
    m_esp_now_handler = config.esp_now_handler;
    m_update_delay_ms = config.update_delay_ms;
    m_data_timer.start();
}

void Transceiver::run()
{
    if (!m_data_timer.hasPassed(m_update_delay_ms, true))
    {
        return;
    }
    m_remote_data = m_esp_now_handler->get_data();
    if (!(m_remote_data.length() == 0 || !verify_checksum(m_remote_data)))
    {
        m_remote_controller_data = parse_remote_data(m_remote_data);
    }
    send_data();
}

RemoteControllerData Transceiver::get_remote_data()
{
    RemoteControllerData remote_data = m_remote_controller_data;
    m_remote_controller_data.new_data = false;
    return remote_data;
}

void Transceiver::set_telemetry_data(const TelemetryData &data)
{
    m_telemetry_data = data;
}
bool Transceiver::verify_checksum(const String &data)
{
    JsonDocument m_json_data;
    deserializeJson(m_json_data, data);
    // Extract the checksum from the JSON document
    uint8_t received_checksum = m_json_data["c"];

    // Calculate the checksum (XOR of all bytes except the checksum itself)
    uint8_t calculated_checksum = 0;
    for (size_t i = 0; i < data.length(); ++i)
    {
        if (data[i] == ',')
            break; // Stop before the checksum field
        calculated_checksum ^= data[i];
    }
    calculated_checksum ^= '}';
    if (received_checksum != calculated_checksum)
    {
        return false; // Checksum mismatch;
    }
    return true;
}

RemoteControllerData Transceiver::parse_remote_data(const String &data)
{
    JsonDocument m_json_data;
    deserializeJson(m_json_data, data);
    // Extract the bitmask from the JSON document
    uint32_t bitmask = m_json_data["b"];
    RemoteControllerData remote_data;
    // Unpack the bitmask
    remote_data.throttle = (((bitmask >> 0) & 0x1FF) / 2.555) - 100; // 9 bits for throttle
    remote_data.steering = (((bitmask >> 9) & 0x1FF) / 2.555) - 100; // 9 bits for steering
    // compensate for resolution loss in the remote
    if (abs(remote_data.throttle) < 0.3)
    {
        remote_data.throttle = 0;
    }
    if (abs(remote_data.steering) < 0.3)
    {
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

void Transceiver::send_data()
{
    JsonDocument m_json_data;
    String json;
    uint64_t bitmask = 0;
    uint8_t scaled_battery_voltage =
        static_cast<uint8_t>((m_telemetry_data.battery_voltage) * 10);
    uint16_t scaled_motor_throttle[Config::num_wheels];
    for (int i = 0; i < Config::num_wheels; i++)
    {
        scaled_motor_throttle[i] = static_cast<uint16_t>((m_telemetry_data.motors_throttle[i] + 100) * 2.555);
    }
    uint16_t scaled_steering_values[Config::num_steering];
    for (int i = 0; i < Config::num_steering; i++)
    {
        scaled_steering_values[i] = static_cast<uint16_t>((m_telemetry_data.steering_valus[i] + 100) * 2.555);
    }
    uint16_t scaled_g_force_x =
        static_cast<uint16_t>((m_telemetry_data.g_force_x + 5) * 102.3);
    uint16_t scaled_g_force_y =
        static_cast<uint16_t>((m_telemetry_data.g_force_y + 5) * 102.3);
    uint16_t scaled_rotation_rate_z =
        static_cast<uint16_t>((m_telemetry_data.rotation_rate_z + 360) * 2.843);

    bitmask |= (scaled_battery_voltage << 0);
    bitmask |= (m_telemetry_data.arm_state << 8);
    
    bitmask |= (m_telemetry_data.steering_mode << 9);
    bitmask |= (m_telemetry_data.drive_mode << 11);
    bitmask |= (m_telemetry_data.battery_status << 13);
    bitmask |= (scaled_motor_throttle[0] << 15);
    bitmask |= (scaled_motor_throttle[1] << 24);
    bitmask |= (scaled_motor_throttle[2] << 33);
    bitmask |= (scaled_motor_throttle[3] << 42);
    bitmask |= (m_telemetry_data.motors_rpm[0] << 55);
    bitmask |= (m_telemetry_data.motors_rpm[1] << 68);
    bitmask |= (m_telemetry_data.motors_rpm[2] << 81);
    bitmask |= (m_telemetry_data.motors_rpm[3] << 94);
    bitmask |= (scaled_steering_values[0] << 107);
    bitmask |= (scaled_steering_values[1] << 116);
    bitmask |= (scaled_g_force_x << 125);
    bitmask |= (scaled_g_force_y << 134);
    bitmask |= (scaled_rotation_rate_z << 143);


    m_json_data["b"] = bitmask;
    serializeJson(m_json_data, json);

    uint8_t checksum = 0;
    for (char c : json)
    {
        checksum ^= c;
    }
    m_json_data["c"] = checksum;
    serializeJson(m_json_data, json);
    m_esp_now_handler->send_data(json);
}
