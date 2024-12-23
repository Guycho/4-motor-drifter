#include <Chrono.h>

#include "config.h"
#include "control.h"
#include "input.h"
#include "mav_bridge.h"
#include "wheels_mixer.h"
#include "steering_mixer.h"


Chrono print_timer;
MavBridge mav_bridge;
SteeringMixer steering_mixer;
WheelsMixer wheels_mixer;
Control control;

void setup() {
    Serial.begin(9600);
    print_timer.start();

    InputControllerConfig input_config;
    input_config.mac = Config::PS4Controller::mac;
    input_config.dead_band = Config::PS4Controller::dead_band;
    init_ps4(input_config);

    MavBridgeConfig mav_config;
    mav_config.serial = Config::MavlinkBridge::serial;
    mav_config.baudrate = Config::MavlinkBridge::baudrate;
    mav_config.system_id = Config::MavlinkBridge::system_id;
    mav_config.component_id = Config::MavlinkBridge::component_id;
    mav_config.message_rate = Config::MavlinkBridge::message_rate;
    mav_config.is_alive_timeout = Config::MavlinkBridge::is_alive_timeout;
    mav_bridge.init(mav_config);

    SteeringMixerConfig steering_config;
    steering_config.mav_bridge = &mav_bridge;
    for (int i = 0; i < Config::num_steering; i++) {
        steering_config.pin[i] = Config::Motor::Steering::pin[i];
        steering_config.min_pulse[i] = Config::Motor::Steering::min_pulse[i];
        steering_config.max_pulse[i] = Config::Motor::Steering::max_pulse[i];
    }
    steering_mixer.init(steering_config);

    WheelsMixerConfig wheels_config;
    wheels_config.mav_bridge = &mav_bridge;
    for (int i = 0; i < Config::num_wheels; i++) {
        wheels_config.pin[i] = Config::Motor::Wheel::pin[i];
        wheels_config.min_pulse[i] = Config::Motor::Wheel::min_pulse[i];
        wheels_config.max_pulse[i] = Config::Motor::Wheel::max_pulse[i];
    }
    wheels_mixer.init(wheels_config);

    ControlConfig control_config;
    control_config.mav_bridge = &mav_bridge;
    control_config.steering_mixer = &steering_mixer;
    control_config.wheels_mixer = &wheels_mixer;
    control.init(control_config);
}

void loop() {
    control.run();
    if (print_timer.hasPassed(500, true)) {
        ControlPrintData print_data = control.get_print_data();
        Serial.print("Throttle: ");
        Serial.print(print_data.throttle);
        Serial.print(" Steering: ");
        Serial.print(print_data.steering);
        Serial.print(" Arm enabled: ");
        Serial.print(print_data.arm_enabled);
        Serial.print(" Wheels: ");
        for (int i = 0; i < Config::num_wheels; i++) {
            Serial.print(print_data.wheels_mixer_data.motor_speed[i]);
            Serial.print(" ");
        }
        Serial.print(" Steering: ");
        for (int i = 0; i < Config::num_steering; i++) {
            Serial.print(print_data.steering_mixer_data.motor_speed[i]);
            Serial.print(" ");
        }
        Serial.println();
    }
}