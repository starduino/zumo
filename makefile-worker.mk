SRCS := $(MAIN) $(shell find $(SRC_DIRS) -not -wholename $(MAIN) -and -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.rel)
DEPS := $(SRCS:%=$(BUILD_DIR)/%.d)

INC_DIRS += $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS += \
  $(INC_FLAGS) \
  -mstm8 \
  --opt-code-size \
  --max-allocs-per-node 50000 \
  --debug \
  --Werror \

LDFLAGS += \
  -mstm8 \
  --out-fmt-elf \

CC := sdcc
AS := sdcc
LD := sdcc

$(shell mkdir -p $(BUILD_DIR))

.PHONY: all
all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	@echo Linking $@...
	@$(MKDIR_P) $(dir $@)
	@$(LD) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.s.rel: %.s
	@echo Assembling $<...
	@$(MKDIR_P) $(dir $@)
	@$(AS) $(ASFLAGS) -c $< -o $@

$(BUILD_DIR)/%.c.rel: %.c
	@echo Compiling $<...
	@$(MKDIR_P) $(dir $@)
	@$(CC) $(CFLAGS) -MM -c $< -o $(@:%.rel=%.d) && sed -i '1s:^:$(dir $@):' $(@:%.rel=%.d)
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@echo Cleaning...
	@$(RM) -rf $(BUILD_DIR)

MKDIR_P ?= mkdir -p

-include $(DEPS)
