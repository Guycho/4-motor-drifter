#include "control.h"

Control::Control() {}
Control::~Control() {}

void Control::init(const ControlConfig &config) {
    m_hb_timer.start();
    m_mav_bridge = *config.mav_bridge;
    m_steering_mixer = *config.steering_mixer;
    m_wheels_mixer = *config.wheels_mixer;
    m_arm_enabled = false;
    m_throttle = 0;
    m_steering = 0;
}

void Control::run() {
    m_mav_bridge.run();
    m_inertial_data = m_mav_bridge.get_inertial_data();
    InputControllerData input_data = get_input_data();
    if (input_data.new_data) {
        if (input_data.arm_toggle) {
            m_arm_enabled = !m_arm_enabled;
        }
        m_steering = input_data.steering;
        m_throttle = input_data.throttle;
        m_hb_timer.restart();
    }
    SteeringMixerData steering_mixer_data;
    WheelsMixerData wheels_mixer_data;
    if (m_arm_enabled) {
        steering_mixer_data.motor_speed[R] = m_steering;
        steering_mixer_data.motor_speed[L] = m_steering;
        wheels_mixer_data.motor_speed[FR] = m_throttle;
        wheels_mixer_data.motor_speed[RR] = m_throttle;
        wheels_mixer_data.motor_speed[RL] = m_throttle;
        wheels_mixer_data.motor_speed[FL] = m_throttle;
    } else {
        steering_mixer_data.motor_speed[R] = 0;
        steering_mixer_data.motor_speed[L] = 0;
        wheels_mixer_data.motor_speed[FR] = 0;
        wheels_mixer_data.motor_speed[RR] = 0;
        wheels_mixer_data.motor_speed[RL] = 0;
        wheels_mixer_data.motor_speed[FL] = 0;
    }
    m_steering_mixer.run(steering_mixer_data);
    m_wheels_mixer.run(wheels_mixer_data);
    m_print_data.throttle = m_throttle;
    m_print_data.steering = m_steering;
    m_print_data.arm_enabled = m_arm_enabled;
    m_print_data.wheels_mixer_data = wheels_mixer_data;
    m_print_data.steering_mixer_data = steering_mixer_data;
}

ControlPrintData Control::get_print_data() { return m_print_data; }
