PREFIX			= arm-eabi-
OPTIMIZE		= -O2
FLOAT_ABI		= soft
SRC_DIR			= ./
OBJ_DIR			= ./build

CC				:= $(PREFIX)gcc
CPP				:= $(PREFIX)g++
AS				:= $(CC)
LD				:= $(PREFIX)gcc
AR				:= $(PREFIX)ar
OBJDUMP			:= $(PREFIX)objdump
CPPFILT			:= $(PREFIX)c++filt
OBJCOPY			:= $(PREFIX)objcopy

ASRCS			:= $(wildcard $(SRC_DIR)/*.S)
SRCS1			:= $(wildcard $(SRC_DIR)/*.c)
SRCS2			:= $(wildcard $(SRC_DIR)/*.cpp)

AOBJS			:= $(subst $(SRC_DIR),$(OBJ_DIR), $(ASRCS:.S=.o))
OBJS1			:= $(subst $(SRC_DIR),$(OBJ_DIR), $(SRCS1:.c=.o))
OBJS2			:= $(subst $(SRC_DIR),$(OBJ_DIR), $(SRCS2:.cpp=.o))
OBJS			:= $(AOBJS) $(OBJS1) $(OBJS2)

ARCH			= -march=armv6k -mtune=arm1176jzf-s -marm -mfpu=vfp -mfloat-abi=$(FLOAT_ABI) -mlittle-endian

AFLAGS			= $(ARCH) -nostartfiles -ffreestanding
CFLAGS			= $(AFLAGS)
CPPFLAGS		= -fno-exceptions -fno-rtti -nostdinc++ $(CFLAGS) -std=c++14
LFLAG			= -static -nostartfiles

TARGET		= kernel

all: $(TARGET).img

$(TARGET).img: $(OBJ_DIR)/$(TARGET).elf
	$(OBJCOPY) $(OBJ_DIR)/$(TARGET).elf -O binary $(TARGET).img

$(OBJ_DIR)/$(TARGET).elf: $(OBJS) linker.ld
	$(LD) $(LFLAG) -T linker.ld $(OBJS) -o $@
	$(OBJDUMP) -d $(OBJ_DIR)/$(TARGET).elf | $(CPPFILT) > $(OBJ_DIR)/$(TARGET).lst

$(AOBJS): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.S
	@if not exist $(OBJ_DIR) ( mkdir $(subst /,\,$(OBJ_DIR)) )
	$(AS) $(AFLAGS) -c -o $@ $<

# source codes unique to this folder
$(OBJS1): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@if not exist $(OBJ_DIR) ( mkdir $(subst /,\,$(OBJ_DIR)) )
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJS2): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if not exist $(OBJ_DIR) ( mkdir $(subst /,\,$(OBJ_DIR)) )
	$(CPP) $(CPPFLAGS) -c -o $@ $<


clean:
	@if exist $(OBJ_DIR) ( rmdir /S /Q $(subst /,\,$(OBJ_DIR)) )
	@del /Q $(TARGET).img
