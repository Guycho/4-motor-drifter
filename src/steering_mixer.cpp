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
        apply_trim(Steering_mixer_data);
        float temp_value = Utils::Calcs::map_float(Steering_mixer_data.motor_speed[i],
          Config::min_percentage, Config::max_percentage, m_min_pulse[i], m_max_pulse[i]);
        float final_value =
          Utils::Calcs::constrain_float(temp_value, m_min_pulse[i], m_max_pulse[i]);
        motor_speed.motor_value = final_value;
        m_mav_bridge.set_motor_speed(motor_speed);
    }
}

void SteeringMixer::set_trim(SteeringMixerData &Steering_mixer_data) {
    m_trim = Steering_mixer_data;
}

void SteeringMixer::apply_trim(SteeringMixerData &Steering_mixer_data) {
    for (int i = 0; i < Config::num_steering; i++) {
        Steering_mixer_data.motor_speed[i] += m_trim.motor_speed[i];
    }
}

uint8_t SteeringMixer::get_num_of_steering_modes() { return NUM_STEERING_MODES; }
