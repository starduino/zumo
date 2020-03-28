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
  src/hardware \

LIB_FILES := \

LIB_DIRS := \
  $(TINY)/src \
  $(STM8_TINY)/src/peripheral \

INC_DIRS := \
  $(STM8_TINY)/lib/stm8/inc \

include makefile-worker.mk
