/*!
 * @file
 * @brief
 */

#ifndef spi_h
#define spi_h

#include <stdbool.h>
#include "stm8s_itc.h"
#include "i_tiny_spi.h"

i_tiny_spi_t* spi_init(void);

void spi_isr(void) __interrupt(ITC_IRQ_SPI);

#endif
