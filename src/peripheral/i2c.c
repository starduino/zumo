/*!
 * @file
 * @brief
 *
 * Good references:
 * - https://lujji.github.io/blog/bare-metal-programming-stm8/#I2C
 * - https://blog.mark-stevens.co.uk/2015/05/stm8s-i2c-master-devices/
 */

#include "stm8s_clk.h"
#include "stm8s_i2c.h"
#include "i2c.h"

enum {
  mode_write = I2C_DIRECTION_TX,
  mode_write_with_restart = 0x10 + I2C_DIRECTION_TX,
  mode_read = I2C_DIRECTION_RX,
  mode_read_with_restart = 0x10 + I2C_DIRECTION_RX,
};
typedef uint8_t mode_t;

static struct {
  i_tiny_i2c_t interface;

  mode_t mode;

  uint8_t address;

  union {
    const uint8_t* write;
    uint8_t* read;
  } buffer;
  uint8_t buffer_size;
  uint8_t buffer_offset;

  tiny_i2c_callback_t callback;
  void* context;
} self;

void i2c_isr(void) __interrupt(ITC_IRQ_I2C) {
  volatile uint8_t dummy;

  // Start condition generated
  if(I2C->SR1 & I2C_SR1_SB) {
    // Clear start condition by reading SR1
    dummy = I2C->SR1;

    // Send the slave address and R/W bit
    I2C->DR = (self.address << 1) | (self.mode & 0x01);

    return;
  }

  // Address sent
  if(I2C->SR1 & I2C_SR1_ADDR) {
    // Clear address sent event by reading SR1 and then SR3
    dummy = I2C->SR1;
    dummy = I2C->SR3;

    if(self.buffer_size == 1) {
      if(self.mode == mode_read) {
        I2C->CR2 = I2C_CR2_STOP;
      }
      else if(self.mode == mode_read_with_restart) {
        I2C->CR2 &= ~I2C_CR2_ACK;
      }
    }

    return;
  }

  // Transmit buffer is empty
  if(I2C->SR1 & I2C_SR1_TXE) {
    if(self.buffer_offset < self.buffer_size) {
      I2C->DR = self.buffer.write[self.buffer_offset++];
    }
    else {
      if(self.mode == mode_write) {
        I2C->CR2 = I2C_CR2_STOP;
        while(I2C->SR3 & I2C_SR3_MSL) {
        }
      }

      self.callback(self.context, true);
    }

    return;
  }

  // Receive buffer is not empty
  if(I2C->SR1 & I2C_SR1_RXNE) {
    do {
      // Clear event by reading received byte
      self.buffer.read[self.buffer_offset++] = I2C->DR;

      if(self.buffer_offset + 1 == self.buffer_size) {
        if(self.mode == mode_read) {
          I2C->CR2 = I2C_CR2_STOP;
        }
        else {
          I2C->CR2 = 0;
          self.callback(self.context, true);
        }
      }
      else if(self.buffer_offset == self.buffer_size) {
        while(I2C->SR3 & I2C_SR3_MSL) {
        }
        self.callback(self.context, true);
      }

      // Byte transfer finished
      // This means we got two bytes before we could read the first out
    } while(I2C->SR1 & I2C_SR1_BTF);

    return;
  }

  // If we're still here something is wrong so let's reset and tell the client
  I2C->CR2 = I2C_CR2_SWRST;
  self.callback(self.context, false);
}

static void write(
  i_tiny_i2c_t* _self,
  uint8_t address,
  bool prepare_for_restart,
  const uint8_t* buffer,
  uint8_t buffer_size,
  tiny_i2c_callback_t callback,
  void* context) {
  (void)_self;

  self.address = address;
  self.buffer.write = buffer;
  self.buffer_size = buffer_size;
  self.buffer_offset = 0;
  self.mode = prepare_for_restart ? mode_write_with_restart : mode_write;
  self.callback = callback;
  self.context = context;

  I2C->CR2 = I2C_CR2_START;
}

static void read(
  i_tiny_i2c_t* _self,
  uint8_t address,
  bool prepare_for_restart,
  uint8_t* buffer,
  uint8_t buffer_size,
  tiny_i2c_callback_t callback,
  void* context) {
  (void)_self;

  self.address = address;
  self.buffer.read = buffer;
  self.buffer_size = buffer_size;
  self.buffer_offset = 0;
  self.mode = prepare_for_restart ? mode_read_with_restart : mode_read;
  self.callback = callback;
  self.context = context;

  I2C->CR2 = I2C_CR2_START | I2C_CR2_ACK;
}

static void configure_peripheral(void) {
  // Un-gate clock for I2C
  CLK->PCKENR1 |= (1 << CLK_PERIPHERAL_I2C);

  // Set peripheral clock frequency to 16 MHz
  I2C->FREQR = 16;

  // Standard mode
  // SCL frequency = 1 / (2 * CCR * tMASTER) = 1 / (2 * 0x50 * 1/16,000,000) = 100 kHz
  I2C->CCRH = 0;
  I2C->CCRL = 0x50;

  // 7-bit addressing
  I2C->OARH = I2C_OARH_ADDCONF;

  // Maximum SCL rise time
  // In standard mode, maximum rise time is 1000 ns
  // The peripheral clock period is 62.5 ns (16 MHz)
  // 1000 / 62.5 = 16 => 16 + 1 = 17
  I2C->TRISER = 17;

  // Enable buffer, event, error interrupts
  I2C->ITR = I2C_ITR_ITBUFEN | I2C_ITR_ITEVTEN | I2C_ITR_ITERREN;

  // Enable peripheral
  I2C->CR1 = I2C_CR1_PE;
}

static void reset(i_tiny_i2c_t* _self) {
  (void)_self;
  I2C->CR2 = I2C_CR2_SWRST;
  configure_peripheral();
}

static const i_tiny_i2c_api_t api = { write, read, reset };

i_tiny_i2c_t* i2c_init(void) {
  configure_peripheral();

  self.interface.api = &api;

  return &self.interface;
}
