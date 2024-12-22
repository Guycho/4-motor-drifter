#include "control.h"

Control::Control() {}
Control::~Control() {}

void Control::init(const ControlConfig &config) {
    m_hb_timer.start();
    m_mav_bridge = *config.mav_bridge;
    m_arm_enabled = false;
    m_throttle = 0;
    m_steering = 0;
    m_steering_motors_logic = *config.steering_motors_logic;
    m_wheel_motors_logic = *config.wheel_motors_logic;
    m_coilover_adjusters_logic = *config.coilover_adjusters_logic;
    NUM_STEERING_MODES = m_steering_motors_logic.get_num_of_steering_modes();
    NUM_THROTTLE_MODES = m_wheel_motors_logic.get_num_of_throttle_modes();
    NUM_COILOVER_MODES = m_coilover_adjusters_logic.get_num_of_coilover_modes();
}

void Control::run() {
    m_mav_bridge.run();
    m_inertial_data = m_mav_bridge.get_inertial_data();
    InputControllerData input_data = get_input_data();
    if (input_data.new_data) {
        if (input_data.arm_toggle) {
            m_arm_enabled = !m_arm_enabled;
        }
        if (input_data.steering_mode_toggle) {
            m_steering_mode = (m_steering_mode + 1) % NUM_STEERING_MODES;
        }
        if (input_data.throttle_mode_toggle) {
            m_throttle_mode = (m_throttle_mode + 1) % NUM_THROTTLE_MODES;
        }
        if (input_data.coilover_mode_toggle) {
            m_coilover_mode = (m_coilover_mode + 1) % NUM_COILOVER_MODES;
        }
        m_steering = input_data.steering;
        m_throttle = input_data.throttle;
        if (input_data.roll_pitch_reset) {
            m_des_roll_pitch = {0, 0};
        }
        if (input_data.ride_height_reset) {
            m_ride_height = 0;
        }
        if (input_data.roll_right) {
            m_des_roll_pitch.roll += 0.5;
        } else if (input_data.roll_left) {
            m_des_roll_pitch.roll -= 0.5;
        }
        if (input_data.pitch_forward) {
            m_des_roll_pitch.pitch += 0.5;
        } else if (input_data.pitch_backward) {
            m_des_roll_pitch.pitch -= 0.5;
        }
        if (input_data.ride_height_up) {
            m_ride_height += 0.5;
        } else if (input_data.ride_height_down) {
            m_ride_height -= 0.5;
        }
        if (m_steering_mode == S_PIVOT) {
            m_throttle_mode = T_PIVOT;
        } else if (m_steering_mode != S_PIVOT && m_throttle_mode == T_PIVOT) {
            m_throttle_mode = AWD;
        }
        m_hb_timer.restart();
    }
    if (m_hb_timer.hasPassed(100, true)) {
        m_arm_enabled = false;
    }
    RollPitch roll_pitch = {m_inertial_data.orientation.x, m_inertial_data.orientation.y};

    m_steering_motors_logic.run(m_arm_enabled, m_steering, m_steering_mode);
    m_wheel_motors_logic.run(m_arm_enabled, m_throttle, m_throttle_mode);
    m_coilover_adjusters_logic.run(m_arm_enabled, m_coilover_mode, roll_pitch, m_des_roll_pitch,
      m_ride_height);
}
