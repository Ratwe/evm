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

module itag_banks
		import taiga_config::*, taiga_types::*;
		(
        input logic clk,
        input logic rst,

        input logic[31:0] stage1_addr,
        input logic[31:0] stage2_addr,

        input logic[ICACHE_WAYS-1:0] update_way,
        input logic update,

        input logic stage1_adv,

        output tag_hit,
        output logic[ICACHE_WAYS-1:0] tag_hit_way
        );

    typedef logic [ICACHE_TAG_W : 0] itag_entry_t;

    function logic[ICACHE_TAG_W-1:0] getTag(logic[31:0] addr);
        return addr[2+ICACHE_SUB_LINE_ADDR_W+ICACHE_LINE_ADDR_W +: ICACHE_TAG_W];
    endfunction

    function logic[ICACHE_LINE_ADDR_W-1:0] getLineAddr(logic[31:0] addr);
        return addr[ICACHE_LINE_ADDR_W + ICACHE_SUB_LINE_ADDR_W + 1 : ICACHE_SUB_LINE_ADDR_W + 2];
    endfunction

    logic hit_allowed;
    itag_entry_t  tag_line[ICACHE_WAYS-1:0];

    itag_entry_t stage2_tag;
    assign stage2_tag = {1'b1, getTag(stage2_addr)};


    always_ff @ (posedge clk) begin
        if (rst)
            hit_allowed <= 0;
        else
            hit_allowed <= stage1_adv;
    end

    genvar i;
    generate
        for (i=0; i < ICACHE_WAYS; i++) begin : tag_bank_gen

            tag_bank #(ICACHE_TAG_W+1, ICACHE_LINES) itag_bank (.en_a(stage1_adv), 
								.wen_a('0),
								.addr_a(getLineAddr(stage1_addr)),
								.data_in_a('0), 
								.data_out_a(tag_line[i]),
								.en_b(update), 
								.wen_b(update_way[i]),
								.addr_b(getLineAddr(stage2_addr)),
								.data_in_b(stage2_tag), 
								.data_out_b(),
								/*AUTOINST*/
								// Inputs
								.clk		(clk),
								.rst		(rst));

            assign tag_hit_way[i] = ({hit_allowed,stage2_tag} == {1'b1,tag_line[i]});

        end
    endgenerate

    assign tag_hit = |tag_hit_way;


endmodule
