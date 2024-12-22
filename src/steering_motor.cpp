#include "steering_motor.h"
SteeringMotor::SteeringMotor() {}   // Constructor
SteeringMotor::~SteeringMotor() {}  // Destructor

void SteeringMotor::init(const SteeringMotorConfig& config) {
    m_motor.init(config.motor_config);
    m_encoder.init(config.encoder_config);
    m_controller.init(config.controller_config);
    m_max_speed_pct = config.max_speed_pct;
    m_max_angle = config.max_angle;
    m_max_pulse = config.motor_config.max_pulse;
    m_min_pulse = config.motor_config.min_pulse;
}  // Method to start the motor

void SteeringMotor::setAngle(float angle) {
    m_des_angle = Utils::Calcs::constrain_float(angle, -m_max_angle, m_max_angle);
}  // Method to set angle
void SteeringMotor::stop() { setAngle(m_encoder.getAngle()); }  // Method to stop the motor
void SteeringMotor::run() {
    float error = m_des_angle - m_encoder.getAngle();
    float output = m_controller.calculateOutput(error);
    setSpeedPct(output);
}  // Method to run the motor

void SteeringMotor::setSpeedPct(float speed_pct) {
    float m_speed_pct = Utils::Calcs::constrain_float(speed_pct, -m_max_speed_pct, m_max_speed_pct);
    uint16_t pulse = Utils::Calcs::map_float(m_speed_pct, -m_max_speed_pct, m_max_speed_pct,
      m_min_pulse, m_max_pulse);
    m_motor.setMicroSeconds(pulse);
}  // Method to set the motor speed percentage

float SteeringMotor::getAngle() { return m_encoder.getAngle(); }  // Method to get the angle

float SteeringMotor::getDesAngle() { return m_des_angle; }  // Method to get the desired angle