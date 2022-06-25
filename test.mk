TARGET ?= zumo_tests
BUILD_DIR ?= ./build/test
STM8_TINY = lib/stm8-tiny
TINY = $(STM8_TINY)/lib/tiny

SRC_DIRS ?= \
  $(TINY)/src \
  $(TINY)/test/src \
  src/application \
  src/application/meta-sensing \
  src/application/plugin \
  src/application/tactics \
  src/application/unit \
  src/application/utility \
  test/application \
  test/application/meta-sensing \
  test/application/tactics \
  test/application/utility \
  test/hardware \

INC_DIRS ?= \
  src/hardware \
  src/hardware/plugin \
  src/hardware/device \
  src/double/inc \
  $(STM8_TINY)/src \
  $(TINY)/include \
  $(TINY)/test/include \

SRC_FILES := \
  $(TINY)/test/tests/test_runner.cpp \

include $(TINY)/Makefile

.PHONY: debug-deps
debug-deps: $(BUILD_DIR)/$(TARGET)
