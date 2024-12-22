#ifndef WHEEL_MOTORS_LOGIC_H
#define WHEEL_MOTORS_LOGIC_H

#include "config.h"
#include "utils.h"
#include "wheel_motor.h"

struct WheelMotorsLogicConfig {
    WheelMotor* wheel_motors[Config::num_wheels];
};

enum ThrottleMode { AWD = 0, FWD = 1, RWD = 2, T_PIVOT = 3 };

class WheelMotorsLogic {
   public:
    WheelMotorsLogic();   // Constructor
    ~WheelMotorsLogic();  // Destructor

    void init(const WheelMotorsLogicConfig& config);  // Method to start the motor
    void run(bool arm_enabled, float throttle, uint8_t throttle_mode);
    uint8_t get_num_of_throttle_modes();

   private:
    WheelMotor m_wheel_motors[Config::num_wheels];

    static constexpr ThrottleMode throttle_modes[] = {FWD, RWD, AWD, T_PIVOT};
    static constexpr uint8_t NUM_THROTTLE_MODES =
      sizeof(throttle_modes) / sizeof(throttle_modes[0]);
};

#endif  // WHEEL_MOTORS_LOGIC_H