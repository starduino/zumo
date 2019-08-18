/*!
 * @file
 * @brief
 */

#ifndef tcs34725_h
#define tcs34725_h

#include "i_tiny_i2c.h"
#include "tiny_timer.h"
#include "tiny_fsm.h"

typedef struct {
  tiny_timer_t timer;
  i_tiny_i2c_t* i2c;
  tiny_fsm_t fsm;
  union {
    uint8_t write_register_address[1];
    uint8_t read_device_id[1];
  } buffer;
} tcs34725_t;

void tcs34725_init(tcs34725_t* self, tiny_timer_group_t* timer_group, i_tiny_i2c_t* i2c);

#endif
