#ifndef STEERING_MOTORS_LOGIC_H
#define STEERING_MOTORS_LOGIC_H

#include "steering_motor.h"
#include "config.h"
#include "utils.h"

struct SteeringMotorsLogicConfig {
    SteeringMotor* steering_motors[Config::num_wheels];
    float pivot_steering_angle;
};

enum SteeringMode { ALL = 0, FRONT = 1, REAR = 2, CRAB = 3, S_PIVOT = 4 };

class SteeringMotorsLogic {
   public:
    SteeringMotorsLogic();  // Constructor
    ~SteeringMotorsLogic();  // Destructor

    void init(const SteeringMotorsLogicConfig& config);  // Method to start the motor
    void run(bool arm_enabled, float steering, uint8_t steering_mode);
    uint8_t get_num_of_steering_modes();

   private:
    SteeringMotor m_steering_motors[Config::num_wheels];

    static constexpr SteeringMode steering_modes[] = {FRONT, REAR, ALL, CRAB, S_PIVOT};
    static constexpr uint8_t NUM_STEERING_MODES =
      sizeof(steering_modes) / sizeof(steering_modes[0]);
    float m_pivot_steering_angle;
};

#endif  // STEERING_MOTORS_LOGIC_H