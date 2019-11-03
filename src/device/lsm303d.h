/*!
 * @file
 * @brief
 */

#ifndef lsm303d_h
#define lsm303d_h

#include "i_tiny_i2c.h"
#include "tiny_timer.h"

typedef struct {
  i_tiny_i2c_t* i2c;
  tiny_timer_group_t* timer_group;
  tiny_timer_t timer;
  uint8_t raw[6];
} lsm303d_t;

void lsm303d_init(lsm303d_t* self, tiny_timer_group_t* timer_group, i_tiny_i2c_t* i2c);

#endif
