/*
 * Copyright © 2017, 2018, 2019 Eric Matthews,  Lesley Shannon
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

interface branch_predictor_interface;
import taiga_config::*;
import riscv_types::*;
import taiga_types::*;
import l2_config_and_types::*;
    //Fetch signals
    logic [31:0] if_pc;
    id_t if_id;
    logic new_mem_request;
    logic [31:0] next_pc;
    
    //Branch Predictor 
    logic [31:0] branch_flush_pc;
    logic [31:0] predicted_pc;
    logic use_prediction;
    logic is_return;
    logic is_call;
    logic is_branch;

    modport branch_predictor (
        input if_pc, if_id, new_mem_request, next_pc,
        output branch_flush_pc, predicted_pc, use_prediction, is_return, is_call, is_branch
    );
    modport fetch (
        input branch_flush_pc, predicted_pc, use_prediction, is_return, is_call, is_branch,
        output if_pc, if_id, new_mem_request, next_pc
     );

endinterface

interface unit_issue_interface;
	import taiga_config::*;
	import riscv_types::*;
	import taiga_types::*;
	import l2_config_and_types::*;
    logic possible_issue;
    logic new_request;
    logic new_request_r;
    id_t id;

    logic ready;

    modport decode (input ready, output possible_issue, new_request, new_request_r, id);
    modport unit (output ready, input possible_issue, new_request, new_request_r, id);
endinterface

interface unit_writeback_interface;
import taiga_config::*;
import riscv_types::*;
import taiga_types::*;
import l2_config_and_types::*;
        logic ack;

        id_t id;
        logic done;
        logic [XLEN-1:0] rd;

        modport unit (
            input ack,
            output id, done, rd
        );
        modport wb (
            output ack,
            input id, done, rd
        );
endinterface

interface ras_interface;
	import taiga_config::*;
import riscv_types::*;
import taiga_types::*;
import l2_config_and_types::*;
    logic push;
    logic pop;
    logic branch_fetched;
    logic branch_retired;

    logic [31:0] new_addr;
    logic [31:0] addr;

    modport branch_unit (output branch_retired);
    modport self (input push, pop, new_addr, branch_fetched, branch_retired, output addr);
    modport fetch (input addr, output pop, push, new_addr, branch_fetched);
endinterface

interface csr_exception_interface;
import taiga_config::*;
import riscv_types::*;
import taiga_types::*;
import l2_config_and_types::*;
    logic valid;
    exception_code_t code;
    logic [31:0] pc;
    logic [31:0] addr;

    logic illegal_instruction; //invalid CSR, invalid CSR op, or priviledge
    logic[31:0] csr_pc;

    modport csr (input valid, code, pc, addr, output illegal_instruction, csr_pc);
    modport econtrol (output valid, code, pc, addr, input illegal_instruction, csr_pc);

endinterface

interface exception_interface;
import taiga_config::*;
import riscv_types::*;
import taiga_types::*;
import l2_config_and_types::*;
    logic valid;
    logic ack;
    
    exception_code_t code;
    logic [31:0] pc;
    logic [31:0] addr;
    id_t id;

    modport econtrol (output valid, code, pc, addr, id, input ack);
    modport unit (input valid, code, pc, addr, id, output ack);
endinterface

interface fifo_interface
#(parameter DATA_WIDTH = 42);//#(parameter type data_type = logic[31:0]);
import taiga_config::*;
import riscv_types::*;
import taiga_types::*;
import l2_config_and_types::*;
    logic push;
    logic pop;
    logic [DATA_WIDTH-1:0] data_in;
    logic [DATA_WIDTH-1:0] data_out;
    logic valid;
    logic full;
    logic potential_push;
    modport enqueue (input full, output data_in, push, potential_push);
    modport dequeue (input valid, data_out, output pop);
    modport structure(input push, pop, data_in, potential_push, output data_out, valid, full);
endinterface

interface mmu_interface;
import taiga_config::*;
import riscv_types::*;
import taiga_types::*;
import l2_config_and_types::*;
    //From TLB
    logic new_request;
    logic execute;
    logic rnw;
    logic [31:0] virtual_address;

    //TLB response
    logic write_entry;
    logic [19:0] new_phys_addr;

    //From CSR
    logic [21:0] ppn;
    logic mxr; //Make eXecutable Readable
    logic pum; //Protect User Memory
    logic [1:0] privilege;

    modport mmu (input virtual_address, new_request, execute, rnw, ppn, mxr, pum, privilege, output write_entry, new_phys_addr);
    modport tlb (input write_entry, new_phys_addr, output new_request, virtual_address, execute, rnw);
    modport csr (output ppn, mxr, pum, privilege);

endinterface

interface tlb_interface;
import taiga_config::*;
import riscv_types::*;
import taiga_types::*;
import l2_config_and_types::*;
    logic [31:0] virtual_address;
    logic new_request;
    logic rnw;
    logic execute;

    logic complete;
    logic [31:0] physical_address;

    logic flush;
    logic flush_complete;

    modport tlb (input virtual_address, new_request, flush, rnw, execute,   output complete, physical_address, flush_complete);
    modport mem  (output new_request, virtual_address, rnw, execute, input complete, physical_address);
    modport fence (output flush, input flush_complete);

endinterface

interface load_store_queue_interface;
import taiga_config::*;
import riscv_types::*;
import taiga_types::*;
import l2_config_and_types::*;
    logic [31:0] addr;
    logic load;
    logic store;
    logic [3:0] be;
    logic [2:0] fn3;
    logic [31:0] data_in;
    id_t id;
    logic forwarded_store;
    id_t data_id;

    logic possible_issue;
    logic new_issue;
    logic ready;

    id_t id_needed_by_store;
    data_access_shared_inputs_t transaction_out;
    logic transaction_ready;
    logic empty;
    logic accepted;

    modport queue (input addr, load, store, be, fn3, data_in, id, forwarded_store, data_id, possible_issue, new_issue, accepted, output ready, id_needed_by_store, transaction_out, transaction_ready, empty);
    modport ls  (output addr, load, store, be, fn3, data_in, id, forwarded_store, data_id, possible_issue, new_issue, accepted, input ready, id_needed_by_store, transaction_out, transaction_ready, empty);
endinterface

interface writeback_store_interface;
import taiga_config::*;
import riscv_types::*;
import taiga_types::*;
import l2_config_and_types::*;
        id_t id_needed;
        logic possibly_waiting;
        logic waiting;
        logic ack;

        logic id_done;
        logic [31:0] data;

        modport ls (
            input id_done, data,
            output id_needed, possibly_waiting ,waiting, ack
        );
        modport wb (
            input id_needed, possibly_waiting, waiting, ack,
            output id_done, data
        );
endinterface

interface ls_sub_unit_interface 
#(	parameter BASE_ADDR = 32'h00000000, 
	parameter UPPER_BOUND = 32'hFFFFFFFF, 
	parameter BIT_CHECK = 4);
import taiga_config::*;
import riscv_types::*;
import taiga_types::*;
import l2_config_and_types::*;
    logic data_valid;
    logic ready;
    logic new_request;

    function  address_range_check (input logic[31:0] addr);
        return (addr[31:32-BIT_CHECK] == BASE_ADDR[31:32-BIT_CHECK]);
    endfunction

    modport sub_unit (input new_request, output data_valid, ready);
    modport ls (output new_request, input data_valid, ready);

endinterface


interface fetch_sub_unit_interface;
import taiga_config::*;
import riscv_types::*;
import taiga_types::*;
import l2_config_and_types::*;
    logic [31:0] stage1_addr;
    logic [31:0] stage2_addr;

    logic [31:0] data_out;
    logic data_valid;
    logic ready;
    logic new_request;
    logic flush;

    modport sub_unit (input stage1_addr, stage2_addr,  new_request, flush, output data_out, data_valid, ready);
    modport fetch (output stage1_addr, stage2_addr,  new_request, flush, input data_out, data_valid, ready);

endinterface

//start and done are cycle cycle pulses
interface unsigned_division_interface #(parameter DATA_WIDTH = 32);
import taiga_config::*;
import riscv_types::*;
import taiga_types::*;
import l2_config_and_types::*;
    logic start;
    logic [DATA_WIDTH-1:0] dividend;
    logic [DATA_WIDTH-1:0] divisor;
    logic [DATA_WIDTH-1:0] remainder;
    logic [DATA_WIDTH-1:0] quotient;
    logic done;
    logic divisor_is_zero;
    modport requester (input remainder, quotient, done, divisor_is_zero, output dividend, divisor, start);
    modport divider (output remainder, quotient, done, divisor_is_zero, input dividend, divisor, start);
endinterface

