/*!
 * @file
 * @brief Defines the general turning direction of the robot
 */

#ifndef direction_h
#define direction_h

#include <stdint.h>

enum {
  direction_forward,
  direction_backward,
  direction_clockwise,
  direction_counterclockwise
};
typedef uint8_t direction_t;

#endif
