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
    bool steering_mode_toggle;
    bool throttle_mode_toggle;
    bool coilover_mode_toggle;
    bool arm_toggle;
    bool roll_right;
    bool roll_left;
    bool pitch_forward;
    bool pitch_backward;
    bool ride_height_up;
    bool ride_height_down;
    bool roll_pitch_reset;
    bool ride_height_reset;
    bool new_data;
};
struct InputControllerConfig {
    const char* mac;
    float dead_band;
};

void init_ps4(const InputControllerConfig config);
InputControllerData get_input_data();


#endif  // INPUT_CONTROLLER_H