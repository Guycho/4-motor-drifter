#include "steering_mixer.h"

SteeringMixer::SteeringMixer() {}
SteeringMixer::~SteeringMixer() {}

void SteeringMixer::init(const SteeringMixerConfig &config) {
    m_mav_bridge = *config.mav_bridge;
    for (int i = 0; i < Config::num_steering; i++) {
        m_pin[i] = config.pin[i];
        m_min_pulse[i] = config.min_pulse[i];
        m_max_pulse[i] = config.max_pulse[i];
    }
}

void SteeringMixer::run(SteeringMixerData &Steering_mixer_data) {
    for (int i = 0; i < Config::num_steering; i++) {
        MotorSpeed motor_speed;
        motor_speed.motor_pin = m_pin[i];
        float temp_value = Utils::Calcs::map_float(Steering_mixer_data.motor_speed[i],
          Config::min_percentage, Config::max_percentage, m_min_pulse[i], m_max_pulse[i]);
        motor_speed.motor_value =
          Utils::Calcs::constrain_float(temp_value, m_min_pulse[i], m_max_pulse[i]);
        m_mav_bridge.set_motor_speed(motor_speed);
    }
}
