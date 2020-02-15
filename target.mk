TARGET = zumo
BUILD_DIR = ./build/target
STM8_TINY = lib/stm8-tiny
TINY = $(STM8_TINY)/lib/tiny

DEVICE := stm8s207cb
DEVICE_TYPE := STM8S207
STLINK := stlinkv2
OPENOCD_CFG := openocd/stm8s207.cfg

MAIN := src/main.c

SRC_FILES := \

SRC_DIRS := \
  src/application \
  src/bsp \
	src/common \
  src/device \

LIB_FILES := \
  $(STM8_TINY)/src/peripheral/adc2.c \
  $(STM8_TINY)/src/peripheral/clock.c \
  $(STM8_TINY)/src/peripheral/i2c.c \
  $(STM8_TINY)/src/peripheral/pa3_heartbeat.c \
  $(STM8_TINY)/src/peripheral/pc5_heartbeat.c \
  $(STM8_TINY)/src/peripheral/tim4_system_tick.c \
  $(STM8_TINY)/src/peripheral/watchdog.c \

LIB_DIRS := \
  $(TINY)/src \

INC_DIRS := \
  lib/stm8/inc \
  $(STM8_TINY)/src/peripheral \

include makefile-worker.mk
