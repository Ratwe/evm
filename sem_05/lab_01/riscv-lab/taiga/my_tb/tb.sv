import taiga_config::*;
import taiga_types::*;
import l2_config_and_types::*;

module tb #(parameter HEX_FILE="") ();
   logic sys_clk; 
   logic ext_reset;
   trace_outputs_t tr;
   //assert (LOG2_COMMIT_PORTS==1);
   typedef logic sel_bank_t [32];
   typedef logic [31:0] register_file_t [32];
   sel_bank_t sel_bank [COMMIT_PORTS];
   sel_bank_t sel_bank1;
   register_file_t register_file;

   logic [31:0] cyc_cnt_reg;
   logic [31:0] cyc_cnt;
   
   
   taiga_wrapper #(.HEX_FILE(HEX_FILE)) uut(
        .sys_clk,
        .ext_reset,
	.tr
   );
   assign sel_bank = uut.cpu.register_file_and_writeback_block.regfile_lvt.sel_bank;
   always_comb
     begin
	int i;
	for(i=0; i<32;i++)
	  begin	  
	     sel_bank1[i]     = sel_bank[0][i] ^ sel_bank[1][i];
	     register_file[i] = (sel_bank1[i]==0)?uut.cpu.register_file_and_writeback_block.for2[0].register_file_blocks.register_file[i] : uut.cpu.register_file_and_writeback_block.for2[1].register_file_blocks.register_file[i];
	  end
     end
   
   initial 
     begin
	sys_clk <= 0;
	#1us;
	forever
	  #1us sys_clk <= ! sys_clk;
     end 

   initial 
     begin
	ext_reset <= 1;
	for(int i=0; i< 5; i++)      
	  @(negedge sys_clk);
	ext_reset <= 0;     
     end

   assign cyc_cnt = (uut.cpu.gc_unit_block.gc_fetch_hold | ext_reset) ? 32'hxxxxxxxx : cyc_cnt_reg;
   always @(posedge sys_clk)
     begin
	if(uut.cpu.gc_unit_block.gc_fetch_hold | ext_reset)
	  cyc_cnt_reg <= 1;
	else
	  cyc_cnt_reg <= cyc_cnt_reg + 1;
     end
        
endmodule 
