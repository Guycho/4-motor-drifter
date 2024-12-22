#include "config.h"

namespace Config {
namespace Esc {
namespace Wheel {
const uint8_t pin[num_wheels] = {4, 5, 12, 13};
const uint16_t min_pulse[num_wheels] = {1000, 1000, 1000, 1000};
const uint16_t max_pulse[num_wheels] = {2000, 2000, 2000, 2000};
}  // namespace Wheel
namespace Steering {
const uint8_t pin[num_steering] = {14, 15, 16, 17};
const uint16_t min_pulse[num_steering] = {1000, 1000, 1000, 1000};
const uint16_t max_pulse[num_steering] = {2000, 2000, 2000, 2000};
}  // namespace Steering
namespace Coilover {
const uint8_t pin[num_coilover] = {18, 19, 21, 22};
const uint16_t min_pulse[num_coilover] = {1000, 1000, 1000, 1000};
const uint16_t max_pulse[num_coilover] = {2000, 2000, 2000, 2000};
}  // namespace Coilover
}  // namespace Esc
namespace Encoder {
const uint8_t pin[num_steering] = {36, 39, 34, 35};
const bool reverse[num_steering] = {false, false, false, false};
const float offset[num_steering] = {0, 0, 0, 0};
}  // namespace Encoder
namespace Controller {
const float Kp[num_steering] = {0.1, 0.1, 0.1, 0.1};
const float max_output[num_steering] = {1, 1, 1, 1};
}  // namespace Controller
namespace Wheel {
const float max_speed_pct[4] = {100, 100, 100, 100};
}  // namespace Wheel
namespace Steering {
const float max_angle[num_steering] = {90, 90, 90, 90};
const float max_speed_pct[num_steering] = {100, 100, 100, 100};
}  // namespace Steering
namespace MavlinkBridge {
HardwareSerial *serial = &Serial2;
const uint32_t baudrate = 500000;
const uint8_t system_id = 1;
const uint8_t component_id = 0;
const uint8_t steering_channel = 12;
const uint8_t throttle_channel = 5;
const uint8_t message_rate = 250;
const uint32_t is_alive_timeout = 1000;
}  // namespace MavlinkBridge
namespace PS4Controller {
const char *mac = "A0:DD:6C:03:9A:EE";
const float dead_band = 5;
}  // namespace PS4Controller
namespace Coilover {
const bool reverse[] = {false, false, false, false};
namespace Controller {
const float Kp[] = {0.1, 0.1, 0.1, 0.1};
const float max_output[] = {1, 1, 1, 1};
}  // namespace Controller
}  // namespace Coilover
}  // namespace Config
