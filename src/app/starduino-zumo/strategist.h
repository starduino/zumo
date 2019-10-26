/*!
 * @file
 * @brief
 */

#ifndef strategist_h
#define strategist_h

#include "tiny_event_subscription.h"
#include "tiny_fsm.h"

typedef struct {
  tiny_event_subscription_t on_change_subscription;
  tiny_fsm_t fsm;
} strategist_t;

#endif