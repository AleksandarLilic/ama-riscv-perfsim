#pragma once

#include "defines.h"
#include "logic_t.h"

// TODO: interfaces cannot have logic_t, creates issues with queue append and ordering
// TODO: global queue? visible to anyone, but only one for entire design
typedef struct dp_ex_intf_t
{

} dp_ex_intf_t;

typedef struct dec_intf_t
{

} dec_intf_t;

typedef struct ctrl_intf_t
{
    uint32_t in_inst_id;
    uint32_t in_inst_ex;
    uint32_t in_inst_mem;
    uint32_t in_store_mask_offset;

    uint32_t opc7_id;
    uint32_t funct3_id;
    uint32_t funct7_id;
    uint32_t funct3_ex;

    // pipeline
    uint32_t stall_if;
    uint32_t clear_if;
    uint32_t clear_id;
    uint32_t clear_ex;
    uint32_t clear_mem;

    // from control, as datapath control
    bool dec_branch_inst_id;
    bool dec_jump_inst_id;
    bool dec_store_inst_id;
    bool dec_load_inst_id;
    // fetch stage
    uint32_t dec_pc_sel_if;
    bool dec_pc_we_if;
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
    uint32_t dec_store_mask_id;
    uint32_t dec_dmem_en_id;
    uint32_t dec_load_sm_en_id;
    // writeback
    uint32_t dec_wb_sel_id;
    uint32_t dec_reg_we_id;

    // register addresses
    uint32_t rs1_addr_id;
    uint32_t rs2_addr_id;
    uint32_t rd_addr_id;
    // uint32_t rd_we_id; // as decoder output
    uint32_t rs1_addr_ex;
    uint32_t rs2_addr_ex;
    uint32_t rd_addr_ex;
    uint32_t rd_we_ex;
    uint32_t rs1_addr_mem;
    uint32_t rs2_addr_mem;
    uint32_t rd_addr_mem;
    uint32_t rd_we_mem;

    // forwarding
    uint32_t of_rf_a_sel_fwd_id;
    uint32_t of_rf_b_sel_fwd_id;
    uint32_t of_bc_a_sel_fwd_id;
    uint32_t of_bcs_b_sel_fwd_id;
    uint32_t of_alu_a_sel_fwd_id;
    uint32_t of_alu_b_sel_fwd_id;

    // from datapath
    bool bc_a_eq_b;
    bool bc_a_lt_b;
    bool dec_branch_inst_ex;
    bool dec_jump_inst_ex;

    uint32_t store_offset;

} ctrl_intf_t;

typedef struct sys_intf_t
{
    bool rst;
    bool rst_seq_id;
    bool rst_seq_ex;
    bool rst_seq_mem;

} sys_intf_t;

typedef struct seq_id_ex_intf_t
{
    //logic_t rf_read_op_a;
    uint32_t dummy;

} seq_id_ex_intf_t;

void assign(ctrl_intf_t *ctrl_intf, dec_intf_t *dec_intf);
void assign(ctrl_intf_t *dp_ex_intf_t, dec_intf_t *ctrl_intf);