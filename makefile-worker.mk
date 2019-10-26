DEFINES += $(DEVICE_TYPE)

SRCS := $(SRC_FILES)

ifneq ($(SRC_DIRS),)
SRCS += $(shell find $(SRC_DIRS) -not -wholename $(MAIN) -and -name *.c -or -name *.s)
endif

LIB_SRCS := $(LIB_FILES)

ifneq ($(LIB_DIRS),)
LIB_SRCS += $(shell find $(LIB_DIRS) -name *.c -or -name *.s)
endif

OBJS := $(SRCS:%=$(BUILD_DIR)/%.rel)
DEPS := $(SRCS:%=$(BUILD_DIR)/%.d)
LIB_OBJS := $(LIB_SRCS:%=$(BUILD_DIR)/%.rel)
LIB_DEPS := $(LIB_SRCS:%=$(BUILD_DIR)/%.d)

DEBUG_OBJS := $(SRCS:%=$(BUILD_DIR)/%.debug.rel)
DEBUG_DEPS := $(SRCS:%=$(BUILD_DIR)/%.debug.d)
DEBUG_LIB_OBJS := $(LIB_SRCS:%=$(BUILD_DIR)/%.debug.rel)
DEBUG_LIB_DEPS := $(LIB_SRCS:%=$(BUILD_DIR)/%.debug.d)

ifneq ($(SRC_DIRS),)
INC_DIRS += $(shell find $(SRC_DIRS) -type d)
endif

ifneq ($(LIB_DIRS),)
INC_DIRS += $(shell find $(LIB_DIRS) -type d)
endif

INC_FLAGS := $(addprefix -I,$(INC_DIRS))
DEFINE_FLAGS := $(addprefix -D,$(DEFINES))

ASFLAGS += \
  -loff

CFLAGS += \
  -mstm8 \
  --debug \
  --Werror \
  --std-c99 \
  --disable-warning 126 \
  $(INC_FLAGS) \
  $(DEFINE_FLAGS) \
  --opt-code-size \

LDFLAGS += \
  -lstm8 \
  $(CFLAGS) \

CC := sdcc
AS := sdasstm8
LD := sdcc
AR := sdar

.PHONY: all
all: $(BUILD_DIR)/$(TARGET)-debug.elf $(BUILD_DIR)/$(TARGET).hex

$(BUILD_DIR)/arm-none-eabi-gdb:
	@$(MKDIR_P) $(dir $@)
	@-ln -s `which stm8-gdb` $@

$(BUILD_DIR)/arm-none-eabi-objdump:
	@$(MKDIR_P) $(dir $@)
	@-ln -s `which stm8-objdump` $@

$(BUILD_DIR)/openocd.cfg:
	@cp $(OPENOCD_CFG) $@

.PHONY: debug-deps
debug-deps: erase $(BUILD_DIR)/$(TARGET)-debug.elf $(BUILD_DIR)/arm-none-eabi-gdb $(BUILD_DIR)/arm-none-eabi-objdump $(BUILD_DIR)/openocd.cfg

.PHONY: upload
upload: $(BUILD_DIR)/$(TARGET).hex
	@stm8flash -c $(STLINK) -p $(DEVICE) -w $<

.PHONY: erase
erase:
	@$(MKDIR_P) $(BUILD_DIR)
	@echo "AA" | xxd -r -p > $(BUILD_DIR)/rop.bin
	@stm8flash -c $(STLINK) -p $(DEVICE) -s opt -w $(BUILD_DIR)/rop.bin
	@stm8flash -c $(STLINK) -p $(DEVICE) -u

$(BUILD_DIR)/$(TARGET).hex: $(MAIN) $(OBJS) $(BUILD_DIR)/$(TARGET).lib
	@echo Linking $(notdir $@)...
	@$(MKDIR_P) $(dir $@)
	@$(LD) $(LDFLAGS) --out-fmt-ihx $^ -o $@

$(BUILD_DIR)/$(TARGET)-debug.elf: $(MAIN) $(DEBUG_OBJS) $(BUILD_DIR)/$(TARGET)-debug.lib
	@echo Linking $(notdir $@)...
	@$(MKDIR_P) $(dir $@)
	@$(LD) $(LDFLAGS) --out-fmt-elf $^ -o $@

$(BUILD_DIR)/$(TARGET).lib: $(LIB_OBJS)
	@echo Building $(notdir $@)...
	@$(MKDIR_P) $(dir $@)
	@$(AR) -rc $@ $(LIB_OBJS)

$(BUILD_DIR)/$(TARGET)-debug.lib: $(DEBUG_LIB_OBJS)
	@echo Building $(notdir $@)...
	@$(MKDIR_P) $(dir $@)
	@$(AR) -rc $@ $(DEBUG_LIB_OBJS)

$(BUILD_DIR)/%.s.rel: %.s
	@echo Assembling $(notdir $@)...
	@$(MKDIR_P) $(dir $@)
	@$(AS) $(ASFLAGS) $@ $<

$(BUILD_DIR)/%.s.debug.rel: %.s
	@echo Assembling $(notdir $@)...
	@$(MKDIR_P) $(dir $@)
	@$(AS) $(ASFLAGS) $@ $<

$(BUILD_DIR)/%.c.rel: %.c
	@echo Compiling $(notdir $@)...
	@$(MKDIR_P) $(dir $@)
	@$(CC) $(CFLAGS) -MM -c $< -o $(@:%.rel=%.d) && sed -i '1s:^$(notdir $(@:%.c.rel=%.rel)):$@:' $(@:%.rel=%.d)
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.c.debug.rel: %.c
	@echo Compiling $(notdir $@)...
	@$(MKDIR_P) $(dir $@)
	@$(CC) $(CFLAGS) -MM -c $< -o $(@:%.rel=%.d) && sed -i '1s:^$(notdir $(@:%.c.debug.rel=%.rel)):$@:' $(@:%.rel=%.d)
	@$(CC) $(CFLAGS) -c $< --out-fmt-elf -o $@

.PHONY: clean
clean:
	@echo Cleaning...
	@$(RM) -rf $(BUILD_DIR)

MKDIR_P ?= mkdir -p

-include $(DEPS) $(DEBUG_DEPS) $(LIB_DEPS) $(DEBUG_LIB_DEPS)
