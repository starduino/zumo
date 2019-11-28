/*!
 * @file
 * @brief
 */

#ifndef application_h
#define application_h

#include "data_model.h"
#include "charge.h"

typedef struct {
  data_model_t data_model;
  charge_t charge;
} application_t;

void application_init(application_t* self);

#endif
