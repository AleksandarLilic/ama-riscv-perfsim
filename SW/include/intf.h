#pragma once

#include "defines.h"
#include "logic_t.h"

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
    uint32_t store_inst;
    uint32_t imm_gen_out;
    // reg file out
    uint32_t rf_data_a;
    uint32_t rf_data_b;

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
    // uint32_t rd_we_id; // as decoder output
    
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
    uint32_t pc_ex;
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

    uint32_t wb_sel_ex;
} ex_intf_t;

typedef struct mem_intf_t
{
    uint32_t inst_mem;
    uint32_t alu_mem;
    uint32_t pc_mem;
    uint32_t funct3_mem;
    uint32_t rs1_addr_mem;
    uint32_t rs2_addr_mem;
    uint32_t rd_addr_mem;
    uint32_t rd_we_mem;
    uint32_t wb_sel_mem;

} mem_intf_t;

typedef struct wb_intf_t
{
    uint32_t load_sm_out;
    uint32_t data_d;

} wb_intf_t;

typedef struct reg_file_intf_t
{
    uint32_t in_r1, out_r1;
    uint32_t in_r2, out_r2;
    uint32_t in_r3, out_r3;
    uint32_t in_r4, out_r4;
    uint32_t in_r5, out_r5;
    uint32_t in_r6, out_r6;
    uint32_t in_r7, out_r7;
    uint32_t in_r8, out_r8;
    uint32_t in_r9, out_r9;
    uint32_t in_r10, out_r10;
    uint32_t in_r11, out_r11;
    uint32_t in_r12, out_r12;
    uint32_t in_r13, out_r13;
    uint32_t in_r14, out_r14;
    uint32_t in_r15, out_r15;
    uint32_t in_r16, out_r16;
    uint32_t in_r17, out_r17;
    uint32_t in_r18, out_r18;
    uint32_t in_r19, out_r19;
    uint32_t in_r20, out_r20;
    uint32_t in_r21, out_r21;
    uint32_t in_r22, out_r22;
    uint32_t in_r23, out_r23;
    uint32_t in_r24, out_r24;
    uint32_t in_r25, out_r25;
    uint32_t in_r26, out_r26;
    uint32_t in_r27, out_r27;
    uint32_t in_r28, out_r28;
    uint32_t in_r29, out_r29;
    uint32_t in_r30, out_r30;
    uint32_t in_r31, out_r31;
} reg_file_t;

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
