#Platform Details
#Platform Details
BASE_MEMORY_ADDR = 0x0000000000100000
AXI4BRAM_BASE_ADDR = 0x0000000000200000
MEM_SIZE_KB = 64
STACK_SIZE_KB = 4

#Sizes coverted into bytes
MEM_SIZE_B = $$(($(MEM_SIZE_KB)*1024))
STACK_SIZE_B = $$(($(STACK_SIZE_KB)*1024))

BOARD_CFLAGS = -DAXI4BRAM_BASE_ADDR=$(AXI4BRAM_BASE_ADDR) 

CFLAGS += $(BOARD_CFLAGS) -march=rv64im -mabi=lp64 -O3 -fno-inline -DDHRYSTONE_ITERATIONS=100000 -ffunction-sections -fdata-sections --specs=picolibc.specs -Wfatal-errors
CXXFLAGS += $(BOARD_CFLAGS) -march=rv64im -mabi=lp64 -O3 -fno-inline -DDHRYSTONE_ITERATIONS=100000 -ffunction-sections -fdata-sections --specs=picolibc.specs -fno-rtti -fno-exceptions -std=gnu++20 -Wfatal-errors
LDFLAGS = -Wl,--print-memory-usage,-gc-sections -Xlinker --defsym=__executable_start=$(BASE_MEMORY_ADDR) -Xlinker --defsym=__mem_addr=$(BASE_MEMORY_ADDR) -Xlinker --defsym=__mem_size=$(MEM_SIZE_B) -Xlinker  --defsym=__stack_size=$(STACK_SIZE_B)

CC = riscv64-unknown-elf-g++
ELF_TO_HW_INIT = python3 /opt/taiga_binary_converter64.py riscv64-unknown-elf- $(BASE_MEMORY_ADDR) $(MEM_SIZE_B)
OBJDUMP = riscv64-unknown-elf-objdump
OBJCOPY = riscv64-unknown-elf-objcopy
CC = riscv64-unknown-elf-g++
AR = riscv64-unknown-elf-ar
RM = rm -f
RMDIR = rm -rf
