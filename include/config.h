#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

enum Positions { FR = 0, RR = 1, RL = 2, FL = 3 };

namespace Config {
const float width = 0.5;
const float length = 0.5;
const float right_angle = 90;
const uint8_t num_wheels = 4;
const uint8_t num_steering = 4;
const uint8_t num_coilover = 4;
namespace Esc {
namespace Wheel {
extern const uint8_t pin[];
extern const uint16_t min_pulse[];
extern const uint16_t max_pulse[];
}  // namespace Wheel
namespace Steering {
extern const uint8_t pin[];
extern const uint16_t min_pulse[];
extern const uint16_t max_pulse[];
}  // namespace Steering
namespace Coilover {
extern const uint8_t pin[];
extern const uint16_t min_pulse[];
extern const uint16_t max_pulse[];
}  // namespace Coilover
}  // namespace Esc
namespace Encoder {
extern const uint8_t pin[];
extern const bool reverse[];
extern const float offset[];
}  // namespace Encoder
namespace Controller {
extern const float Kp[];
extern const float max_output[];
}  // namespace Controller
namespace Wheel {
extern const float max_speed_pct[];
}  // namespace Wheel
namespace Steering {
extern const float max_angle[];
extern const float max_speed_pct[];
}  // namespace Steering
namespace MavlinkBridge {
extern HardwareSerial *serial;
extern const uint32_t baudrate;
extern const uint8_t system_id;
extern const uint8_t component_id;
extern const uint8_t steering_channel;
extern const uint8_t throttle_channel;
extern const uint8_t message_rate;
extern const uint32_t is_alive_timeout;
}  // namespace MavlinkBridge
namespace PS4Controller {
extern const char *mac;
extern const float dead_band;
}  // namespace PS4Controller
namespace Coilover {
extern const bool reverse[];
namespace Controller {
extern const float Kp[];
extern const float max_output[];
}  // namespace Controller
}  // namespace Coilover
}  // namespace Config
#endif  // CONFIG_H