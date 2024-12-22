#ifndef STEERING_MOTOR_H
#define STEERING_MOTOR_H

#include "P_controller.h"
#include "brushed_motor.h"
#include "rotational_encoder.h"
#include "utils.h"
struct SteeringMotorConfig {
    BrushedMotorConfig motor_config;
    RotationalEncoderConfig encoder_config;
    PControllerConfig controller_config;
    float max_angle;
    float max_speed_pct;
};

class SteeringMotor {
   public:
    SteeringMotor();   // Constructor
    ~SteeringMotor();  // Destructor

    void init(const SteeringMotorConfig& config);
    void setAngle(float angle);  // Method to set angle
    void stop();                 // Method to stop the motor
    void run();                  // Method to run the motor
    float getAngle();            // Method to get the angle
    float getDesAngle();         // Method to get the desired angle

private : BrushedMotor m_motor;   // Variable to store the motor
    RotationalEncoder m_encoder;  // Variable to store the encoder
    PController m_controller;     // Variable to store the controller

    void setSpeedPct(float speed_pct);

    float m_max_speed_pct;  // Variable to store the motor max speed
    float m_max_angle;
    float m_des_angle;
    uint16_t m_max_pulse;
    uint16_t m_min_pulse;
};

#endif  // STEERING_MOTOR_H