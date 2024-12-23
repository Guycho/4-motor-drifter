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
    bool new_data;
};
struct InputControllerConfig {
    const char* mac;
    float dead_band;
};

void init_ps4(const InputControllerConfig config);
InputControllerData get_input_data();

#endif  // INPUT_CONTROLLER_H