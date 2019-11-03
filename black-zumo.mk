TARGET = tiny
BUILD_DIR = ./build/black-zumo

DEVICE := stm8s105k4
DEVICE_TYPE := STM8S105
STLINK := stlinkv2
OPENOCD_CFG := openocd/stm8s105.cfg

MAIN := src/app/starduino-zumo/main.c

SRC_FILES := \

SRC_DIRS := \
  src/app/starduino-zumo \

LIB_FILES := \
  src/peripheral/clock.c \
  src/peripheral/i2c.c \
  src/peripheral/pc5_heartbeat.c \
  src/peripheral/tim4_system_tick.c \
  src/peripheral/watchdog.c \

LIB_DIRS := \
  lib/tiny/src \
  src/device \
  src/app/modules \

INC_DIRS := \
  lib/stm8/inc \
  src/peripheral \

include makefile-worker.mk

# These are duplicated so that we can hook in to set option bytes on erase
# (needed for debug) and for upload
# Setting option bytes lets us use I2C on PB4, PB5

.PHONY: upload
upload: $(BUILD_DIR)/$(TARGET).hex erase
	@stm8flash -c $(STLINK) -p $(DEVICE) -w $<

.PHONY: erase
erase:
	@$(MKDIR_P) $(BUILD_DIR)
	@echo "AA" | xxd -r -p > $(BUILD_DIR)/rop.bin
	@stm8flash -c $(STLINK) -p $(DEVICE) -s opt -w $(BUILD_DIR)/rop.bin
	@stm8flash -c $(STLINK) -p $(DEVICE) -u
	@echo "00 00 FF 40 BF" | xxd -r -p > $(BUILD_DIR)/opt.bin
	@stm8flash -c $(STLINK) -p $(DEVICE) -s opt -w $(BUILD_DIR)/opt.bin
