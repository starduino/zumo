TARGET = tiny
BUILD_DIR = ./build/nucleo-64

DEVICE := stm8s208rb
DEVICE_TYPE := STM8S208
STLINK := stlinkv21
OPENOCD_CFG := openocd/stm8s208.cfg

MAIN := src/app/nucleo-64/main.c

SRC_FILES := \

SRC_DIRS := \
  src/app/nucleo-64 \

LIB_FILES := \
  src/peripheral/clock.c \
  src/peripheral/pc5_heartbeat.c \
  src/peripheral/tim4_system_tick.c \
  src/peripheral/watchdog.c \

LIB_DIRS := \
  lib/tiny/src \

INC_DIRS := \
  lib/stm8/inc \
  src/peripheral \

include makefile-worker.mk
