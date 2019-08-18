TARGET ?= stm8-tiny
BUILD_DIR ?= ./build

DEVICE := stm8s103f3
DEVICE_TYPE := STM8S103

MAIN := src/main.c

SRC_DIRS := \

LIB_DIRS := \
  lib/tiny/src \
	src/device \
	src/peripheral \

INC_DIRS := \
  lib/stm8/inc \

include makefile-worker.mk
