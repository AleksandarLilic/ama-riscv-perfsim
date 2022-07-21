#pragma once

#include "defines.h"
#include "intf.h"
#include "perf_cpu.h"

class op_fwd
{
private:
    sys_intf_t *sys_intf;
    id_intf_t *id_intf;
    ex_intf_t *ex_intf;
    mem_intf_t *mem_intf;

private:
    bool dd_rs1_ex = 0;
    bool dd_rs2_ex = 0;
    bool dd_rs1_mem = 0;
    bool dd_rs2_mem = 0;

public:
    op_fwd() = delete;
    op_fwd(sys_intf_t *sys_intf, id_intf_t *id_intf, ex_intf_t *ex_intf, mem_intf_t *mem_intf);
    void update();

private:
    void dependency_detection(id_intf_t *id_intf, ex_intf_t *ex_intf, mem_intf_t *mem_intf);
    void op_fwd_rf(id_intf_t *id_intf);
    void op_fwd_bcs(id_intf_t *id_intf);
    void op_fwd_alu(id_intf_t *id_intf);
};
