/*!
 * @file
 * @brief
 */

#ifndef system_tick_h
#define system_tick_h

#include "stm8s_itc.h"
#include "i_tiny_time_source.h"

i_tiny_time_source_t* system_tick_init(void);

void system_tick_isr(void) __interrupt(ITC_IRQ_TIM4_OVF);

#endif
