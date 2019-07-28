TARGET ?= stm8-tiny
BUILD_DIR ?= ./build

MAIN := src/main.c

SRC_DIRS := \
  src \

INC_DIRS := \
  lib/st/Libraries/STM8S_StdPeriph_Driver/inc

include makefile-worker.mk
