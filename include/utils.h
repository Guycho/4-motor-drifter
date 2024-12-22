#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

namespace Utils {
namespace Structs {
struct FourValues {
    float fr;
    float fl;
    float rr;
    float rl;
};
struct RollPitch {
    float roll;
    float pitch;
};
}  // namespace Structs
namespace Calcs {
float map_float(float x, float in_min, float in_max, float out_min, float out_max);
float constrain_float(float x, float min, float max);
float calc_dead_band(float x, float max_output, float dead_band);
float milli_to_single(float x);
float rad_to_deg(float x);
float calc_hypotenuse_angle(float a, float b);
float calc_shock_travel(float alpha, float a, float b);
float calc_alpha(float a, float b, float c);
Utils::Structs::RollPitch rotateRollPitch45Degrees(Utils::Structs::RollPitch roll_pitch);
}  // namespace Calcs
}  // namespace Utils
#endif  // UTILS_H