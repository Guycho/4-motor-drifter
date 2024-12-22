#include "mav_bridge.h"

MavBridge::MavBridge() {}
MavBridge::~MavBridge() {}
void MavBridge::init(const MavBridgeConfig &config) {
    m_serial = config.serial;
    m_baudrate = config.baudrate;
    m_system_id = config.system_id;
    m_component_id = config.component_id;
    m_message_rate = config.message_rate;
    m_is_alive_timeout = config.is_alive_timeout;

    m_serial->begin(m_baudrate);
    set_messages_rates();
    m_is_alive_timer.start();
    // Initialize other necessary setups
}

void MavBridge::run() {
    m_inertial_data.is_alive = m_is_alive_timer.hasPassed(m_is_alive_timeout);
    mavlink_message_t msg;
    mavlink_status_t status;

    uint16_t len = m_serial->available();
    uint8_t buf[len];
    m_serial->readBytes(buf, len);
    for (uint16_t i = 0; i < len; i++) {
        uint8_t c = buf[i];
        if (mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {
            if (msg.msgid == MAVLINK_MSG_ID_SCALED_IMU) {
                mavlink_scaled_imu_t imu;
                mavlink_msg_scaled_imu_decode(&msg, &imu);
                m_inertial_data.accel.x = Utils::Calcs::milli_to_single(imu.xacc);
                m_inertial_data.accel.y = Utils::Calcs::milli_to_single(imu.yacc);
                m_inertial_data.accel.z = Utils::Calcs::milli_to_single(imu.zacc);
                m_inertial_data.gyro.x = Utils::Calcs::milli_to_single(imu.xgyro);
                m_inertial_data.gyro.y = Utils::Calcs::milli_to_single(imu.ygyro);
                m_inertial_data.gyro.z = Utils::Calcs::milli_to_single(imu.zgyro);
            }
            if (msg.msgid == MAVLINK_MSG_ID_ATTITUDE) {
                mavlink_attitude_t att;
                mavlink_msg_attitude_decode(&msg, &att);
                m_inertial_data.orientation.x = Utils::Calcs::rad_to_deg(att.roll);
                m_inertial_data.orientation.y = Utils::Calcs::rad_to_deg(att.pitch);
                m_inertial_data.orientation.z = Utils::Calcs::rad_to_deg(att.yaw);
            }
            if (msg.msgid == MAVLINK_MSG_ID_HEARTBEAT) {
                m_is_alive_timer.restart();
            }
        }
    }
}

void MavBridge::set_messages_rates() {
    set_message_rate(MAVLINK_MSG_ID_SCALED_IMU, m_message_rate);
    set_message_rate(MAVLINK_MSG_ID_HEARTBEAT, m_message_rate);
    set_message_rate(MAVLINK_MSG_ID_ATTITUDE, m_message_rate);
}

void MavBridge::set_message_rate(uint32_t msg_id, uint16_t message_rate_hz) {
    uint32_t interval_us = 1e6 / message_rate_hz;
    MavMsg m_mav_msg;
    m_mav_msg.system_id = m_system_id;
    m_mav_msg.component_id = m_component_id;
    m_mav_msg.msg_id = MAV_CMD_SET_MESSAGE_INTERVAL;
    m_mav_msg.params[0] = msg_id;
    m_mav_msg.params[1] = interval_us;
    send_mavlink_message(m_mav_msg);
}

void MavBridge::send_mavlink_message(const MavMsg &mav_msg) {
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];

    // Pack the COMMAND_LONG message
    mavlink_msg_command_long_pack(mav_msg.system_id,  // Your system ID
      mav_msg.component_id,                           // Your component ID
      &msg,
      mav_msg.system_id,     // Target system ID
      mav_msg.component_id,  // Target component ID
      mav_msg.msg_id,        // Command ID
      0,                     // Confirmation
      mav_msg.params[0],     // Param 1
      mav_msg.params[1],     // Param 2
      mav_msg.params[2],     // Param 3
      mav_msg.params[3],     // Param 4
      mav_msg.params[4],     // Param 5
      mav_msg.params[5],     // Param 6
      mav_msg.params[6]      // Param 7
    );

    // Serialize the message
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);

    // Send the message over the serial interface
    m_serial->write(buf, len);
}

InertialData MavBridge::get_inertial_data() {
    return m_inertial_data;
}