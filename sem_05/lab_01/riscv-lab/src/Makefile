ifeq ($(OS),Windows_NT)
AS = riscv64-unknown-elf-as --march=rv32i
LD = riscv64-unknown-elf-ld -b elf32-littleriscv
OBJCOPY = riscv64-unknown-elf-objcopy
OBJDUMP = riscv64-unknown-elf-objdump
else
AS = riscv64-linux-gnu-as --march=rv32i
LD = riscv64-linux-gnu-ld -b elf32-littleriscv
OBJCOPY = riscv64-linux-gnu-objcopy
OBJDUMP = riscv64-linux-gnu-objdump
endif
SRC = test

%.o: %.s
	$(AS) $< -o $@

%.elf: %.o
	$(LD) -T link.ld $< -o $@

%.bin: %.elf
	$(OBJDUMP) -D -M numeric,no-aliases -t $<
	$(OBJCOPY) -O binary --reverse-bytes=4 $< $@ 

%.hex: %.bin 
	xxd -g 4 -c 4 -p $< $@
#	$(OBJCOPY) -O verilog -I binary --verilog-data-width=4 $< $@

default: $(SRC).hex

clean: 
	rm -f *.hex *.elf *.bin *.o
