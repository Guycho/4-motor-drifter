#ifndef STEERING_MIXER_H
#define STEERING_MIXER_H

#include "mav_bridge.h"
#include "config.h"

struct SteeringMixerConfig {
    MavBridge *mav_bridge;
    uint8_t pin[Config::num_steering];
    uint16_t min_pulse[Config::num_steering];
    uint16_t max_pulse[Config::num_steering];
};
struct SteeringMixerData {
    float motor_speed[Config::num_steering];
};

class SteeringMixer {
   public:
    // Constructor
    SteeringMixer();

    // Destructor
    ~SteeringMixer();

    void init(const SteeringMixerConfig &config);
    void run(SteeringMixerData &Steering_mixer_data);

   private:
    MavBridge m_mav_bridge;
    uint8_t m_pin[Config::num_steering];
    uint16_t m_min_pulse[Config::num_steering];
    uint16_t m_max_pulse[Config::num_steering];
};

#endif  // CONTROL_H