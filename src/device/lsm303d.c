/*!
 * @file
 * @brief
 */

#include "lsm303d.h"
#include "tiny_utils.h"

enum {
  period = 25,

  address = 0x1E,

  auto_increment = (1 << 7),

  register_ctrl1 = 0x20,
  register_out_x_l_a = 0x28
};

static void initialize_accelerometer(lsm303d_t* self);
static void arm_poll_timer(lsm303d_t* self);

static void read_acceleration_complete(void* context, bool success) {
  reinterpret(self, context, lsm303d_t*);

  if(!success) {
    initialize_accelerometer(self);
  }
  else {
    // we're in an ISR here, but we want to hand off results and do roughly this:
    // arm_poll_timer(self);
  }
}

static void set_up_acceleration_read_complete(void* context, bool success) {
  reinterpret(self, context, lsm303d_t*);

  if(!success) {
    initialize_accelerometer(self);
  }
  else {
    tiny_i2c_read(
      self->i2c,
      address,
      false,
      self->raw,
      sizeof(self->raw),
      read_acceleration_complete,
      self);
  }
}

static void set_up_acceleration_read(tiny_timer_group_t* group, void* context) {
  reinterpret(self, context, lsm303d_t*);
  (void)group;

  static const uint8_t buffer[] = {
    register_out_x_l_a | auto_increment
  };

  tiny_i2c_write(
    self->i2c,
    address,
    true,
    buffer,
    sizeof(buffer),
    set_up_acceleration_read_complete,
    self);
}

static void configuration_complete(void* context, bool success) {
  reinterpret(self, context, lsm303d_t*);

  if(!success) {
    initialize_accelerometer(self);
  }
  else {
    arm_poll_timer(self);
  }
}

static void arm_poll_timer(lsm303d_t* self) {
  tiny_timer_start(self->timer_group, &self->timer, period, set_up_acceleration_read, self);
}

static void initialize_accelerometer(lsm303d_t* self) {
  const uint8_t buffer[] = {
    register_ctrl1,
    0x57 // 50 Hz, all axes enabled
  };

  tiny_i2c_write(
    self->i2c,
    address,
    false,
    buffer,
    sizeof(buffer),
    configuration_complete,
    self);
}

void lsm303d_init(lsm303d_t* self, tiny_timer_group_t* timer_group, i_tiny_i2c_t* i2c) {
  self->i2c = i2c;
  self->timer_group = timer_group;

  initialize_accelerometer(self);
}
