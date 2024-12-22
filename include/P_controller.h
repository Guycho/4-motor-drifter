#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <Arduino.h>
#include <Chrono.h>

#include "utils.h"

struct PIDControllerConfig {
    float Kp;
    float Ki;
    float Kd;
    float max_output;
    float max_integral;  // Added for anti-windup
    float dt_seconds;
};

class PIDController {
   public:
    PIDController();
    ~PIDController();
    void init(const PIDControllerConfig& config);
    float calculateOutput(float error);

   private:
    Chrono m_dt_timer;
    float m_kp;
    float m_ki;
    float m_kd;
    float m_max_output;
    float m_max_integral;  // Added for anti-windup
    float m_integral;
    float m_previous_error;
};

#endif  // PID_CONTROLLER_H