#pragma once

#include "defines.h"
#include "logic_t.h"


typedef struct ctrl_intf_t
{
    // to control
    uint32_t inst_id;
    uint32_t inst_ex;
    uint32_t store_mask_offset;
    // from control, as pipeline control
    uint32_t stall_if;
    uint32_t clear_id;
    uint32_t clear_ex;
    uint32_t clear_mem;
    // from control, as datapath control
    // fetch stage
    uint32_t pc_sel_if;
    uint32_t pc_we_if;
    uint32_t ig_sel_id;
    // rf fwd in decode stage
    uint32_t rf_a_sel_fwd_id;
    uint32_t rf_b_sel_fwd_id;
    // branching
    uint32_t bc_uns_id;
    uint32_t bc_a_sel_fwd_id;
    uint32_t bcs_b_sel_fwd_id;
    // alu
    uint32_t alu_a_sel_fwd_id;
    uint32_t alu_b_sel_fwd_id;
    uint32_t alu_op_sel_id;
    // dmem
    uint32_t store_mask_id;
    uint32_t dmem_en_id;
    uint32_t load_sm_en_id;
    // writeback
    uint32_t wb_sel_id;
} ctrl_intf_t;

typedef struct ctrl_intf_private_t
{
    uint32_t opc7_id;
    uint32_t funct3_id;
    uint32_t funct7_id;
    uint32_t funct3_ex;
    bool branch_inst_id;
    bool jump_inst_id;
} ctrl_intf_private_t;

typedef struct sys_intf_t
{
    bool rst;

} sys_intf_t;

typedef struct seq_id_ex_intf_t
{
    //logic_t rf_read_op_a;
    uint32_t dummy;

} seq_id_ex_intf_t;