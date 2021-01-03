TARGET ?= zumo_tests
BUILD_DIR ?= ./build/test
STM8_TINY = lib/stm8-tiny
TINY = $(STM8_TINY)/lib/tiny

SRC_DIRS ?= \
  $(TINY)/src \
  $(TINY)/test/double \
  src/application \
  test \

INC_DIRS ?= \
  src/hardware \
  src/hardware/plugin \
  src/hardware/device \
  src/double/inc \
  $(STM8_TINY)/src \
  $(TINY)/include \

SRC_FILES := \
  $(TINY)/test/test_runner.cpp \

include $(TINY)/Makefile

.PHONY: debug-deps
debug-deps: $(BUILD_DIR)/$(TARGET)
