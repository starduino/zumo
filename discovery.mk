TARGET = tiny
BUILD_DIR = ./build/discovery

DEVICE := stm8s105c6
DEVICE_TYPE := STM8S105
STLINK := stlink
OPENOCD_CFG := openocd/stm8s105.cfg

MAIN := src/app/discovery/main.c

SRC_FILES := \

SRC_DIRS := \
  src/app/discovery \

LIB_FILES := \
  src/peripheral/clock.c \
  src/peripheral/pd0_heartbeat.c \
  src/peripheral/tim4_system_tick.c \
  src/peripheral/uart2.c \
  src/peripheral/watchdog.c \

LIB_DIRS := \
  lib/tiny/src \
  src/device \

INC_DIRS := \
  lib/stm8/inc \
  src/peripheral \

include makefile-worker.mk
