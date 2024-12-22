#include "ground_sensor.h"

GroundSensor::GroundSensor() {}
GroundSensor::~GroundSensor() {}

void GroundSensor::init(const GroundSensorConfig &config) {
    m_pin = config.pin;
    m_reverse = config.reverse;
    m_a_length = config.a_length;
    m_b_length = config.b_length;
    m_min_distance = config.min_distance;
    m_max_distance = config.max_distance;
    m_min_distance_value = config.min_distance_value;
    m_max_distance_value = config.max_distance_value;
    m_min_alpha =
      Utils::Calcs::rad_to_deg(Utils::Calcs::calc_alpha(m_a_length, m_b_length, m_min_distance));
    m_max_alpha =
      Utils::Calcs::rad_to_deg(Utils::Calcs::calc_alpha(m_a_length, m_b_length, m_max_distance));
    pinMode(m_pin, INPUT);
}
float GroundSensor::get_distance() {
    uint16_t value = analogRead(m_pin);
    if (m_reverse) {
        value = 1023 - value;
    }
    float alpha = Utils::Calcs::map_float(value, m_min_distance_value, m_max_distance_value,
      m_min_distance, m_max_distance);
    alpha = Utils::Calcs::constrain_float(Utils::Calcs::rad_to_deg(alpha), m_min_alpha, m_max_alpha);
    m_distance = Utils::Calcs::calc_shock_travel(Utils::Calcs::rad_to_deg(alpha), m_a_length, m_b_length);
    return m_distance;
}

float GroundSensor::get_max_distance() { return m_max_distance; }
float GroundSensor::get_min_distance() { return m_min_distance; }