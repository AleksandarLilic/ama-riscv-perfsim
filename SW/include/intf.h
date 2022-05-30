#pragma once

#include "defines.h"
#include "logic_t.h"


typedef struct ctrl_intf_t
{
    uint32_t in_inst_id;
    uint32_t in_inst_ex;
    uint32_t in_store_mask_offset;

    uint32_t opc7_id;
    uint32_t funct3_id;
    uint32_t funct7_id;
    uint32_t funct3_ex;

    // pipeline
    uint32_t stall_if;
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
    uint32_t dec_store_mask_id;
    uint32_t dec_dmem_en_id;
    uint32_t dec_load_sm_en_id;
    // writeback
    uint32_t dec_wb_sel_id;
    uint32_t dec_reg_we_id;

        
    uint32_t of_rf_a_sel_fwd_id;
    uint32_t of_rf_b_sel_fwd_id;
    uint32_t of_bc_a_sel_fwd_id;
    uint32_t of_bcs_b_sel_fwd_id;
    uint32_t of_alu_a_sel_fwd_id;
    uint32_t of_alu_b_sel_fwd_id;

} ctrl_intf_t;

typedef struct sys_intf_t
{
    bool rst;

} sys_intf_t;

typedef struct seq_id_ex_intf_t
{
    //logic_t rf_read_op_a;
    uint32_t dummy;

} seq_id_ex_intf_t;