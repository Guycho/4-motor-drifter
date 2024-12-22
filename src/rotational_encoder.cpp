#include "rotational_encoder.h"

RotationalEncoder::RotationalEncoder(){}
RotationalEncoder::~RotationalEncoder(){}

void RotationalEncoder::init(const RotationalEncoderConfig &config){
    m_pin = config.pin;
    reverse = config.reverse;
    offset = config.offset;
}
float RotationalEncoder::getAngle(){
    float angle = analogRead(m_pin);
    angle = map(angle, 0, 1023, 0, 360);
    if(reverse){
        angle = 360 - angle;
    }
    angle = angle + offset;
    return angle;
}
