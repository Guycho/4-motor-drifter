#ifndef WHEELS_MIXER_H
#define WHEELS_MIXER_H

#include "mav_bridge.h"
#include "config.h"

struct WheelsMixerConfig {
    MavBridge *mav_bridge;
    uint8_t pin[Config::num_wheels];
    uint16_t min_pulse[Config::num_wheels];
    uint16_t max_pulse[Config::num_wheels];
};
struct WheelsMixerData {
    float motor_speed[Config::num_wheels];
};

class WheelsMixer {
   public:
    // Constructor
    WheelsMixer();

    // Destructor
    ~WheelsMixer();

    void init(const WheelsMixerConfig &config);
    void run(WheelsMixerData &wheels_mixer_data);

   private:
    MavBridge m_mav_bridge;
    uint8_t m_pin[Config::num_wheels];
    uint16_t m_min_pulse[Config::num_wheels];
    uint16_t m_max_pulse[Config::num_wheels];
};

#endif  // CONTROL_H