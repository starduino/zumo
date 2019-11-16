/*!
 * @file
 * @brief
 */

#ifndef vl6180x_h
#define vl6180x_h

#include <stdbool.h>
#include "i_tiny_i2c.h"
#include "tiny_timer.h"
#include "tiny_single_subscriber_event.h"

typedef struct {
  struct {
    int16_t x;
    int16_t y;
    int16_t z;
  };
} vl6180x_distance_update_t;

typedef struct {
  union {
    uint8_t raw[1];
    vl6180x_distance_update_t distance;
  };
} vl6180x_read_buffer_t;

typedef struct {
  i_tiny_i2c_t* i2c;
  tiny_timer_group_t* timer_group;
  tiny_timer_t timer;

  volatile bool busy;
  volatile bool data_ready;

  vl6180x_read_buffer_t read_buffer;

  tiny_single_subscriber_event_t distance_update;
} vl6180x_t;

void vl6180x_init(vl6180x_t* self, tiny_timer_group_t* timer_group, i_tiny_i2c_t* i2c);

i_tiny_event_t* vl6180x_on_distance_update(vl6180x_t* self);

#endif
