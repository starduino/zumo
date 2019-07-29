TARGET ?= stm8-tiny
BUILD_DIR ?= ./build

DEVICE := stm8s103f3

MAIN := src/main.c

SRC_DIRS := \
  src \
	lib/tiny/src \

INC_DIRS := \
  lib/stm8

include makefile-worker.mk
