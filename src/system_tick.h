/*!
 * @file
 * @brief
 */

#ifndef system_tick_h
#define system_tick_h

#include "stm8.h"
#include "i_tiny_time_source.h"

i_tiny_time_source_t* system_tick_init(void);

void system_tick_isr(void) __interrupt(TIM2_OVR_UIF_IRQ);

#endif
