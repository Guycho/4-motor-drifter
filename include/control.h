#ifndef CONTROL_H
#define CONTROL_H
#include <Chrono.h>

#include "PID.h"
#include "config.h"
#include "input_controller.h"
#include "mav_bridge.h"
#include "nvm.h"
#include "steering_mixer.h"
#include "transceiver.h"
#include "utils.h"
#include "wheels_mixer.h"

struct ControlConfig {
    MavBridge *mav_bridge;
    SteeringMixer *steering_mixer;
    WheelsMixer *wheels_mixer;
    PID *pid;
    NVM *nvm;
    InputController *input_controller;
    Transceiver *transceiver;
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
    void apply_trim(InputControllerData &input_data);

   private:

    MavBridge *m_mav_bridge;
    SteeringMixer *m_steering_mixer;
    WheelsMixer *m_wheels_mixer;
    PID *m_pid;
    NVM *m_nvm;
    InputController *m_input_controller;
    Transceiver *m_transceiver;

    void set_telemetry_data();

    Chrono m_hb_timer;
    NVMData m_nvm_data;
    MavlinkData m_mavlink_data;
    InputControllerData m_input_data;
    SteeringMixerData m_steering_mixer_data;
    WheelsMixerData m_wheels_mixer_data;
    TelemetryData m_telemetry_data;

    uint8_t NUM_STEERING_MODES;
    uint8_t NUM_DRIVE_MODES;

    uint8_t m_steering_mode;
    uint8_t m_drive_mode;

    bool m_arm_enabled = false;
    float m_throttle = 0;
    float m_steering = 0;
    float m_throttle_trim = 0;
    float m_steering_trim = 0;
    bool m_lock_rear_right = false;
    bool m_lock_rear_left = false;
};

#endif  // CONTROL_H