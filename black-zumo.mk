TARGET = tiny
BUILD_DIR = ./build/black-zumo

DEVICE := stm8s105k4
DEVICE_TYPE := STM8S105
STLINK := stlinkv2
OPENOCD_CFG := openocd/stm8s105.cfg

MAIN := src/app/black-zumo/main.c

SRC_FILES := \

SRC_DIRS := \
  src/app/black-zumo \

LIB_FILES := \
  src/peripheral/clock.c \
  src/peripheral/pe5_heartbeat.c \
  src/peripheral/tim4_system_tick.c \
  src/peripheral/watchdog.c \

LIB_DIRS := \
  lib/tiny/src \
  src/device \

INC_DIRS := \
  lib/stm8/inc \
  src/peripheral \

include makefile-worker.mk
