#ifndef P_CONTROLLER_H
#define P_CONTROLLER_H

#include <Arduino.h>
#include "utils.h"

struct PControllerConfig {
    float Kp;
    float max_output;
    };

class PController {
public:
 PController();
 ~PController();
 void init(const PControllerConfig& config);
 float calculateOutput(float error);

private:
    float m_kp;
    float m_max_output;
};

#endif // P_CONTROLLER_H