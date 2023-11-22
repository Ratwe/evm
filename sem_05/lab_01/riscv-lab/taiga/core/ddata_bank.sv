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
 * Initial code developed under the supervision of Dr. Lesley Shannon,
 * Reconfigurable Computing Lab, Simon Fraser University.
 *
 * Author(s):
 *             Eric Matthews <ematthew@sfu.ca>
 */

module ddata_bank 
	import taiga_config::*, taiga_types::*;
	#(
        parameter LINES = 2048
        )
        (
        input logic clk,
        input logic[$clog2(LINES)-1:0] addr_a,
        input logic en_a,
        input logic[3:0] be_a,
        input logic[31:0] data_in_a,
        output logic[31:0] data_out_a,

        //write only port
        input logic[$clog2(LINES)-1:0] addr_b,
        input logic en_b,
        input logic[31:0] data_in_b
        );

    byte_en_BRAM #(LINES, "", 0) ram_block (.be_b({4{en_b}}), 
					    .data_out_b(),
					     /*AUTOINST*/
					    // Outputs
					    .data_out_a		(data_out_a[XLEN-1:0]),
					    // Inputs
					    .clk		(clk),
					    .addr_a		(addr_a[$clog2(LINES)-1:0]),
					    .en_a		(en_a),
					    .be_a		(be_a[XLEN/8-1:0]),
					    .data_in_a		(data_in_a[XLEN-1:0]),
					    .addr_b		(addr_b[$clog2(LINES)-1:0]),
					    .en_b		(en_b),
					    .data_in_b		(data_in_b[XLEN-1:0]));

endmodule // ddata_bank
