/*!
 * @file
 * @brief Defines the general behaviors of the robot
 */

#ifndef tactic_h
#define tactic_h

#include <stdint.h>

typedef uint8_t tactic_t;

enum {
  tactic_seeking,
  tactic_charge
};

#endif