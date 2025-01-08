#include "utils.h"

namespace Utils {
namespace Calcs {
float map_float(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
float constrain_float(float x, float min, float max) {
    if (x < min) {
        return min;
    } else if (x > max) {
        return max;
    } else {
        return x;
    }
}
float calc_dead_band(float x, float max_output, float dead_band) {
    if (abs(x) < dead_band) {
        return 0;
    }
    int sign = x > 0 ? 1 : -1;
    float abs_temp = Utils::Calcs::map_float(abs(x), dead_band, max_output, 0, max_output);
    float scaled_temp = sign * abs_temp;
    return scaled_temp;
}
float milli_to_single(float x) { return x / 1e3; }
float rad_to_deg(float x) { return x * 180 / PI; }
Utils::Structs::RollPitch rotateRollPitch45Degrees(Utils::Structs::RollPitch roll_pitch) {
    const float angle = M_PI / 4;  // 45 degrees in radians

    // Roll (x-axis) rotation matrix
    float Rx[3][3] = {{1, 0, 0}, {0, cos(angle), -sin(angle)}, {0, sin(angle), cos(angle)}};

    // Pitch (y-axis) rotation matrix
    float Ry[3][3] = {{cos(angle), 0, sin(angle)}, {0, 1, 0}, {-sin(angle), 0, cos(angle)}};

    // Combined rotation matrix R = Ry * Rx
    float R[3][3] = {0};

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            R[i][j] = 0;
            for (int k = 0; k < 3; ++k) {
                R[i][j] += Ry[i][k] * Rx[k][j];
            }
        }
    }

    // Input vector (roll, pitch, 0)
    float input[3] = {roll_pitch.roll, roll_pitch.pitch, 0};
    float output[3] = {0};

    // Apply the rotation matrix to the input vector
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            output[i] += R[i][j] * input[j];
        }
    }

    // Return the new roll and pitch values
    Utils::Structs::RollPitch return_roll_pitch = {output[0], output[1]};
    return return_roll_pitch;
}
float calc_hypotenuse_angle(float a, float b) { return atan2(b, a); }
float calc_shock_travel(float alpha, float a, float b) {
    // Using the Law of Cosines to calculate the length of side C
    float c = sqrt(a * a + b * b - 2 * a * b * cos(alpha));
    return c;
}
float calc_alpha(float a, float b, float c) {
    // Using the Law of Cosines to calculate the angle alpha
    float alpha = acos((a * a + b * b - c * c) / (2 * a * b));
    return alpha;
}
}  // namespace Calcs
}  // namespace Utils
