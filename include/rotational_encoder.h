#ifndef ROTATIONAL_ENCODER_H
#define ROTATIONAL_ENCODER_H

#include <Arduino.h>

struct RotationalEncoderConfig {
    uint8_t pin;
    bool reverse;
    float offset;
};

class RotationalEncoder {
   public:
    RotationalEncoder();
    ~RotationalEncoder();

    void init(const RotationalEncoderConfig &config);
    float getAngle();

   private:
    uint8_t m_pin;
    bool reverse;
    float offset;
    float m_angle;
};

#endif  // ROTATIONAL_ENCODER_H