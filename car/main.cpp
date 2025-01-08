#include <Chrono.h>

#include "ESP_now_handler.h"
#include "PID.h"
#include "config.h"
#include "control.h"
#include "input.h"
#include "mav_bridge.h"
#include "nvm.h"
#include "steering_mixer.h"
#include "transceiver.h"
#include "wheels_mixer.h"

Chrono print_timer;
MavBridge mav_bridge;
SteeringMixer steering_mixer;
WheelsMixer wheels_mixer;
PID pid;
NVM nvm;
Control control;
Transceiver transceiver;
ESPNowHandler esp_now_handler(Config::Car::ESPNow::peer_mac_address);

void setup() {
    Serial.begin(9600);
    print_timer.start();

    InputControllerConfig input_config;
    input_config.mac = Config::Car::PS4Controller::mac;
    input_config.dead_band = Config::Car::PS4Controller::dead_band;
    init_ps4(input_config);

    MavBridgeConfig mav_config;
    mav_config.serial = Config::Car::MavlinkBridge::serial;
    mav_config.baudrate = Config::Car::MavlinkBridge::baudrate;
    mav_config.system_id = Config::Car::MavlinkBridge::system_id;
    mav_config.component_id = Config::Car::MavlinkBridge::component_id;
    mav_config.message_rate = Config::Car::MavlinkBridge::message_rate;
    mav_config.is_alive_timeout = Config::Car::MavlinkBridge::is_alive_timeout;
    mav_bridge.init(mav_config);

    SteeringMixerConfig steering_config;
    steering_config.mav_bridge = &mav_bridge;
    for (int i = 0; i < Config::Car::num_steering; i++) {
        steering_config.pin[i] = Config::Car::Motor::Steering::pin[i];
        steering_config.min_pulse[i] = Config::Car::Motor::Steering::min_pulse[i];
        steering_config.max_pulse[i] = Config::Car::Motor::Steering::max_pulse[i];
    }
    steering_mixer.init(steering_config);

    WheelsMixerConfig wheels_config;
    wheels_config.mav_bridge = &mav_bridge;
    for (int i = 0; i < Config::Car::num_wheels; i++) {
        wheels_config.pin[i] = Config::Car::Motor::Wheel::pin[i];
        wheels_config.min_pulse[i] = Config::Car::Motor::Wheel::min_pulse[i];
        wheels_config.max_pulse[i] = Config::Car::Motor::Wheel::max_pulse[i];
    }
    wheels_mixer.init(wheels_config);

    PIDConfig pid_config;
    pid_config.kp = Config::Car::PIDController::kp;
    pid_config.ki = Config::Car::PIDController::ki;
    pid_config.kd = Config::Car::PIDController::kd;
    pid_config.max_output = Config::Car::PIDController::max_output;
    pid_config.integral_percentage = Config::Car::PIDController::integral_percentage;
    pid_config.low_pass_alpha = Config::Car::PIDController::low_pass_alpha;
    pid_config.high_Pass_alpha = Config::Car::PIDController::high_Pass_alpha;
    pid_config.use_filters = Config::Car::PIDController::use_filters;
    pid.init(pid_config);

    nvm.init();

    ControlConfig control_config;
    control_config.mav_bridge = &mav_bridge;
    control_config.steering_mixer = &steering_mixer;
    control_config.wheels_mixer = &wheels_mixer;
    control_config.pid = &pid;
    control_config.nvm = &nvm;
    control.init(control_config);

    esp_now_handler.init();

    TransceiverConfig transceiver_config;
    transceiver_config.update_delay_ms = Config::Car::Transceiver::update_delay_ms;
    transceiver_config.control = &control;
    transceiver_config.esp_now_handler = &esp_now_handler;
    transceiver.init(transceiver_config);
}

void loop() {
    control.run();
    transceiver.update_data();
    if (print_timer.hasPassed(2500, true)) {
        ControlPrintData print_data = control.get_print_data();
        Serial.print("Throttle: ");
        Serial.print(print_data.throttle);
        Serial.print(" Steering: ");
        Serial.print(print_data.steering);
        Serial.print(" Arm enabled: ");
        Serial.print(print_data.arm_enabled);
        Serial.print(" Steering mode: ");
        Serial.print(print_data.steering_mode);
        Serial.print(" Drive mode: ");
        Serial.print(print_data.drive_mode);
        Serial.print(" Battery voltage: ");
        Serial.print(print_data.mavlink_data.battery_voltage);
        Serial.print(" Wheels mixer data: ");
        for (int i = 0; i < Config::Car::num_wheels; i++) {
            Serial.print(print_data.wheels_mixer_data.motor_speed[i]);
            Serial.print(" ");
        }
        Serial.print(" Steering mixer data: ");
        for (int i = 0; i < Config::Car::num_steering; i++) {
            Serial.print(print_data.steering_mixer_data.motor_speed[i]);
            Serial.print(" ");
        }
        Serial.println();
    }
}