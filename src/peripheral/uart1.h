/*!
 * @file
 * @brief
 */

#ifndef uart1_h
#define uart1_h

#include "stm8s_itc.h"
#include "i_tiny_uart.h"

i_tiny_uart_t* uart1_init(void);

void uart1_send_complete_isr(void) __interrupt(ITC_IRQ_UART1_TX);
void uart1_receive_isr(void) __interrupt(ITC_IRQ_UART1_RX);

#endif
