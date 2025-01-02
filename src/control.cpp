#include "control.h"

Control::Control() {}
Control::~Control() {}

void Control::init(const ControlConfig &config) {
    m_hb_timer.start();
    m_mav_bridge = *config.mav_bridge;
    m_steering_mixer = *config.steering_mixer;
    m_wheels_mixer = *config.wheels_mixer;
    m_pid = *config.pid;
    m_nvm = *config.nvm;
    m_nvm_data = m_nvm.get_data();
    m_arm_enabled = false;
    m_throttle = 0;
    m_steering = 0;
    NUM_STEERING_MODES = m_steering_mixer.get_num_of_steering_modes();
    NUM_DRIVE_MODES = m_wheels_mixer.get_num_of_drive_modes();
}

void Control::run() {
    m_mav_bridge.run();
    m_inertial_data = m_mav_bridge.get_inertial_data();
    InputControllerData input_data = get_input_data();
    if (input_data.new_data) {
        apply_trim(input_data);
        if (input_data.arm_toggle) {
            m_arm_enabled = !m_arm_enabled;
        }
        if (input_data.steering_mode_toggle) {
            m_steering_mode = (m_steering_mode + 1) % NUM_STEERING_MODES;
            m_pid.reset_pid();
        }
        if (input_data.drive_mode_toggle) {
            m_drive_mode = (m_drive_mode + 1) % NUM_DRIVE_MODES;
        }
        m_steering = input_data.steering + m_nvm_data.steering_trim;
        m_throttle = input_data.throttle + m_nvm_data.throttle_trim;
        m_lock_rear_right = input_data.lock_rear_right;
        m_lock_rear_left = input_data.lock_rear_left;
        m_hb_timer.restart();
    }
    SteeringMixerData steering_mixer_data;
    WheelsMixerData wheels_mixer_data;
    if (m_arm_enabled) {
        float desired_omega = 0.0f;
        float pid_output = 0.0f;
        float current_omega = 0.0f;
        switch (m_steering_mode) {
            case NORMAL:
                steering_mixer_data.motor_speed[R] = m_steering;
                steering_mixer_data.motor_speed[L] = m_steering;
                break;
            case GYRO:
                desired_omega = m_steering;
                current_omega = Utils::Calcs::constrain_float(
                  Utils::Calcs::map_float(m_inertial_data.gyro.z, -Config::max_omega,
                    Config::max_omega, Config::min_percentage, Config::max_percentage),
                  Config::min_percentage, Config::max_percentage);
                pid_output = m_pid.compute(desired_omega, current_omega);
                steering_mixer_data.motor_speed[R] = pid_output;
                steering_mixer_data.motor_speed[L] = pid_output;
                break;
            default:
                steering_mixer_data.motor_speed[R] = 0;
                steering_mixer_data.motor_speed[L] = 0;
                break;
        }
        uint8_t current_drive_mode = m_drive_mode;
        if (m_throttle < 0) {
            current_drive_mode = AWD;
        }
        switch (current_drive_mode) {
            case AWD:
                wheels_mixer_data.motor_speed[FR] = m_throttle;
                wheels_mixer_data.motor_speed[RR] = m_throttle;
                wheels_mixer_data.motor_speed[RL] = m_throttle;
                wheels_mixer_data.motor_speed[FL] = m_throttle;
                break;
            case CS:
                wheels_mixer_data.motor_speed[FR] = m_throttle * Config::cs_ratio;
                wheels_mixer_data.motor_speed[RR] = m_throttle;
                wheels_mixer_data.motor_speed[RL] = m_throttle;
                wheels_mixer_data.motor_speed[FL] = m_throttle * Config::cs_ratio;
                break;
            case RWD:
                wheels_mixer_data.motor_speed[FR] = 0;
                wheels_mixer_data.motor_speed[RR] = m_throttle;
                wheels_mixer_data.motor_speed[RL] = m_throttle;
                wheels_mixer_data.motor_speed[FL] = 0;
                break;
            default:
                wheels_mixer_data.motor_speed[FR] = 0;
                wheels_mixer_data.motor_speed[RR] = 0;
                wheels_mixer_data.motor_speed[RL] = 0;
                wheels_mixer_data.motor_speed[FL] = 0;
                break;
        }
        if (m_lock_rear_right) {
            wheels_mixer_data.motor_speed[RR] = Config::min_percentage;
        }
        if (m_lock_rear_left) {
            wheels_mixer_data.motor_speed[RL] = Config::min_percentage;
        }
    } else {
        steering_mixer_data.motor_speed[R] = 0;
        steering_mixer_data.motor_speed[L] = 0;
        wheels_mixer_data.motor_speed[FR] = 0;
        wheels_mixer_data.motor_speed[RR] = 0;
        wheels_mixer_data.motor_speed[RL] = 0;
        wheels_mixer_data.motor_speed[FL] = 0;
    }

    apply_multiplier(steering_mixer_data);
    m_steering_mixer.run(steering_mixer_data);
    m_wheels_mixer.run(wheels_mixer_data);
    m_print_data.throttle = m_throttle;
    m_print_data.steering = m_steering;
    m_print_data.arm_enabled = m_arm_enabled;
    m_print_data.steering_mode = m_steering_mode;
    m_print_data.drive_mode = m_drive_mode;
    m_print_data.wheels_mixer_data = wheels_mixer_data;
    m_print_data.steering_mixer_data = steering_mixer_data;
    m_print_data.inertial_data = m_inertial_data;
}

void Control::apply_multiplier(SteeringMixerData &steering_mixer_data) {
    float adjustment_value =
      (steering_mixer_data.motor_speed[R] + steering_mixer_data.motor_speed[L]) *
      Config::steering_r_l_ratio;
    if (steering_mixer_data.motor_speed[R] + steering_mixer_data.motor_speed[L] > 0) {
        steering_mixer_data.motor_speed[L] -= adjustment_value;
    } else {
        steering_mixer_data.motor_speed[R] -= adjustment_value;
    }
}

void Control::apply_trim(InputControllerData &input_data) {
    if (input_data.trim_r) {
        if (input_data.trim_direction_r) {
            m_nvm_data.steering_mixer_data.motor_speed[R] += Config::trim_increment;
        }
        if (input_data.trim_direction_l) {
            m_nvm_data.steering_mixer_data.motor_speed[R] -= Config::trim_increment;
        }
        if (input_data.reset_trim) {
            m_nvm_data.steering_mixer_data.motor_speed[R] = 0;
        }
    }
    if (input_data.trim_l) {
        if (input_data.trim_direction_r) {
            m_nvm_data.steering_mixer_data.motor_speed[L] += Config::trim_increment;
        }
        if (input_data.trim_direction_l) {
            m_nvm_data.steering_mixer_data.motor_speed[L] -= Config::trim_increment;
        }
        if (input_data.reset_trim) {
            m_nvm_data.steering_mixer_data.motor_speed[L] = 0;
        }
    }
    if (input_data.trim_throttle) {
        if (input_data.trim_direction_f) {
            m_nvm_data.throttle_trim += Config::trim_increment;
        }
        if (input_data.trim_direction_b) {
            m_nvm_data.throttle_trim -= Config::trim_increment;
        }
        if (input_data.reset_trim) {
            m_nvm_data.throttle_trim = 0;
        }
    }   
    if (input_data.trim_steering) {
        if (input_data.trim_direction_r) {
            m_nvm_data.steering_trim += Config::trim_increment;
        }
        if (input_data.trim_direction_l) {
            m_nvm_data.steering_trim -= Config::trim_increment;
        }
        if (input_data.reset_trim) {
            m_nvm_data.steering_trim = 0;
        }
    }
    m_steering_mixer.set_trim(m_nvm_data.steering_mixer_data);
    if (input_data.write_to_nvm) {
        m_nvm.set_data(m_nvm_data);
    }
}

ControlPrintData Control::get_print_data() {
    return m_print_data;
}
