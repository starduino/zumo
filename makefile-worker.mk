SRCS := $(shell find $(SRC_DIRS) -not -wholename $(MAIN) -and -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.rel)
DEPS := $(SRCS:%=$(BUILD_DIR)/%.d)

INC_DIRS += $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
DEFINE_FLAGS := $(addprefix -D,$(DEFINES))

CFLAGS += \
  -mstm8 \
  --debug \
  --Werror \
  $(INC_FLAGS) \
  $(DEFINE_FLAGS) \
	--out-fmt-elf \
  --opt-code-size \

LDFLAGS += \
  -lstm8 \
  $(CFLAGS) \

CC := sdcc
AS := sdcc
LD := sdcc

.PHONY: all
all: $(BUILD_DIR)/$(TARGET).elf

$(BUILD_DIR)/arm-none-eabi-gdb:
	@$(MKDIR_P) $(dir $@)
	@-ln -s `which stm8-gdb` $(BUILD_DIR)/arm-none-eabi-gdb

.PHONY: debug-deps
debug-deps: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/arm-none-eabi-gdb

.PHONY: upload
upload: $(BUILD_DIR)/$(TARGET).hex
	@stm8flash -c stlinkv2 -p $(DEVICE) -w $<

.PHONY: erase
erase:
	@stm8flash -c stlinkv2 -p $(DEVICE) -u

$(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex: $(OBJS) $(MAIN)
	@$(MKDIR_P) $(dir $@)
	@echo Linking $(BUILD_DIR)/$(TARGET).hex...
	@$(LD) $(LDFLAGS) --out-fmt-ihx $(MAIN) $(OBJS) -o $(BUILD_DIR)/$(TARGET).hex
	@echo Linking $(BUILD_DIR)/$(TARGET).elf...
	@$(LD) $(LDFLAGS) --out-fmt-elf $(MAIN) $(OBJS) -o $(BUILD_DIR)/$(TARGET).elf

$(BUILD_DIR)/%.s.rel: %.s
	@echo Assembling $<...
	@$(MKDIR_P) $(dir $@)
	@$(AS) $(ASFLAGS) -c $< -o $@

$(BUILD_DIR)/%.c.rel: %.c
	@echo Compiling $<...
	@$(MKDIR_P) $(dir $@)
	@$(CC) $(CFLAGS) -MM -c $< -o $(@:%.rel=%.d) && sed -i '1s:^:$(dir $@):' $(@:%.rel=%.d)
	@$(CC) $(CFLAGS) -c $< --out-fmt-elf -o $@

.PHONY: clean
clean:
	@echo Cleaning...
	@$(RM) -rf $(BUILD_DIR)

MKDIR_P ?= mkdir -p

-include $(DEPS)
