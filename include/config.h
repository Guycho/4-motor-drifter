#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

enum WheelsPositions { FR = 0, RR = 1, RL = 2, FL = 3 };
enum SteeringPositions { R = 0, L = 1 };

namespace Config {
extern const uint8_t num_wheels;
extern const uint8_t num_steering;
extern const float min_percentage;
extern const float max_percentage;
extern const float max_omega;
extern const float steering_r_l_ratio;
extern const float trim_increment;
extern const float cs_ratio;
extern const uint16_t hb_timeout;
extern const uint8_t arm_led_pin;

namespace Transceiver {
extern const uint16_t update_delay_ms;
}

namespace Motor {
extern const float deadband;
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

namespace MavlinkBridge {
extern HardwareSerial *serial;
extern const uint32_t baudrate;
extern const uint8_t system_id;
extern const uint8_t component_id;
extern const uint16_t message_rate;
extern const uint32_t is_alive_timeout;
}  // namespace MavlinkBridge

namespace ESPNow {
extern const uint8_t peer_mac_address[];
extern const bool use_lr;
extern const bool print_debug;
}  // namespace ESPNow
}  // namespace Config

#endif  // CONFIG_H