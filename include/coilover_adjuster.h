#ifndef COILOVER_ADJUSTER_H
#define COILOVER_ADJUSTER_H

#include <ESP32Servo.h>

#include <Chrono.h>
#include "utils.h"
#include "P_controller.h"

struct CoiloverAdjusterConfig {
    uint8_t pin;
    uint16_t min_pulse;
    uint16_t max_pulse;
    uint16_t min_speed;
    uint16_t max_speed;
    bool reverse;
    PControllerConfig controller_config;
};

class CoiloverAdjuster {
   public:
    CoiloverAdjuster();  // Constructor
    ~CoiloverAdjuster();  // Destructor

    void init(const CoiloverAdjusterConfig &config);
    void set_speed(float speed);
    void set_speed_offset(float offset);
    void reset();
    void run();


   private:
    Servo m_servo_output;
    Chrono m_timer;
    void set_pos(float pos);

    uint16_t m_min_pulse;
    uint16_t m_max_pulse;
    uint16_t m_mid_pulse;
    bool m_reverse;
    float m_current_pos;
    float m_speed; //steps per second
    float m_speed_offset;
    float m_min_speed;
    float m_max_speed;
};

#endif  // COILOVER_ADJUSTER_H