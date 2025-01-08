#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

enum WheelsPositions { FR = 0, RR = 1, RL = 2, FL = 3 };
enum SteeringPositions { R = 0, L = 1 };

namespace Config {
namespace Car {
const uint8_t num_wheels = 4;
const uint8_t num_steering = 2;
const float min_percentage = -100.0;
const float max_percentage = 100.0;
const float max_omega = 360; // deg/s
const float steering_r_l_ratio = 0.1;
const float trim_increment = 0.01;
const float cs_ratio = 0.7;
namespace Transceiver {
extern const uint16_t update_delay_ms;
}  // namespace Transceiver
namespace Motor {
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
}  // namespace Motor
namespace PIDController {
extern const float kp;
extern const float ki;
extern const float kd;
extern const float max_output;
extern const float integral_percentage;
extern const float low_pass_alpha;
extern const float high_Pass_alpha;
extern const bool use_filters;
}  // namespace PIDController
namespace PController {
extern const float kp;
extern const float max_output;
}  // namespace PController
namespace MavlinkBridge {
extern HardwareSerial *serial;
extern const uint32_t baudrate;
extern const uint8_t system_id;
extern const uint8_t component_id;
extern const uint16_t message_rate;
extern const uint32_t is_alive_timeout;
}  // namespace MavlinkBridge
namespace PS4Controller {
extern const char *mac;
extern const float dead_band;
}  // namespace PS4Controller
namespace ESPNow {
extern const uint8_t peer_mac_address[];
}  // namespace ESPNow
}  // namespace Car
}  // namespace Config
#endif  // CONFIG_H