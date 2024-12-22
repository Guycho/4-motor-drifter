#include "wheel_motor.h"

    WheelMotor::WheelMotor(){}  // Constructor
    WheelMotor::~WheelMotor(){}  // Destructor

    void WheelMotor::init(const WheelMotorConfig& config){
        m_motor.init(config.motor_config);
        m_max_speed_pct = config.max_speed_pct;
        m_max_pulse = config.motor_config.max_pulse;
        m_min_pulse = config.motor_config.min_pulse;
    }  // Method to start the motor

    void WheelMotor::setSpeedPct(float speed_pct){
        m_speed_pct = Utils::Calcs::constrain_float(speed_pct, -m_max_speed_pct, m_max_speed_pct);
        uint16_t pulse = Utils::Calcs::map_float(m_speed_pct, -m_max_speed_pct, m_max_speed_pct, m_min_pulse, m_max_pulse); 
        m_motor.setMicroSeconds(pulse);
    }  // Method to set the motor speed percentage
    void WheelMotor::stop(){
        setSpeedPct(0);
    }
    float WheelMotor::getSpeedPct(){
        return m_speed_pct;
        // return Utils::Calcs::map_float(m_motor.getMicroSeconds(), m_min_pulse, m_max_pulse, -m_max_speed_pct, m_max_speed_pct);
    }  // Method to get the motor speed percentage
