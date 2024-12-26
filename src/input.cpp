#include "input.h"

void on_connect();
void on_disConnect();
void controller_do();
void remove_paired_devices();
float calc_throttle(uint8_t l2, uint8_t r2);
float calc_steering(int8_t lx);

InputControllerData m_input_controller_data;

uint8_t m_dead_band;

void init_ps4(const InputControllerConfig config) {
    PS4.attach(controller_do);
    PS4.attachOnConnect(on_connect);
    PS4.attachOnDisconnect(on_disConnect);
    PS4.begin(config.mac);
    remove_paired_devices();  // This helps to solve connection issues
    m_dead_band = config.dead_band;
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

    m_input_controller_data.lock_rear_right = r1d;
    m_input_controller_data.lock_rear_left = l1d;

    if (crd) {
        m_input_controller_data.arm_toggle = true;
    }
    if (sqd) {
        m_input_controller_data.steering_mode_toggle = true;
    }
    if (cid) {
        m_input_controller_data.drive_mode_toggle = true;
    }

    m_input_controller_data.throttle = calc_throttle(l2, r2);
    m_input_controller_data.steering = calc_steering(lx);

    m_input_controller_data.new_data = true;
}

InputControllerData get_input_data(){
    m_input_controller_data.throttle = 0;
    m_input_controller_data.steering = 0;
    m_input_controller_data.arm_toggle = false;
    m_input_controller_data.steering_mode_toggle = false;
    m_input_controller_data.drive_mode_toggle = false;
    m_input_controller_data.lock_rear_right = false;
    m_input_controller_data.lock_rear_left = false;
    m_input_controller_data.new_data = false;
    return m_input_controller_data;
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