#include "ESP_now_handler.h"
#include "PID.h"
#include "config.h"
#include "control.h"
#include "input_controller.h"
#include "mav_bridge.h"
#include "steering_mixer.h"
#include "transceiver.h"
#include "wheels_mixer.h"

ESPNowHandler esp_now_handler(Config::ESPNow::peer_mac_address);
Transceiver transceiver;
InputController input_controller;
MavBridge mav_bridge;
SteeringMixer steering_mixer;
WheelsMixer wheels_mixer;
PID pid;
Control control;

void setup() {
    Serial.begin(9600);

    TransceiverConfig transceiver_config;
    transceiver_config.update_delay_ms = Config::Transceiver::update_delay_ms;
    transceiver_config.esp_now_handler = &esp_now_handler;
    transceiver.init(transceiver_config);

    InputControllerConfig input_controller_config;
    input_controller_config.transceiver = &transceiver;
    input_controller.init(input_controller_config);

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

    PIDConfig pid_config;
    pid_config.kp = Config::PIDController::kp;
    pid_config.ki = Config::PIDController::ki;
    pid_config.kd = Config::PIDController::kd;
    pid_config.max_output = Config::PIDController::max_output;
    pid_config.integral_percentage = Config::PIDController::integral_percentage;
    pid_config.low_pass_alpha = Config::PIDController::low_pass_alpha;
    pid_config.high_Pass_alpha = Config::PIDController::high_Pass_alpha;
    pid_config.use_filters = Config::PIDController::use_filters;
    pid.init(pid_config);


    ControlConfig control_config;
    control_config.mav_bridge = &mav_bridge;
    control_config.steering_mixer = &steering_mixer;
    control_config.wheels_mixer = &wheels_mixer;
    control_config.pid = &pid;
    control_config.input_controller = &input_controller;
    control_config.transceiver = &transceiver;
    control.init(control_config);

    esp_now_handler.init();

}

void loop() {
    mav_bridge.run();
    transceiver.update_data();
    input_controller.get_input_data();
    control.run();
}