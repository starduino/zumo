/*!
 * @file
 * @brief Defines the general behaviors of the robot
 */

#ifndef tactic_h
#define tactic_h

#include <stdint.h>

enum {
  tactic_seeking,
  tactic_charge
};
typedef uint8_t tactic_t;

#endif
