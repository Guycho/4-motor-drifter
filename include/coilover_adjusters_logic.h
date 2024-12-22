#ifndef COILOVER_ADJUSTERS_LOGIC_H
#define COILOVER_ADJUSTERS_LOGIC_H

#include "config.h"
#include "utils.h"
#include "coilover_adjuster.h"
#include "ground_sensor.h"
#include "P_controller.h"

using RollPitch = Utils::Structs::RollPitch;

enum CoiloverMode { OFF = 0, STABILIZE = 1, KEEP_CONTACT = 2, RIDE_HEIGHT = 3 };

struct CoiloverAdjustersLogicConfig {
    CoiloverAdjuster* CoiloverAdjusters[Config::num_wheels];
    GroundSensor* GroundSensors[Config::num_wheels];
    PControllerConfig PControllers[Config::num_wheels];
};

class CoiloverAdjustersLogic {
   public:
    CoiloverAdjustersLogic();  // Constructor
    ~CoiloverAdjustersLogic();  // Destructor

    void init(const CoiloverAdjustersLogicConfig& config);  // Method to start the motor
    void run(bool arm_enabled, uint8_t coilover_mode, RollPitch roll_pitch,
      RollPitch des_roll_pitch, float ride_height);
    uint8_t get_num_of_coilover_modes();

   private:
    CoiloverAdjuster m_coilover_adjusters[Config::num_wheels];
    GroundSensor m_ground_sensors[Config::num_wheels];
    PController m_p_controllers[Config::num_wheels];


    void keep_contact();
    void run_coilover_adjusters();

    static constexpr CoiloverMode coilover_modes[] = {OFF, STABILIZE, KEEP_CONTACT, RIDE_HEIGHT};
    static constexpr uint8_t NUM_COILOVER_MODES =
      sizeof(coilover_modes) / sizeof(coilover_modes[0]);
    
    float m_shock_lengths[Config::num_wheels];
    bool m_ground_contact[Config::num_wheels];
    float m_shock_min_length[Config::num_wheels];
    float m_shock_max_length[Config::num_wheels];
    float m_coilover_adjusters_speed[Config::num_wheels];
};

#endif  // WHEEL_MOTORS_LOGIC_H