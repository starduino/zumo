/*!
 * @file
 * @brief
 */

#include <stddef.h>
#include "tcs34725.h"
#include "tiny_utils.h"

#define self_from_fsm(_fsm) container_of(tcs34725_t, fsm, _fsm)

enum {
  address = 0x29,

  command_set_register = 0x80,

  register_device_id = 0x12
};

enum {
  signal_entry = tiny_fsm_signal_entry,
  signal_exit = tiny_fsm_signal_exit,
  signal_id_requested = tiny_fsm_signal_user_start,
  signal_register_address_written,
  signal_register_value_read
};

static void register_address_written(void* context, bool success) {
  (void)success;
  reinterpret(self, context, tcs34725_t*);
  tiny_fsm_send_signal(&self->fsm, signal_register_address_written, NULL);
}

static void register_value_read(void* context, bool success) {
  (void)success;
  reinterpret(self, context, tcs34725_t*);
  tiny_fsm_send_signal(&self->fsm, signal_register_value_read, NULL);
}

static void state_idle(tiny_fsm_t* fsm, tiny_fsm_signal_t signal, const void* data);
static void state_write_register_address(tiny_fsm_t* fsm, tiny_fsm_signal_t signal, const void* data);
static void state_read_value(tiny_fsm_t* fsm, tiny_fsm_signal_t signal, const void* data);

static void state_idle(tiny_fsm_t* fsm, tiny_fsm_signal_t signal, const void* data) {
  (void)data;

  switch(signal) {
    case signal_id_requested:
      tiny_fsm_transition(fsm, state_write_register_address);
      break;
  }
}

static void state_write_register_address(tiny_fsm_t* fsm, tiny_fsm_signal_t signal, const void* data) {
  (void)data;
  tcs34725_t* self = self_from_fsm(fsm);

  switch(signal) {
    case signal_entry:
      self->buffer.write_register_address[0] = command_set_register | register_device_id;
      tiny_i2c_write(
        self->i2c,
        address,
        false,
        self->buffer.write_register_address,
        sizeof(self->buffer.write_register_address),
        register_address_written,
        self);
      break;

    case signal_register_address_written:
      tiny_fsm_transition(fsm, state_read_value);
      break;
  }
}

static void state_read_value(tiny_fsm_t* fsm, tiny_fsm_signal_t signal, const void* data) {
  (void)data;
  tcs34725_t* self = self_from_fsm(fsm);

  switch(signal) {
    case signal_entry:
      tiny_i2c_read(
        self->i2c,
        address,
        false,
        self->buffer.read_device_id,
        sizeof(self->buffer.read_device_id),
        register_value_read,
        self);
      break;

    case signal_register_value_read:
      tiny_fsm_transition(fsm, state_idle);
      break;
  }
}

void tcs34725_init(tcs34725_t* self, tiny_timer_group_t* timer_group, i_tiny_i2c_t* i2c) {
  (void)timer_group;

  self->i2c = i2c;
  tiny_fsm_init(&self->fsm, state_idle);

  tiny_fsm_send_signal(&self->fsm, signal_id_requested, NULL);
}
