#pragma once

#include "defines.h"
#include "logic_t.h"

// TODO: interfaces cannot have logic_t, creates issues with queue append and ordering
// TODO: global queue? visible to anyone, but only one for entire design

// struct if_intf_t;
// struct id_intf_t;
// struct ex_intf_t;
// struct mem_intf_t;
// struct wb_intf_t;

// stage interfaces
typedef struct if_intf_t
{
    uint32_t imem_addr;
    uint32_t pc;
    uint32_t pc_sel_if;
    uint32_t pc_we_if = 1u;
    uint32_t alu_out_ex;

} if_intf_t;

typedef struct id_intf_t
{
    uint32_t inst_id;

    uint32_t nx_pc;
    uint32_t rf_data_a;
    uint32_t rf_data_b;
    uint32_t imm_id;
    uint32_t store_inst;

    uint32_t in_store_mask_offset;

    uint32_t opc7_id;
    uint32_t funct3_id;
    uint32_t funct7_id;

    // pipeline
    uint32_t stall_if_id;
    uint32_t stall_if_id_d;
    uint32_t clear_if_id;
    uint32_t clear_id_ex;
    uint32_t clear_ex_mem;
    uint32_t clear_mem_wb;

    // from control, as datapath control
    uint32_t dec_branch_inst_id;
    uint32_t dec_jump_inst_id;
    uint32_t dec_store_inst_id;
    uint32_t dec_load_inst_id;
    // fetch stage
    // uint32_t dec_pc_sel_if;
    uint32_t dec_pc_sel_if;
    uint32_t dec_pc_we_if;
    uint32_t dec_ig_sel_id;
    uint32_t dec_csr_en_id;
    uint32_t dec_csr_we_id;
    uint32_t dec_csr_ui_id;
    // branching
    uint32_t dec_bc_uns_id;
    // alu
    uint32_t dec_alu_a_sel_id;
    uint32_t dec_alu_b_sel_id;
    uint32_t dec_alu_op_sel_id;
    // dmem
    uint32_t dec_store_mask_ex;
    uint32_t dec_dmem_en_id;
    uint32_t dec_load_sm_en_id;
    // writeback
    uint32_t dec_wb_sel_id;
    uint32_t dec_rd_we_id;

    // register addresses
    uint32_t rs1_addr_id;
    uint32_t rs2_addr_id;
    uint32_t rd_addr_id;
    // bool rd_we_id; // as decoder output

    // forwarding
    uint32_t of_rf_a_sel_fwd_id;
    uint32_t of_rf_b_sel_fwd_id;
    uint32_t of_bc_a_sel_fwd_id;
    uint32_t of_bcs_b_sel_fwd_id;
    uint32_t of_alu_a_sel_fwd_id;
    uint32_t of_alu_b_sel_fwd_id;

} id_intf_t;

typedef struct ex_intf_t
{
    uint32_t inst_ex;
    uint32_t funct3_ex;
    uint32_t rs1_addr_ex;
    uint32_t rs2_addr_ex;
    uint32_t rd_addr_ex;
    uint32_t rd_we_ex;

    uint32_t store_inst_ex;
    uint32_t branch_inst_ex;
    uint32_t jump_inst_ex;
    
    uint32_t bc_a_eq_b;
    uint32_t bc_a_lt_b;
    uint32_t alu_out;
    uint32_t store_offset;
} ex_intf_t;

typedef struct mem_intf_t
{
    uint32_t inst_mem;
    uint32_t funct3_mem;
    uint32_t rs1_addr_mem;
    uint32_t rs2_addr_mem;
    uint32_t rd_addr_mem;
    uint32_t rd_we_mem;
} mem_intf_t;

typedef struct wb_intf_t
{

} wb_intf_t;

// Pipeline interfaces
// typedef struct seq_if_id_intf_t
// {
//     logic_t pc;
//     logic_t inst_id;
// 
//     seq_if_id_intf_t() = delete;
//     seq_if_id_intf_t(seq_queue *q) :
//         pc(q, RESET_VECTOR, "Program Counter"),
//         inst_id(q, 0, "inst_id")
//     {
//         LOG("seq_if_id_intf called");
//     }
// 
// } seq_if_id_intf_t;
// 
// typedef struct seq_id_ex_intf_t
// {
//     logic_t rf_read_op_a_ex;
//     logic_t rf_read_op_b_ex;
// 
//     seq_id_ex_intf_t() = delete;
//     seq_id_ex_intf_t(seq_queue *q) : 
//         rf_read_op_a_ex(q, 0, "rf_read_op_a_ex"),
//         rf_read_op_b_ex(q, 0, "rf_read_op_b_ex")
//     {
//         LOG("seq_id_ex_intf called");
//     };
// 
// } seq_id_ex_intf_t;

// System interfaces
typedef struct sys_intf_t
{
    uint32_t rst;
    uint32_t rst_seq;
    uint32_t rst_seq_d1;
    uint32_t rst_seq_d2;
    uint32_t rst_seq_d3;

    uint32_t rst_seq_id_ex;
    uint32_t rst_seq_ex_mem;
    uint32_t rst_seq_mem_wb;
} sys_intf_t;
