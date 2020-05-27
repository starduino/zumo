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

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS += $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS +=
CPPFLAGS += -fsanitize=address -fno-omit-frame-pointer
CPPFLAGS += $(INC_FLAGS) -MMD -MP -g -Wall -Wextra -Wcast-qual -Werror
CXXFLAGS +=
LDFLAGS := -fsanitize=address -lstdc++ -lCppUTest -lCppUTestExt -lm

.PHONY: test
test: $(BUILD_DIR)/$(TARGET)
	@echo Running tests...
	@$<

$(BUILD_DIR)/$(TARGET): $(OBJS)
	@echo Linking $@...
	@$(MKDIR_P) $(dir $@)
	@$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.s.o: %.s
	@echo Assembling $<...
	@$(MKDIR_P) $(dir $@)
	@$(AS) $(ASFLAGS) -c $< -o $@

$(BUILD_DIR)/%.c.o: %.c
	@echo Compiling $<...
	@$(MKDIR_P) $(dir $@)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.cpp.o: %.cpp
	@echo Compiling $<...
	@$(MKDIR_P) $(dir $@)
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@echo Cleaning...
	@$(RM) -rf $(BUILD_DIR)

.PHONY: debug-deps
debug-deps: $(BUILD_DIR)/$(TARGET)

MKDIR_P ?= mkdir -p

-include $(DEPS)
