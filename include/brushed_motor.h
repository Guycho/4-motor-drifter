#ifndef BRUSHED_MOTOR_H
#define BRUSHED_MOTOR_H

#include <ESP32Servo.h>

struct BrushedMotorConfig {
    uint8_t pin;
    uint16_t min_pulse;
    uint16_t max_pulse;
};

class BrushedMotor {
   public:
    BrushedMotor();   // Constructor
    ~BrushedMotor();  // Destructor

    void init(const BrushedMotorConfig &config);   // Method to start the motor
    void setMicroSeconds(uint16_t micro_seconds);  // Method to set the motor speed
    uint16_t getMicroSeconds();                    // Method to get the motor speed

   private:
    Servo m_servo_output;  // Variable to store the motor
    };

#endif  // BRUSHED_MOTOR_H