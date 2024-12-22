#ifndef CONTROL_H
#define CONTROL_H
#include <Chrono.h>

#include "coilover_adjusters_logic.h"
#include "config.h"
#include "input.h"
#include "mav_bridge.h"
#include "steering_motors_logic.h"
#include "wheel_motors_logic.h"

using FourValues = Utils::Structs::FourValues;

struct ControlConfig {
    SteeringMotorsLogic *steering_motors_logic;
    WheelMotorsLogic *wheel_motors_logic;
    CoiloverAdjustersLogic *coilover_adjusters_logic;
    MavBridge *mav_bridge;
};

class Control {
   public:
    // Constructor
    Control();

    // Destructor
    ~Control();

    void init(const ControlConfig &config);
    void run();

   private:
    Chrono m_hb_timer;

    SteeringMotorsLogic m_steering_motors_logic;
    WheelMotorsLogic m_wheel_motors_logic;
    CoiloverAdjustersLogic m_coilover_adjusters_logic;
    MavBridge m_mav_bridge;

    InertialData m_inertial_data;
    RollPitch m_des_roll_pitch;

    uint8_t NUM_THROTTLE_MODES;
    uint8_t NUM_STEERING_MODES;
    uint8_t NUM_COILOVER_MODES;

    uint8_t m_steering_mode;
    uint8_t m_throttle_mode;
    uint8_t m_coilover_mode;
    bool m_arm_enabled = false;
    float m_throttle = 0;
    float m_steering = 0;
    float m_ride_height = 0;
    float m_pivot_steering_angle = 0;
};

#endif  // CONTROL_H