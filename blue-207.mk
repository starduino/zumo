TARGET = tiny
BUILD_DIR = ./build/blue-207

DEVICE := stm8s207rb
DEVICE_TYPE := STM8S207
STLINK := stlinkv2
OPENOCD_CFG := openocd/stm8s207.cfg

MAIN := src/app/blue-207/main.c

SRC_FILES := \

SRC_DIRS := \
  src/app/blue-207 \

LIB_FILES := \
  src/peripheral/clock.c \
  src/peripheral/tim4_system_tick.c \
  src/peripheral/watchdog.c \

LIB_DIRS := \
  lib/tiny/src \
  src/device \

INC_DIRS := \
  lib/stm8/inc \
  src/peripheral \

include makefile-worker.mk
