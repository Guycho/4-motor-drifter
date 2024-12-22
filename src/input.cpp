#include "input.h"

void on_connect();
void on_disConnect();
void controller_do();
void remove_paired_devices();
float calc_throttle(uint8_t l2, uint8_t r2);
float calc_steering(int8_t lx);

float m_throttle;
float m_steering;
bool m_steering_mode_toggle;
bool m_throttle_mode_toggle;
bool m_coilover_mode_toggle;
bool m_arm_toggle;
bool m_roll_right;
bool m_roll_left;
bool m_pitch_forward;
bool m_pitch_backward;
bool m_ride_height_up;
bool m_ride_height_down;
bool m_roll_pitch_reset;
bool m_ride_height_reset;
bool m_new_data;

uint8_t m_dead_band;

void init_ps4(const InputControllerConfig config) {
    PS4.attach(controller_do);
    PS4.attachOnConnect(on_connect);
    PS4.attachOnDisconnect(on_disConnect);
    PS4.begin(config.mac);
    remove_paired_devices();  // This helps to solve connection issues
    m_dead_band = config.dead_band;
}

float get_throttle() { return m_throttle; }
float get_steering() { return m_steering; }

bool get_steering_mode_toggle() {
    bool temp = m_steering_mode_toggle;
    m_steering_mode_toggle = false;
    return temp;
}

bool get_throttle_mode_toggle() {
    bool temp = m_throttle_mode_toggle;
    m_throttle_mode_toggle = false;
    return temp;
}

bool get_coilover_mode_toggle() {
    bool temp = m_coilover_mode_toggle;
    m_coilover_mode_toggle = false;
    return temp;
}

bool get_arm_toggle() {
    bool temp = m_arm_toggle;
    m_arm_toggle = false;
    return temp;
}

void remove_paired_devices() {
    uint8_t pairedDeviceBtAddr[20][6];
    int count = esp_bt_gap_get_bond_device_num();
    esp_bt_gap_get_bond_device_list(&count, pairedDeviceBtAddr);
    for (int i = 0; i < count; i++) {
        esp_bt_gap_remove_bond_device(pairedDeviceBtAddr[i]);
    }
}

void controller_do() {
    boolean sqd = PS4.event.button_down.square, trd = PS4.event.button_down.triangle,
            crd = PS4.event.button_down.cross, cid = PS4.event.button_down.circle,
            upd = PS4.event.button_down.up, rid = PS4.event.button_down.right,
            dod = PS4.event.button_down.down, led = PS4.event.button_down.left,
            l1d = PS4.event.button_down.l1, r1d = PS4.event.button_down.r1,
            l3d = PS4.event.button_down.l3, r3d = PS4.event.button_down.r3;

    boolean sq = PS4.Square(), tr = PS4.Triangle(), cr = PS4.Cross(), ci = PS4.Circle(), 
            up = PS4.Up(), right = PS4.Right(), down = PS4.Down(), left = PS4.Left(), 
            l1 = PS4.L1(), r1 = PS4.R1();

    int8_t lx = PS4.LStickX(), ly = PS4.LStickY(), rx = PS4.RStickX(), ry = PS4.RStickY();

    uint8_t l2 = PS4.L2Value(), r2 = PS4.R2Value();

    int16_t gx = PS4.GyrX(), gy = PS4.GyrY(), gz = PS4.GyrZ(), ax = PS4.AccX(), ay = PS4.AccY(),
            az = PS4.AccZ();

    if (trd) {
        m_steering_mode_toggle = true;
    }
    if (cid) {
        m_throttle_mode_toggle = true;
    }
    if (crd) {
        m_arm_toggle = true;
    }
    if (sqd) {
        m_coilover_mode_toggle = true;
    }
    if (l3d) {
        m_roll_pitch_reset = true;
    }
    if (l3d) {
        m_roll_pitch_reset = true;
    }
    if (r3d) {
        m_ride_height_reset = true;
    }

    m_ride_height_up = l1;
    m_ride_height_down = r1;
    m_pitch_forward = up;
    m_roll_right = right;
    m_pitch_backward = down;
    m_roll_left = left;

    m_throttle = calc_throttle(l2, r2);
    m_steering = calc_steering(lx);

    m_new_data = true;
}

InputControllerData get_input_data(){
    InputControllerData data;
    data.throttle = m_throttle;
    data.steering = m_steering;
    data.steering_mode_toggle = m_steering_mode_toggle;
    data.throttle_mode_toggle = m_throttle_mode_toggle;
    data.coilover_mode_toggle = m_coilover_mode_toggle;
    data.arm_toggle = m_arm_toggle;
    data.roll_right = m_roll_right;
    data.roll_left = m_roll_left;
    data.pitch_forward = m_pitch_forward;
    data.pitch_backward = m_pitch_backward;
    data.ride_height_up = m_ride_height_up;
    data.ride_height_down = m_ride_height_down;
    data.roll_pitch_reset = m_roll_pitch_reset;
    data.ride_height_reset = m_ride_height_reset;
    data.new_data = m_new_data;
    m_throttle = 0;
    m_steering = 0;
    m_steering_mode_toggle = false;
    m_throttle_mode_toggle = false;
    m_coilover_mode_toggle = false;
    m_arm_toggle = false;
    m_roll_right = false;
    m_roll_left = false;
    m_pitch_forward = false;
    m_pitch_backward = false;
    m_ride_height_up = false;
    m_ride_height_down = false;
    m_roll_pitch_reset = false;
    m_ride_height_reset = false;
    m_new_data = false;
    return data;
}

float calc_throttle(uint8_t l2, uint8_t r2) {
    float temp = Utils::Calcs::map_float((r2 - l2), -255, 255, -100, 100);
    float scaled = Utils::Calcs::calc_dead_band(temp, 100, m_dead_band);
    return scaled;
}

float calc_steering(int8_t lx) {
    float temp = Utils::Calcs::map_float(lx, -127, 127, -100, 100);
    float scaled = Utils::Calcs::calc_dead_band(temp, 100, m_dead_band);
    return scaled;
}

void on_connect() {}

void on_disConnect() {}
