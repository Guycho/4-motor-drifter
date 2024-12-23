#ifndef CONTROL_H
#define CONTROL_H
#include <Chrono.h>

#include "config.h"
#include "input.h"
#include "mav_bridge.h"
#include "wheels_mixer.h"
#include "steering_mixer.h"
#include "utils.h"

struct ControlConfig {
    MavBridge *mav_bridge;
    SteeringMixer *steering_mixer;
    WheelsMixer *wheels_mixer;
};

struct ControlPrintData {
    float throttle;
    float steering;
    bool arm_enabled;
    WheelsMixerData wheels_mixer_data;
    SteeringMixerData steering_mixer_data;
};

class Control {
   public:
    // Constructor
    Control();

    // Destructor
    ~Control();

    void init(const ControlConfig &config);
    void run();
    ControlPrintData get_print_data();

   private:
    Chrono m_hb_timer;

    MavBridge m_mav_bridge;
    SteeringMixer m_steering_mixer;
    WheelsMixer m_wheels_mixer;
    ControlPrintData m_print_data;

    InertialData m_inertial_data;

    bool m_arm_enabled = false;
    float m_throttle = 0;
    float m_steering = 0;
};

#endif  // CONTROL_H