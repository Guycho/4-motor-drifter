#include <brushed_motor.h>

BrushedMotor::BrushedMotor() {}   // Constructor
BrushedMotor::~BrushedMotor() {}  // Destructor definition

void BrushedMotor::init(const BrushedMotorConfig &config) {
    m_servo_output.attach(config.pin, config.min_pulse,
      config.max_pulse);  // Attaching the motor to the pin and setting the min and max pulse
      uint16_t init_pulse = (config.min_pulse + config.max_pulse) / 2;
    m_servo_output.writeMicroseconds(init_pulse);  // Setting the motor speed to 0
}  // Method to start the motor

void BrushedMotor::setMicroSeconds(uint16_t micro_seconds) {
    m_servo_output.writeMicroseconds(micro_seconds);  // Setting the motor speed
}  // Method to set the motor speed

uint16_t BrushedMotor::getMicroSeconds() {
    return m_servo_output.readMicroseconds();  // Getting the motor speed
}  // Method to get the motor speed
