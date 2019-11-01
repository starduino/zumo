/*!
 * @file
 * @brief Defines the general behaviors of the robot
 */

#ifndef tactic_h
#define tactic_h

#include <stdint.h>

enum {
  tactic_idle,
  tactic_seek_clockwise,
  tactic_seek_counterclockwise,
  tactic_charge,
  tactic_avoid_line
};
typedef uint8_t tactic_t;

#endif
