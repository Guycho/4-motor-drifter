#include "input_controller.h"

InputController::InputController() {}

// Destructor
InputController::~InputController() {}

void InputController::init(const InputControllerConfig &config) {
    m_transceiver = config.transceiver;
}
InputControllerData InputController::get_input_data() {

    String remote_data = m_transceiver->get_remote_data();
    JsonDocument m_json_data;
    deserializeJson(m_json_data, remote_data);
    InputControllerData input_controller_data;
    input_controller_data.throttle = m_json_data["t"];
    input_controller_data.steering = m_json_data["s"];
    input_controller_data.arm_toggle = m_json_data["a"];
    input_controller_data.steering_mode_toggle = m_json_data["sm"];
    input_controller_data.drive_mode_toggle = m_json_data["dm"];
    input_controller_data.lock_rear_right = m_json_data["rr"];
    input_controller_data.lock_rear_left = m_json_data["rl"];
    input_controller_data.write_to_nvm = m_json_data["wn"];
    input_controller_data.trim_r = m_json_data["tr"];
    input_controller_data.trim_l = m_json_data["tl"];
    input_controller_data.trim_throttle = m_json_data["tt"];
    input_controller_data.trim_steering = m_json_data["ts"];
    input_controller_data.trim_direction_r = m_json_data["tdr"];
    input_controller_data.trim_direction_l = m_json_data["tdl"];
    input_controller_data.trim_direction_f = m_json_data["tdf"];
    input_controller_data.trim_direction_b = m_json_data["tdb"];
    input_controller_data.reset_trim = m_json_data["rt"];
    input_controller_data.new_data = m_json_data["nd"];
    return input_controller_data;
}