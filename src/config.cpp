#include "config.h"

namespace Config {
namespace Transceiver {
const uint16_t update_delay_ms = 25;
}

namespace Motor {
const float deadband = 1.0;
namespace Wheel {
const uint8_t pin[num_wheels] = {1, 2, 3, 4};
const uint16_t min_pulse[num_wheels] = {1000, 1000, 1000, 1000};
const uint16_t max_pulse[num_wheels] = {2000, 2000, 2000, 2000};
}  // namespace Wheel
namespace Steering {
const uint8_t pin[num_steering] = {7, 8};
const uint16_t min_pulse[num_steering] = {1000, 1000};
const uint16_t max_pulse[num_steering] = {2000, 2000};
}  // namespace Steering
}  // namespace Motor

namespace PIDController {
const float kp = 1;
const float ki = 0.1;
const float kd = 0.01;
const float max_output = 100;
const float integral_percentage = 30;
const float low_pass_alpha = 0.1;
const float high_Pass_alpha = 0.1;
const bool use_filters = true;
}  // namespace PIDController

namespace MavlinkBridge {
HardwareSerial *serial = &Serial2;
const uint32_t baudrate = 2000000;
const uint8_t system_id = 1;
const uint8_t component_id = 0;
const uint16_t message_rate = 100;
const uint32_t is_alive_timeout = 2500;
}  // namespace MavlinkBridge

namespace ESPNow {
const uint8_t peer_mac_address[] = {0xA0, 0xDD, 0x6C, 0x03, 0x9E, 0x40};
const bool use_lr = true;
const bool print_debug = false;
}  // namespace ESPNow
}  // namespace Config