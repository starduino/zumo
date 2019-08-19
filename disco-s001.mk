TARGET = tiny
BUILD_DIR = ./build/disco-s001

DEVICE := stm8s001j3
DEVICE_TYPE := STM8S001
STLINK := stlinkv2
OPENOCD_CFG := openocd/stm8s001.cfg

MAIN := src/app/disco-s001/main.c

SRC_FILES := \

SRC_DIRS := \
  src/app/disco-s001 \

LIB_FILES := \
  src/peripheral/clock.c \
  src/peripheral/pb5_heartbeat.c \
  src/peripheral/tim4_system_tick.c \
  src/peripheral/watchdog.c \

LIB_DIRS := \
  lib/tiny/src \
  src/device \

INC_DIRS := \
  lib/stm8/inc \
  src/peripheral \

include makefile-worker.mk
