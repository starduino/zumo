TARGET = tiny
BUILD_DIR = ./build

DEVICE := stm8s105c6
DEVICE_TYPE := STM8S105
STLINK := stlinkv2
OPENOCD_CFG := openocd/stm8s105.cfg

MAIN := src/main.c

SRC_FILES := \

SRC_DIRS := \
  src \

LIB_FILES := \

LIB_DIRS := \
  lib/tiny/src \

INC_DIRS := \
  lib/stm8/inc \

include makefile-worker.mk
