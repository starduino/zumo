/*!
 * @file
 * @brief
 */

#include "vl6180x.h"

// enum {

// };

// stolen from app note:

/*
// Required private registers
WriteByte(0x0207, 0x01);
WriteByte(0x0208, 0x01);
WriteByte(0x0096, 0x00);
WriteByte(0x0097, 0xfd);
WriteByte(0x00e3, 0x00);
WriteByte(0x00e4, 0x04);
WriteByte(0x00e5, 0x02);
WriteByte(0x00e6, 0x01);
WriteByte(0x00e7, 0x03);
WriteByte(0x00f5, 0x02);
WriteByte(0x00d9, 0x05);
WriteByte(0x00db, 0xce);
WriteByte(0x00dc, 0x03);
WriteByte(0x00dd, 0xf8);
WriteByte(0x009f, 0x00);
WriteByte(0x00a3, 0x3c);
WriteByte(0x00b7, 0x00);
WriteByte(0x00bb, 0x3c);
WriteByte(0x00b2, 0x09);
WriteByte(0x00ca, 0x09);
WriteByte(0x0198, 0x01);
WriteByte(0x01b0, 0x17);
WriteByte(0x01ad, 0x00);
WriteByte(0x00ff, 0x05);
WriteByte(0x0100, 0x05);
WriteByte(0x0199, 0x05);
WriteByte(0x01a6, 0x1b);
WriteByte(0x01ac, 0x3e);
WriteByte(0x01a7, 0x1f);

// Recommended : Public registers - See data sheet for more detail
WriteByte(0x0030, 0x00);
WriteByte(0x0011, 0x10); // Enables polling for ‘New Sample ready’  // when measurement completes
WriteByte(0x010a, 0x30); // Set the averaging sample period // (compromise between lower noise and  // increased execution time)
WriteByte(0x003f, 0x46); // Sets the light and dark gain (upper  // nibble). Dark gain should not be                         //    changed.
WriteByte(0x0031, 0xFF); // sets the # of range measurements after  // which auto calibration of system is // performed
WriteByte(0x0040, 0x63); // Set ALS integration time to 100ms
WriteByte(0x002e, 0x01); // perform a single temperature calibration // of the ranging sensor Optional: Public registers - See data sheet for more detail
WriteByte(0x001b, 0x09); // Set default ranging inter-measurement  // period to 100ms
WriteByte(0x003e, 0x31); // Set default ALS inter-measurement period  // to 500ms
*/

void vl6180x_init(vl6180x_t* self, tiny_timer_group_t* timer_group, i_tiny_i2c_t* i2c) {
  self->i2c = i2c;
  self->busy = false;
  self->data_ready = false;

  tiny_single_subscriber_event_init(&self->distance_update);
}

i_tiny_event_t* vl6180x_on_distance_update(vl6180x_t* self) {
  return &self->distance_update.interface;
}
