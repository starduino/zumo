/*!
 * @file
 * @brief
 */

#include "i_tiny_key_value_store.h"
#include "tiny_timer.h"
#include "strategist.h"
#include "line_detected.h"
#include "charge.h"
#include "seeking.h"

typedef struct {
  strategist_t strategist;
  line_detected_t line_detected;
  charge_t charge;
  seeking_t seeking;
} strategy_plugin_t;

void strategy_plugin_init(
  strategy_plugin_t* self,
  i_tiny_key_value_store_t* key_value_store,
  tiny_timer_group_t* timer_group);