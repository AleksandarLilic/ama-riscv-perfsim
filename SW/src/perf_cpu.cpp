#include "../include/perf_cpu.h"

uint32_t perf_cpu::cycle_cnt = 0;
uint32_t perf_cpu::inst_cnt = 0;

void perf_cpu::update(sys_intf_t *sys_intf, if_intf_t *if_intf, wb_intf_t *wb_intf)
{
    bool inst_wb_nop_or_clear = 0;
    inst_wb_nop_or_clear = (!sys_intf->rst) && ((wb_intf->inst_wb == NOP) || (wb_intf->inst_wb == 0));

    cycle_cnt = (sys_intf->rst) ? 0 : cycle_cnt + 1;
    inst_cnt = (sys_intf->rst) ? 0 : inst_cnt + uint32_t(!inst_wb_nop_or_clear);
}

void perf_cpu::status_log() 
{
    float cpi = float(cycle_cnt) / float(inst_cnt);
    LOG("    Cycle_cnt: " << cycle_cnt);
    LOG("    Instruction_cnt: " << inst_cnt);
    LOG("    CPI: " << cpi);
}