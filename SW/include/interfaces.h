#pragma once

#include "defines.h"
#include "logic_t.h"

// MOVE ALL TO INTERFACE class, add as many IFs as needed

typedef struct ctrl_conn_t
{
    uint32_t inst_id;
    uint32_t alu_op_sel;
    uint32_t alu_a_sel;
    uint32_t alu_b_sel;

    logic_t test_logic_type;


} ctrl_intfc_t;

typedef struct sys_conn_t
{
    bool rst;

} sys_intfc_t;

typedef struct seq_id_ex_intfc_t
{
    //logic_t rf_read_op_a;
    uint32_t dummy;

} seq_id_ex_intfc_t;