#pragma once

#include "defines.h"
#include "inst_field.h"
#include "intf.h"
#include "seq_queue.h"
#include "logic_t.h"
#include "cl.h"
#include <array>

class op_fwd
{
private:
    bool dd_rs1_ex;
    bool dd_rs2_ex;
    bool dd_rs1_mem;
    bool dd_rs2_mem;
    sys_intf_t *sys_intf;
    id_intf_t *id_intf;
    ex_intf_t *ex_intf;
    mem_intf_t *mem_intf;

public:
    void update();
    op_fwd() = delete;
    op_fwd(sys_intf_t *sys_intf, id_intf_t *id_intf, ex_intf_t *ex_intf, mem_intf_t *mem_intf);


private:
    void dependency_detection(id_intf_t *id_intf, ex_intf_t *ex_intf, mem_intf_t *mem_intf);
    void op_fwd_rf(id_intf_t *id_intf);
    void op_fwd_bcs(id_intf_t *id_intf);
    void op_fwd_alu(id_intf_t *id_intf);
};


