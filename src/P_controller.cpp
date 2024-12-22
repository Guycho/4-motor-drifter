#include "P_controller.h"

PController::PController() {}
PController::~PController() {}

void PController::init(const PControllerConfig& config) {
    m_kp = config.Kp;
    m_max_output = config.max_output;
}
float PController::calculateOutput(float error) {
    float output = m_kp * error;
    output = Utils::Calcs::constrain_float(output, -m_max_output, m_max_output);
    return output;
}
