#ifndef CONTROL_H
#define CONTROL_H
#include <Chrono.h>

#include "config.h"
#include "input.h"
#include "mav_bridge.h"
#include "steering_mixer.h"
#include "utils.h"
#include "PID.h"
#include "wheels_mixer.h"

struct ControlConfig {
    MavBridge *mav_bridge;
    SteeringMixer *steering_mixer;
    WheelsMixer *wheels_mixer;
    PID *pid;
};

struct ControlPrintData {
    float throttle;
    float steering;
    bool arm_enabled;
    uint8_t steering_mode;
    uint8_t drive_mode;
    WheelsMixerData wheels_mixer_data;
    SteeringMixerData steering_mixer_data;
    InertialData inertial_data;
};

class Control {
   public:
    // Constructor
    Control();

    // Destructor
    ~Control();

    void init(const ControlConfig &config);
    void run();
    void apply_multiplier(SteeringMixerData &steering_mixer_data);
    ControlPrintData get_print_data();

   private:
    Chrono m_hb_timer;

    MavBridge m_mav_bridge;
    SteeringMixer m_steering_mixer;
    WheelsMixer m_wheels_mixer;
    PID m_pid;
    ControlPrintData m_print_data;
    InertialData m_inertial_data;

    uint8_t NUM_STEERING_MODES;
    uint8_t NUM_DRIVE_MODES;

    uint8_t m_steering_mode;
    uint8_t m_drive_mode;

    bool m_arm_enabled = false;
    float m_throttle = 0;
    float m_steering = 0;
};

#endif  // CONTROL_H