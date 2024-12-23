#ifndef STEERING_MIXER_H
#define STEERING_MIXER_H

#include "config.h"
#include "mav_bridge.h"

struct SteeringMixerConfig {
    MavBridge *mav_bridge;
    uint8_t pin[Config::num_steering];
    uint16_t min_pulse[Config::num_steering];
    uint16_t max_pulse[Config::num_steering];
};
struct SteeringMixerData {
    float motor_speed[Config::num_steering];
};

enum SteeringMode { NORMAL = 0, GYRO = 1, OMEGA = 2 };

class SteeringMixer {
   public:
    // Constructor
    SteeringMixer();

    // Destructor
    ~SteeringMixer();

    void init(const SteeringMixerConfig &config);
    void run(SteeringMixerData &Steering_mixer_data);
    uint8_t get_num_of_steering_modes();

   private:
    MavBridge m_mav_bridge;
    uint8_t m_pin[Config::num_steering];
    uint16_t m_min_pulse[Config::num_steering];
    uint16_t m_max_pulse[Config::num_steering];

    static constexpr SteeringMode steering_modes[] = {NORMAL, GYRO, OMEGA};
    static constexpr uint8_t NUM_STEERING_MODES =
      sizeof(steering_modes) / sizeof(steering_modes[0]);
};

#endif  // CONTROL_H