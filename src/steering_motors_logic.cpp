#include "steering_motors_logic.h"

SteeringMotorsLogic::SteeringMotorsLogic() {}   // Constructor
SteeringMotorsLogic::~SteeringMotorsLogic() {}  // Destructor

void SteeringMotorsLogic::init(const SteeringMotorsLogicConfig& config) {
    for (int i = 0; i < Config::num_wheels; i++) {
        m_steering_motors[i] = *config.steering_motors[i];
    }
    m_pivot_steering_angle = config.pivot_steering_angle;
}  // Method to start the motor
void SteeringMotorsLogic::run(bool arm_enabled, float steering,
  uint8_t steering_mode) {
    if (arm_enabled) {
        switch (steering_mode) {
            case FRONT:
                m_steering_motors[FR].setAngle(steering);
                m_steering_motors[FL].setAngle(steering);
                m_steering_motors[RR].setAngle(0);
                m_steering_motors[RL].setAngle(0);
                break;
            case REAR:
                m_steering_motors[FR].setAngle(0);
                m_steering_motors[FL].setAngle(0);
                m_steering_motors[RR].setAngle(-steering);
                m_steering_motors[RL].setAngle(-steering);
                break;
            case ALL:
                m_steering_motors[FR].setAngle(steering);
                m_steering_motors[FL].setAngle(steering);
                m_steering_motors[RR].setAngle(-steering);
                m_steering_motors[RL].setAngle(-steering);
                break;
            case CRAB:
                m_steering_motors[FR].setAngle(steering);
                m_steering_motors[FL].setAngle(steering);
                m_steering_motors[RR].setAngle(steering);
                m_steering_motors[RL].setAngle(steering);
                break;
            case S_PIVOT:
                m_steering_motors[FR].setAngle(-m_pivot_steering_angle);
                m_steering_motors[FL].setAngle(m_pivot_steering_angle);
                m_steering_motors[RR].setAngle(m_pivot_steering_angle);
                m_steering_motors[RL].setAngle(-m_pivot_steering_angle);
                break;
        }
    } else {
        m_steering_motors[FR].stop();
        m_steering_motors[FL].stop();
        m_steering_motors[RR].stop();
        m_steering_motors[RL].stop();
    }
    m_steering_motors[FR].run();
    m_steering_motors[FL].run();
    m_steering_motors[RR].run();
    m_steering_motors[RL].run();
}
uint8_t SteeringMotorsLogic::get_num_of_steering_modes() { return NUM_STEERING_MODES; }
