#ifndef WHEEL_MOTOR_H
#define WHEEL_MOTOR_H

#include "brushed_motor.h"
#include "utils.h"

struct WheelMotorConfig {
    BrushedMotorConfig motor_config;
    float max_speed_pct;
};

class WheelMotor {
   public:
    WheelMotor();   // Constructor
    ~WheelMotor();  // Destructor

    void init(const WheelMotorConfig& config);  // Method to start the motor
    void setSpeedPct(float speed_pct);            // Method to set the motor speed percentage
    void stop();
    float getSpeedPct();  // Method to get the motor speed percentage

   private:
    BrushedMotor m_motor;  // Variable to store the motor

    float m_max_speed_pct;  // Variable to store the motor max speed
    float m_speed_pct;      // Variable to store the motor speed percentage
    uint16_t m_max_pulse;
    uint16_t m_min_pulse;
    
};

#endif  // WHEEL_MOTOR_H