#include <coilover_adjuster.h>

CoiloverAdjuster::CoiloverAdjuster() {}   // Constructor
CoiloverAdjuster::~CoiloverAdjuster() {}  // Destructor definition

void CoiloverAdjuster::init(const CoiloverAdjusterConfig &config) {
    m_min_pulse = config.min_pulse;
    m_max_pulse = config.max_pulse;
    m_mid_pulse = (config.min_pulse + config.max_pulse) / 2;
    m_servo_output.attach(config.pin, m_min_pulse,
      m_max_pulse);  // Attaching the motor to the pin and setting the min and max pulse
    m_servo_output.writeMicroseconds(m_mid_pulse);  // Setting the coilover position to 0
    m_reverse = config.reverse;
    m_min_speed = config.min_speed;
    m_max_speed = config.max_speed;
    m_current_pos = 0;
    m_speed = 0;
    m_timer.start();
}  // Method to start the coilover adjuster

void CoiloverAdjuster::set_pos(float pos) {
    uint16_t micro_seconds = Utils::Calcs::map_float(pos, -100, 100, m_min_pulse, m_max_pulse);
    if (m_reverse) {
        micro_seconds = m_max_pulse - micro_seconds + m_min_pulse;
    }
    micro_seconds = Utils::Calcs::constrain_float(micro_seconds, m_min_pulse, m_max_pulse);
    m_servo_output.writeMicroseconds(micro_seconds);  // Setting the coilover position
    m_current_pos = pos;
}  // Method to set the coilover position
void CoiloverAdjuster::set_speed(float speed) {
    m_speed = Utils::Calcs::constrain_float(speed, -100, 100);

}  // Method to set the coilover speed
void CoiloverAdjuster::reset() {
    set_pos(0);
    set_speed(0);
}  // Method to reset the coilover position

void CoiloverAdjuster::set_speed_offset(float offset) {
    m_speed_offset = Utils::Calcs::constrain_float(m_speed + offset, m_min_speed, m_max_speed);
}  // Method to set the coilover speed offset
void CoiloverAdjuster::run() {
    uint16_t comb_speed = Utils::Calcs::constrain_float(
      Utils::Calcs::map_float(m_speed + m_speed_offset, -100, 100, m_min_speed, m_max_speed),
      m_min_speed, m_max_speed);
    if (comb_speed == 0) {
        m_timer.restart();
        return;
    }
    uint16_t elapsed = m_timer.elapsed();
    uint16_t steps = comb_speed * (elapsed / 1e3);
    if (steps != 0) {
        m_timer.restart();
        float new_pos = m_current_pos + steps;
        set_pos(new_pos);
    }
}  // Method to run the coilover adjuster
