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

public:
    void update(ctrl_intf_t *ctrl_intf, sys_intf_t *sys_intf);

private:
    void dependency_detection(ctrl_intf_t *ctrl_intf);
    void op_fwd_rf(ctrl_intf_t *ctrl_intf);
    void op_fwd_bcs(ctrl_intf_t *ctrl_intf);
    void op_fwd_alu(ctrl_intf_t *ctrl_intf);
};


