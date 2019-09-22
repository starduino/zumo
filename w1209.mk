TARGET = tiny
BUILD_DIR = ./build/w1209

DEVICE := stm8s003f3
DEVICE_TYPE := STM8S003
STLINK := stlinkv2
OPENOCD_CFG := openocd/stm8s103.cfg

MAIN := src/app/w1209/main.c

SRC_FILES := \

SRC_DIRS := \
  src/app/w1209 \

LIB_FILES := \
  src/peripheral/clock.c \
  src/peripheral/pa3_heartbeat.c \
  src/peripheral/tim4_system_tick.c \
  src/peripheral/watchdog.c \

LIB_DIRS := \
  lib/tiny/src \
  src/device \

INC_DIRS := \
  lib/stm8/inc \
  src/peripheral \

include makefile-worker.mk
