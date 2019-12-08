TARGET = tiny
BUILD_DIR = ./build/starduino-zumo

DEVICE := stm8s207cb
DEVICE_TYPE := STM8S207
STLINK := stlinkv2
OPENOCD_CFG := openocd/stm8s207.cfg

MAIN := src/app/starduino-zumo/main.c

SRC_FILES := \

SRC_DIRS := \
  src/app/starduino-zumo \

LIB_FILES := \
  src/peripheral/adc2.c \
  src/peripheral/clock.c \
  src/peripheral/i2c.c \
  src/peripheral/pc5_heartbeat.c \
  src/peripheral/tim4_system_tick.c \
  src/peripheral/watchdog.c \

LIB_DIRS := \
  lib/tiny/src \
  src/device \
  src/app/common \

INC_DIRS := \
  lib/stm8/inc \
  src/peripheral \

include makefile-worker.mk
