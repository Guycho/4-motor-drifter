#include "wheels_mixer.h"

WheelsMixer::WheelsMixer() {}
WheelsMixer::~WheelsMixer() {}

void WheelsMixer::init(const WheelsMixerConfig &config) {
    m_mav_bridge = *config.mav_bridge;
    for (int i = 0; i < Config::num_wheels; i++) {
        m_pin[i] = config.pin[i];
        m_min_pulse[i] = config.min_pulse[i];
        m_max_pulse[i] = config.max_pulse[i];
    }
}

void WheelsMixer::run(WheelsMixerData &wheels_mixer_data) {
    for (int i = 0; i < Config::num_wheels; i++) {
        MotorSpeed motor_speed;
        motor_speed.motor_pin = m_pin[i];
        float temp_value = Utils::Calcs::map_float(wheels_mixer_data.motor_speed[i],
          Config::min_percentage, Config::max_percentage, m_min_pulse[i], m_max_pulse[i]);
        motor_speed.motor_value =
          Utils::Calcs::constrain_float(temp_value, m_min_pulse[i], m_max_pulse[i]);
        m_mav_bridge.set_motor_speed(motor_speed);
    }
}

uint8_t WheelsMixer::get_num_of_drive_modes() { return NUM_DRIVE_MODES; }
