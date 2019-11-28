/*!
 * @file
 * @brief
 */

#include <stddef.h>
#include <stdlib.h>
#include "application.h"
#include "tiny_utils.h"

void application_init(application_t* self) {
  data_model_init(&self->data_model);
  i_tiny_key_value_store_t* store = data_model_key_value_store(&self->data_model);
  charge_init(&self->charge, store);
}
