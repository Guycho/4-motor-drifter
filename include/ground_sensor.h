#ifndef GROUND_SENSOR_H
#define GROUND_SENSOR_H

#include <Arduino.h>
#include "utils.h"

struct GroundSensorConfig {
    uint8_t pin;
    bool reverse;
    float a_length;
    float b_length;
    float min_distance;
    float max_distance;
    uint16_t min_distance_value;
    uint16_t max_distance_value;
};

class GroundSensor {
   public:
    GroundSensor();
    ~GroundSensor();

    void init(const GroundSensorConfig &config);
    float get_distance();
    float get_max_distance();
    float get_min_distance();
   private:
    uint8_t m_pin;
    bool m_reverse;
    float m_a_length;
    float m_b_length;
    float m_min_distance;
    float m_max_distance;
    uint16_t m_min_distance_value;
    uint16_t m_max_distance_value;
    float m_min_alpha;
    float m_max_alpha;
    float m_distance;
};

#endif  // GROUND_SENSOR_H