/*!
 * @file
 * @brief Defines the motor control of the robot
 */

#ifndef motors_h
#define motors_h

#include <stdint.h>

typedef int8_t motor_power_t;

typedef struct {
  motor_power_t left_power;
  motor_power_t right_power;
} motors_t;

#endif
