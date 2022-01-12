TARGET := zumo
BUILD_DIR := ./build

DEVICE := stm8s207cb
DEVICE_TYPE := STM8S207
STLINK := stlinkv2
OPENOCD_CFG := tools/openocd/stm8s207.cfg
SVD := tools/svd/stm8s207k8.svd
TOOLCHAIN_VERSION := 4.0.0

include tools/defaults.mk

MAIN := src/main.c

SRC_FILES := \

SRC_DIRS := \
  src/application \
  src/application/meta-sensing \
  src/application/plugin \
  src/application/tactics \
  src/application/unit \
  src/application/utility \
  src/hardware/device \
  src/hardware/plugin \

include lib/stm8-tiny/lib_stm8-tiny.mk

include tools/tools.mk
