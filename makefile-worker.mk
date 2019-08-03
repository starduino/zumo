DEFINES += $(DEVICE_TYPE)

SRCS := $(shell find $(SRC_DIRS) -not -wholename $(MAIN) -and -name *.c -or -name *.s)
LIB_SRCS := $(shell find $(LIB_DIRS) -name *.c -or -name *.s)

OBJS := $(SRCS:%=$(BUILD_DIR)/%.rel)
DEPS := $(SRCS:%=$(BUILD_DIR)/%.d)
LIB_OBJS := $(LIB_SRCS:%=$(BUILD_DIR)/%.rel)
LIB_DEPS := $(LIB_SRCS:%=$(BUILD_DIR)/%.d)

ELF_OBJS := $(SRCS:%=$(BUILD_DIR)/%.elf.rel)
ELF_DEPS := $(SRCS:%=$(BUILD_DIR)/%.elf.d)
ELF_LIB_OBJS := $(LIB_SRCS:%=$(BUILD_DIR)/%.elf.rel)
ELF_LIB_DEPS := $(LIB_SRCS:%=$(BUILD_DIR)/%.elf.d)

INC_DIRS += $(shell find $(SRC_DIRS) -type d)
ifneq ($(LIB_DIRS),)
INC_DIRS += $(shell find $(LIB_DIRS) -type d)
endif

INC_FLAGS := $(addprefix -I,$(INC_DIRS))
DEFINE_FLAGS := $(addprefix -D,$(DEFINES))

CFLAGS += \
  -mstm8 \
  --debug \
  --Werror \
  $(INC_FLAGS) \
  $(DEFINE_FLAGS) \
  --opt-code-size \

LDFLAGS += \
  -lstm8 \
  $(CFLAGS) \

CC := sdcc
AS := sdcc
LD := sdcc
AR := sdar

.PHONY: all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex

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

$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).lib $(OBJS) $(MAIN)
	@echo Linking $(notdir $@)...
	@$(MKDIR_P) $(dir $@)
	@$(LD) $(LDFLAGS) --out-fmt-ihx $(MAIN) $(OBJS) $(BUILD_DIR)/$(TARGET).lib -o $(BUILD_DIR)/$(TARGET).hex

$(BUILD_DIR)/$(TARGET).elf: $(BUILD_DIR)/$(TARGET).elf.lib $(ELF_OBJS) $(MAIN)
	@echo Linking $(notdir $@)...
	@$(MKDIR_P) $(dir $@)
	@$(LD) $(LDFLAGS) --out-fmt-elf $(MAIN) $(ELF_OBJS) $(BUILD_DIR)/$(TARGET).elf.lib -o $(BUILD_DIR)/$(TARGET).elf

$(BUILD_DIR)/$(TARGET).lib: $(LIB_OBJS)
	@echo Building $(notdir $@)...
	@$(MKDIR_P) $(dir $@)
	@$(AR) -rc $@ $(LIB_OBJS)

$(BUILD_DIR)/$(TARGET).elf.lib: $(ELF_LIB_OBJS)
	@echo Building $(notdir $@)...
	@$(MKDIR_P) $(dir $@)
	@$(AR) -rc $@ $(ELF_LIB_OBJS)

$(BUILD_DIR)/%.s.rel: %.s
	@echo Assembling $(notdir $@)...
	@$(MKDIR_P) $(dir $@)
	@$(AS) $(ASFLAGS) -c $< -o $@

$(BUILD_DIR)/%.s.elf.rel: %.s
	@echo Assembling $(notdir $@)...
	@$(MKDIR_P) $(dir $@)
	@$(AS) $(ASFLAGS) -c $< -o $@

$(BUILD_DIR)/%.c.rel: %.c
	@echo Compiling $(notdir $@)...
	@$(MKDIR_P) $(dir $@)
	@$(CC) $(CFLAGS) -MM -c $< -o $(@:%.rel=%.d) && sed -i '1s:^$(notdir $(@:%.c.rel=%.rel)):$@:' $(@:%.rel=%.d)
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.c.elf.rel: %.c
	@echo Compiling $(notdir $@)...
	@$(MKDIR_P) $(dir $@)
	@$(CC) $(CFLAGS) -MM -c $< -o $(@:%.rel=%.d) && sed -i '1s:^$(notdir $(@:%.c.elf.rel=%.rel)):$@:' $(@:%.rel=%.d)
	@$(CC) $(CFLAGS) -c $< --out-fmt-elf -o $@

.PHONY: clean
clean:
	@echo Cleaning...
	@$(RM) -rf $(BUILD_DIR)

MKDIR_P ?= mkdir -p

-include $(DEPS) $(ELF_DEPS) $(LIB_DEPS) $(ELF_LIB_DEPS)
