TARGET = uart_rxtx_test

CC      = arm-none-eabi-gcc
AS      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

MCUFLAGS = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard

INCS = -Iinclude
SRCS = src/main.c src/uart.c startup/startup_stm32f411retx.s

OBJS = $(SRCS:.c=.o)
OBJS := $(OBJS:.s=.o)

LINKER_SCRIPT = linker/STM32F411RETX_FLASH.ld

CFLAGS = $(MCUFLAGS) $(INCS) \
         -Wall -Wextra -O2 \
         -ffreestanding \
         -DSTM32F411xE \
         -MMD -MP
LDFLAGS = $(MCUFLAGS) \
          -T$(LINKER_SCRIPT) \
          -Wl,-Map=$(TARGET).map \
          --specs=nosys.specs \
	  	  --specs=nano.specs \
	  	  -nostartfiles \
          -Wl,--gc-sections

.PHONY: all
all: $(TARGET).elf

$(TARGET).elf: $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@
	$(SIZE) $@

%.o: %.c uart.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(MCUFLAGS) -c $< -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

.PHONY: flash
flash:
	openocd -f interface/stlink.cfg \
	        -f target/stm32f4x.cfg \
	        -c "program $(TARGET).elf verify reset exit"

.PHONY: clean
clean:
	rm -f *.o *.d *.elf *.bin *.hex *.map