#ifndef CONTROL_H
#define CONTROL_H

#include <Chrono.h>

#include "PID.h"
#include "config.h"
#include "input_controller.h"
#include "mav_bridge.h"
#include "steering_mixer.h"
#include "transceiver.h"
#include "utils.h"
#include "wheels_mixer.h"

struct ControlConfig {
    MavBridge *mav_bridge;
    SteeringMixer *steering_mixer;
    WheelsMixer *wheels_mixer;
    PID *pid;
    InputController *input_controller;
    Transceiver *transceiver;
    uint8_t arm_led_pin;
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

   private:
    void update_mavlink_data();
    void update_input_data();
    void handle_new_input_data();
    void handle_heartbeat_timeout();
    void handle_steering_mode();
    void handle_drive_mode();
    void handle_locks();
    void disable_motors();

    MavBridge *m_mav_bridge;
    SteeringMixer *m_steering_mixer;
    WheelsMixer *m_wheels_mixer;
    PID *m_pid;
    InputController *m_input_controller;
    Transceiver *m_transceiver;

    Chrono m_hb_timer;
    MavlinkData m_mavlink_data;
    InputControllerData m_input_data;
    SteeringMixerData m_steering_mixer_data;
    WheelsMixerData m_wheels_mixer_data;

    uint8_t m_arm_led_pin;

    uint8_t NUM_STEERING_MODES;
    uint8_t NUM_DRIVE_MODES;

    uint8_t m_steering_mode;
    uint8_t m_drive_mode;

    bool m_arm_enabled;
    float m_throttle;
    float m_steering;
    bool m_lock_rear_right;
    bool m_lock_rear_left;
};

#endif  // CONTROL_H