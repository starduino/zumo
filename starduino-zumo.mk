TARGET = tiny
BUILD_DIR = ./build/starduino-zumo

DEVICE := stm8s105c6
DEVICE_TYPE := STM8S105
STLINK := stlinkv2
OPENOCD_CFG := openocd/stm8s105.cfg

MAIN := src/app/starduino-zumo/main.c

SRC_FILES := \

SRC_DIRS := \
  src/app/starduino-zumo \

LIB_FILES := \
  src/peripheral/clock.c \
  src/peripheral/pc5_heartbeat.c \
  src/peripheral/tim4_system_tick.c \
  src/peripheral/watchdog.c \

LIB_DIRS := \
  lib/tiny/src \
  src/device \

INC_DIRS := \
  lib/stm8/inc \
  src/peripheral \

include makefile-worker.mk
