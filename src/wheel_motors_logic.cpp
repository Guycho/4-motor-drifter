#include "wheel_motors_logic.h"

WheelMotorsLogic::WheelMotorsLogic() {}    // Constructor
WheelMotorsLogic ::~WheelMotorsLogic() {}  // Destructor

void WheelMotorsLogic::init(const WheelMotorsLogicConfig& config) {
    for (int i = 0; i < Config::num_wheels; i++) {
        m_wheel_motors[i] = *config.wheel_motors[i];
    }
}  // Method to start the logic
void WheelMotorsLogic::run(bool arm_enabled, float throttle,
  uint8_t throttle_mode) {
    if (arm_enabled) {
        switch (throttle_mode) {
            case FWD:
                m_wheel_motors[FR].setSpeedPct(throttle);
                m_wheel_motors[FL].setSpeedPct(throttle);
                m_wheel_motors[RR].setSpeedPct(0);
                m_wheel_motors[RL].setSpeedPct(0);
                break;
            case RWD:
                m_wheel_motors[FR].setSpeedPct(0);
                m_wheel_motors[FL].setSpeedPct(0);
                m_wheel_motors[RR].setSpeedPct(throttle);
                m_wheel_motors[RL].setSpeedPct(throttle);
                break;
            case AWD:
                m_wheel_motors[FR].setSpeedPct(throttle);
                m_wheel_motors[FL].setSpeedPct(throttle);
                m_wheel_motors[RR].setSpeedPct(throttle);
                m_wheel_motors[RL].setSpeedPct(throttle);
                break;
            case T_PIVOT:
                m_wheel_motors[FR].setSpeedPct(throttle);
                m_wheel_motors[FL].setSpeedPct(-throttle);
                m_wheel_motors[RR].setSpeedPct(throttle);
                m_wheel_motors[RL].setSpeedPct(-throttle);
                break;
        }
    }
}
uint8_t WheelMotorsLogic::get_num_of_throttle_modes() {
    return NUM_THROTTLE_MODES;
}
