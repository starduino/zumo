/*!
 * @file
 * @brief
 */

#ifndef location_h
#define location_h

#include <stdint.h>

typedef int8_t location_t;

enum {
  location_unknown,
  location_left,
  location_right,
  location_center
};

#endif