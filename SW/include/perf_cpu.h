#pragma once

#include "defines.h"
#include "intf.h"

class perf_cpu
{
public:
    static void update(sys_intf_t *sys_intf, if_intf_t *if_intf, wb_intf_t *wb_intf);
    static void status_log();
private:
    static uint32_t cycle_cnt;
    static uint32_t inst_cnt;

    //void cnt_cycle();
    //void cnt_inst();
    //void cnt_inst_update();
};