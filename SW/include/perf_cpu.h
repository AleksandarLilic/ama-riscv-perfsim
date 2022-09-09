#pragma once

#include "defines.h"
#include "intf.h"

class perf_cpu
{
public:
    static void update(sys_intf_t *sys_intf, if_intf_t *if_intf, id_intf_t *id_intf, wb_intf_t *wb_intf);
    static void collect_data(uint32_t *perf_array);
    static void status_log(uint32_t *perf_array);
    static void update_dd(sys_intf_t *sys_intf, uint32_t dd_rs1_ex, uint32_t dd_rs2_ex,
        uint32_t dd_rs1_mem, uint32_t dd_rs2_mem);
private:
    static uint32_t cycle_cnt;
    static uint32_t inst_cnt;
    static uint32_t hw_nop;
    static uint32_t tot_nop;

    static uint32_t dd_rs1_ex_cnt;
    static uint32_t dd_rs2_ex_cnt;
    static uint32_t dd_rs1_mem_cnt;
    static uint32_t dd_rs2_mem_cnt;
};
