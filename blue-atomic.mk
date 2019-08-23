TARGET = tiny
BUILD_DIR = ./build/blue-atomic

DEVICE := stm8s103f3
DEVICE_TYPE := STM8S103
STLINK := stlinkv2
OPENOCD_CFG := openocd/stm8s103.cfg

MAIN := src/app/blue-atomic/main.c

SRC_FILES := \
  src/peripheral/clock.c \

SRC_DIRS := \
  src/app/blue-atomic \
	src/rtos \

LIB_FILES := \
  lib/atomthreads/ports/stm8/atomport-asm-sdcc.s \

LIB_DIRS := \
  lib/atomthreads/kernel \

INC_DIRS := \
  src/peripheral \
  src/rtos \
  lib/stm8/inc \

include makefile-worker.mk
