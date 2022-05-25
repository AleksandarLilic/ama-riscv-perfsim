#pragma once

#include "defines.h"
#include "logic_t.h"


typedef struct ctrl_intf_t
{
    uint32_t inst_id;
    uint32_t alu_op_sel;
    uint32_t alu_a_sel;
    uint32_t alu_b_sel;

    logic_t alu_a_sel_id;

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