#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include <PS4Controller.h>

#include "esp_bt_device.h"
#include "esp_bt_main.h"
#include "esp_err.h"
#include "esp_gap_bt_api.h"
#include "utils.h"

struct InputControllerData {
    float throttle;
    float steering;
    bool arm_toggle;
    bool steering_mode_toggle;
    bool drive_mode_toggle;
    bool lock_rear_right;
    bool lock_rear_left;
    bool write_to_nvm;
    bool trim_r;
    bool trim_l;
    bool trim_throttle;
    bool trim_steering;
    bool trim_direction_r;
    bool trim_direction_l;
    bool trim_direction_f;
    bool trim_direction_b;
    bool reset_trim;
    bool new_data;
};

struct InputControllerConfig {
    const char* mac;
    float dead_band;
};

void init_ps4(const InputControllerConfig config);
InputControllerData get_input_data();

#endif  // INPUT_CONTROLLER_H