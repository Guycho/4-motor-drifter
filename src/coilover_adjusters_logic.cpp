#include "coilover_adjusters_logic.h"

CoiloverAdjustersLogic::CoiloverAdjustersLogic() {}   // Constructor
CoiloverAdjustersLogic::~CoiloverAdjustersLogic() {}  // Destructor

void CoiloverAdjustersLogic::init(const CoiloverAdjustersLogicConfig& config) {
    for (int i = 0; i < Config::num_wheels; i++) {
        m_coilover_adjusters[i] = *config.CoiloverAdjusters[i];
        m_ground_sensors[i] = *config.GroundSensors[i];
        m_p_controllers[i].init(config.PControllers[i]);
        m_shock_min_length[i] = m_ground_sensors[i].get_min_distance();
        m_shock_max_length[i] = m_ground_sensors[i].get_max_distance();
    }
}  // Method to start the motor
void CoiloverAdjustersLogic::run(bool arm_enabled, uint8_t coilover_mode, RollPitch roll_pitch,
  RollPitch des_roll_pitch, float ride_height) {
    if (arm_enabled) {
        for (int i = 0; i < Config::num_wheels; i++) {
            m_shock_lengths[i] = m_ground_sensors[i].get_distance();
            if (m_shock_lengths[i] < m_shock_max_length[i] - 5) {
                m_ground_contact[i] = true;
            } else if (m_shock_lengths[i] == m_shock_max_length[i]) {
                m_ground_contact[i] = false;
            }
            m_coilover_adjusters_speed[i] = 0;
        }

        RollPitch m_roll_pitch = Utils::Calcs::rotateRollPitch45Degrees(roll_pitch);
        RollPitch m_des_roll_pitch = Utils::Calcs::rotateRollPitch45Degrees(des_roll_pitch);
        switch (coilover_mode) {
            case OFF:
                m_coilover_adjusters[FR].reset();
                m_coilover_adjusters[RR].reset();
                m_coilover_adjusters[RL].reset();
                m_coilover_adjusters[FL].reset();
                break;
            case STABILIZE:
                break;
            case KEEP_CONTACT:
                keep_contact();
                run_coilover_adjusters();
                  break;
            case RIDE_HEIGHT:
                bool contact = m_ground_contact[FR] && m_ground_contact[RR] &&
                               m_ground_contact[RL] && m_ground_contact[FL];
                if (contact) {
                    for (int i = 0; i < Config::num_wheels; i++) {
                        m_coilover_adjusters[i].set_speed(10);
                    }
                } else {
                    keep_contact();
                }
                break;
        }
    } else {
        m_coilover_adjusters[FR].reset();
        m_coilover_adjusters[RR].reset();
        m_coilover_adjusters[RL].reset();
        m_coilover_adjusters[FL].reset();
    }
}
uint8_t CoiloverAdjustersLogic::get_num_of_coilover_modes() { return NUM_COILOVER_MODES; }

void CoiloverAdjustersLogic::keep_contact() {
    for (int i = 0; i < Config::num_wheels; i++) {
        if (!m_ground_contact[i]) {
            m_coilover_adjusters_speed[i] = +10;
        }
    }
}

void CoiloverAdjustersLogic::run_coilover_adjusters() {
    for (int i = 0; i < Config::num_wheels; i++) {
        m_coilover_adjusters[i].set_speed(m_coilover_adjusters_speed[i]);
        m_coilover_adjusters[i].run();
    }
}
