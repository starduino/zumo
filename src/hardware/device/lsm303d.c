/*!
 * @file
 * @brief
 */

#include "lsm303d.h"
#include "tiny_utils.h"

enum {
  period = 10,
  startup_delay = 100,

  address = 0x1D,

  auto_increment = (1 << 7),

  register_ctrl1 = 0x20,
  register_out_x_l_a = 0x28
};

static void swap_endianness(uint8_t* buffer)
{
  uint8_t temp = buffer[0];
  buffer[0] = buffer[1];
  buffer[1] = temp;
}

static void initialize_accelerometer(lsm303d_t* self);

static void read_acceleration_complete(void* context, bool success)
{
  reinterpret(self, context, lsm303d_t*);

  if(!success) {
    initialize_accelerometer(self);
  }
  else {
    self->busy = false;
    self->data_ready = true;
  }
}

static void set_up_acceleration_read_complete(void* context, bool success)
{
  reinterpret(self, context, lsm303d_t*);

  if(!success) {
    initialize_accelerometer(self);
  }
  else {
    tiny_async_i2c_read(
      self->i2c,
      address,
      false,
      self->read_buffer.raw,
      sizeof(self->read_buffer),
      self,
      read_acceleration_complete);
  }
}

static void set_up_acceleration_read(lsm303d_t* self)
{
  self->busy = true;

  static const uint8_t buffer[] = {
    register_out_x_l_a | auto_increment
  };

  tiny_async_i2c_write(
    self->i2c,
    address,
    true,
    buffer,
    sizeof(buffer),
    self,
    set_up_acceleration_read_complete);
}

static void poll(void* context)
{
  reinterpret(self, context, lsm303d_t*);

  if(self->data_ready) {
    self->data_ready = false;

    swap_endianness((uint8_t*)&self->read_buffer.acceleration.x);
    swap_endianness((uint8_t*)&self->read_buffer.acceleration.y);
    swap_endianness((uint8_t*)&self->read_buffer.acceleration.z);

    tiny_single_subscriber_event_publish(
      &self->acceleration_update,
      &self->read_buffer.acceleration);
  }

  if(!self->busy) {
    set_up_acceleration_read(self);
  }
}

static void arm_poll_timer(lsm303d_t* self)
{
  tiny_timer_start_periodic(self->timer_group, &self->timer, period, self, poll);
}

static void initialize_accelerometer(lsm303d_t* self);

static void configuration_complete(void* context, bool success)
{
  reinterpret(self, context, lsm303d_t*);

  if(!success) {
    initialize_accelerometer(self);
  }
  else {
    self->busy = false;
  }
}

static void initialize_accelerometer(lsm303d_t* self)
{
  self->busy = true;

  static const uint8_t buffer[] = {
    register_ctrl1,
    0x77 // 200 Hz, all axes enabled
  };

  tiny_async_i2c_write(
    self->i2c,
    address,
    false,
    buffer,
    sizeof(buffer),
    self,
    configuration_complete);
}

static void startup_delay_complete(void* context)
{
  reinterpret(self, context, lsm303d_t*);
  initialize_accelerometer(self);
  arm_poll_timer(self);
}

void lsm303d_init(lsm303d_t* self, tiny_timer_group_t* timer_group, i_tiny_async_i2c_t* i2c)
{
  self->i2c = i2c;
  self->busy = false;
  self->data_ready = false;
  self->timer_group = timer_group;

  tiny_single_subscriber_event_init(&self->acceleration_update);

  tiny_timer_start(timer_group, &self->timer, startup_delay, self, startup_delay_complete);
}

i_tiny_event_t* lsm303d_on_acceleration_update(lsm303d_t* self)
{
  return &self->acceleration_update.interface;
}
