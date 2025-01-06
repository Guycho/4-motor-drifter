#ifndef MAV_BRIDGE_H
#define MAV_BRIDGE_H

#include <Arduino.h>
#include <all/mavlink.h>
#include <Chrono.h>

#include "utils.h"
struct MavBridgeConfig {
    HardwareSerial *serial;
    uint32_t baudrate;
    uint8_t system_id;
    uint8_t component_id;
    uint8_t message_rate;
    uint32_t is_alive_timeout;
};

struct Vector3D {
    float x;
    float y;
    float z;
};

struct InertialData {
    Vector3D gyro;
    Vector3D orientation;
    Vector3D acceleration;
};

struct MavMsg {
    uint8_t system_id;
    uint8_t component_id;
    uint16_t msg_id;
    float params[7] = {0, 0, 0, 0, 0, 0, 0};

};

struct MotorSpeed {
    uint8_t motor_pin;
    uint16_t motor_value;
};

struct FourMotorSpeed {
    uint16_t motor1_rpm;
    uint16_t motor2_rpm;
    uint16_t motor3_rpm;
    uint16_t motor4_rpm;
};

struct MavlinkData {
    InertialData inertial_data;
    FourMotorSpeed four_motor_speed;
    float battery_voltage;
    bool is_alive = false;
};

class MavBridge {
   public:
    MavBridge();
    ~MavBridge();

    void init(const MavBridgeConfig &mav__msg);
    void run();
    void set_motor_speed(MotorSpeed motor_speed);
    MavlinkData get_mavlink_data();

   private:
    void set_messages_rates();
    void set_message_rate(uint32_t msg_id, uint16_t message_rate_hz);
    void send_mavlink_message(const MavMsg &mav_msg);

    HardwareSerial *m_serial;
    uint32_t m_baudrate;
    uint8_t m_system_id;
    uint8_t m_component_id;
    uint8_t m_message_rate;
    uint16_t m_is_alive_timeout;

    Chrono m_is_alive_timer;
    MavlinkData m_mavlink_data;
};

#endif  // MAV_BRIDGE_H