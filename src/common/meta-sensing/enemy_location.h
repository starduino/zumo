/*!
 * @file
 * @brief Defines the general behaviors of the robot
 */

#ifndef enemy_location_h
#define enemy_location_h

#include <stdint.h>

enum {
  enemy_location_unknown,
  enemy_location_front_center,
  enemy_location_front_left,
  enemy_location_front_right
};
typedef uint8_t enemy_location_t;

#endif
