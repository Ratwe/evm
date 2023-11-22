/*
 * Copyright © 2017 Eric Matthews,  Lesley Shannon
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Initial code developed under the supervismodule taiga_wrapper ion of Dr. Lesley Shannon,
 * Reconfigurable Computing Lab, Simon Fraser University.
 *
 * Author(s):
 *             Eric Matthews <ematthew@sfu.ca>
 */

module taiga_wrapper 
import taiga_config::*, taiga_types::*, l2_config_and_types::*;
(
	input logic sys_clk,
   input logic ext_reset,
	output trace_outputs_t tr
);
        
        
    parameter SCRATCH_MEM_KB = 16;
    parameter MEM_LINES = (SCRATCH_MEM_KB*1024)/4;
    parameter HEX_FILE = "";

    logic clk;
	 assign clk=sys_clk;
    logic rst;
	 assign rst=ext_reset;
    logic resetn;

    axi_interface m_axi();
    avalon_interface m_avalon();
    wishbone_interface m_wishbone();
    l2_requester_interface l2[L2_NUM_PORTS-1:0]();
    l2_memory_interface mem();

    logic interrupt;

    assign interrupt = 0;

    
    logic processor_reset;


    logic axi_clk;
    logic processor_clk;


    local_memory_interface instruction_bram();
    local_memory_interface data_bram();

	 logic timer_interrupt = 0;
	 
    taiga cpu(.*, .l2(l2[0]));

    //design_2 infra(.*);

   /* generate
        if (ENABLE_S_MODE || USE_ICACHE || USE_DCACHE) begin
            l2_arbiter l2_arb (.*, .request(l2));
            axi_to_arb l2_to_mem (.*, .l2(mem));
        end
    endgenerate */

    //arm proc(.*);
    byte_en_BRAM #(MEM_LINES, HEX_FILE, 1) inst_data_ram (
            .clk(clk),
            .addr_a(instruction_bram.addr[$clog2(MEM_LINES)- 1:0]),
            .en_a(instruction_bram.en),
            .be_a(instruction_bram.be),
            .data_in_a(instruction_bram.data_in),
            .data_out_a(instruction_bram.data_out),

            .addr_b(data_bram.addr[$clog2(MEM_LINES)- 1:0]),
            .en_b(data_bram.en),
            .be_b(data_bram.be),
            .data_in_b(data_bram.data_in),
            .data_out_b(data_bram.data_out)
        );

endmodule
