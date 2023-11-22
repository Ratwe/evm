transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

set HEX_FILE $1

vlog -sv -svinputport=net -work work +incdir+lib {lib/taiga_config.sv}
vlog -sv -svinputport=net -work work +incdir+local_memory {local_memory/local_memory_interface.sv}
vlog -sv -svinputport=net -work work +incdir+l2_arbiter {l2_arbiter/l2_config_and_types.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/one_hot_to_integer.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/cycler.sv}
vlog -sv -svinputport=net -work work +incdir+lib {lib/riscv_types.sv}
vlog -sv -svinputport=net -work work +incdir+l2_arbiter {l2_arbiter/l2_interfaces.sv}
vlog -sv -svinputport=net -work work +incdir+l2_arbiter {l2_arbiter/l2_external_interfaces.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/illegal_instruction_checker.sv}
vlog -sv -svinputport=net -work work +incdir+lib {lib/taiga_types.sv}
vlog -sv -svinputport=net -work work +incdir+core/intel {core/intel/intel_byte_enable_ram.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/toggle_memory.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/shift_counter.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/set_clr_reg_with_rst.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/register_file.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/regfile_bank_sel.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/interfaces.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/external_interfaces.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/csr_types.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/byte_en_BRAM.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/branch_predictor_ram.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/branch_comparator.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/barrel_shifter.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/taiga_fifo.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/register_file_and_writeback.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/load_store_unit.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/instruction_metadata_and_id_management.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/ibram.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/gc_unit.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/fetch.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/decode_and_issue.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/dbram.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/csr_regs.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/branch_unit.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/branch_predictor.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/alu_unit.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/taiga.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/ras.sv}
vlog -sv -svinputport=net -work work +incdir+core {core/load_store_queue.sv}
vlog -sv -svinputport=net -work work +incdir+examples/zedboard {examples/zedboard/taiga_wrapper.sv}

vlog -sv -svinputport=net -work work +incdir+my_tb {my_tb/tb.sv}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L cyclonev_ver -L cyclonev_hssi_ver -L cyclonev_pcie_hip_ver -L rtl_work -L work -voptargs="+acc"  tb -G HEX_FILE=$HEX_FILE

do scripts/wave1.do
