#include "../include/perf_cpu.h"

uint32_t perf_cpu::cycle_cnt = 0;
uint32_t perf_cpu::inst_cnt = 0;
uint32_t perf_cpu::hw_nop = 0;
uint32_t perf_cpu::tot_nop = 0;

uint32_t perf_cpu::dd_rs1_ex_cnt = 0;
uint32_t perf_cpu::dd_rs2_ex_cnt = 0;
uint32_t perf_cpu::dd_rs1_mem_cnt = 0;
uint32_t perf_cpu::dd_rs2_mem_cnt = 0;

void perf_cpu::update(sys_intf_t *sys_intf, if_intf_t *if_intf, id_intf_t *id_intf, wb_intf_t *wb_intf)
{
    bool inst_wb_nop_or_clear = (!sys_intf->rst) && ((wb_intf->inst_wb == NOP) || (wb_intf->inst_wb == 0));
    bool inst_hw_insterted_nop = id_intf->stall_if_id || sys_intf->rst_seq_mem_wb;

    //cycle_cnt = (sys_intf->rst) ? 0 : cycle_cnt + 1;
    inst_cnt = (sys_intf->rst) ? 0 : inst_cnt + uint32_t(!inst_wb_nop_or_clear);
    tot_nop = (sys_intf->rst) ? 0 : tot_nop + uint32_t(inst_wb_nop_or_clear);
    hw_nop = (sys_intf->rst) ? 0 : hw_nop + uint32_t(inst_hw_insterted_nop);
}

void perf_cpu::update_clk()
{
    cycle_cnt++;
}

void perf_cpu::reset_clk()
{
    cycle_cnt = 0;
}

void perf_cpu::update_dd(sys_intf_t *sys_intf, uint32_t dd_rs1_ex, uint32_t dd_rs2_ex, uint32_t dd_rs1_mem, uint32_t dd_rs2_mem)
{
    dd_rs1_ex_cnt = (sys_intf->rst) ? 0 : dd_rs1_ex_cnt + uint32_t(dd_rs1_ex);
    dd_rs2_ex_cnt = (sys_intf->rst) ? 0 : dd_rs2_ex_cnt + uint32_t(dd_rs2_ex);
    dd_rs1_mem_cnt = (sys_intf->rst) ? 0 : dd_rs1_mem_cnt + uint32_t(dd_rs1_mem);
    dd_rs2_mem_cnt = (sys_intf->rst) ? 0 : dd_rs2_mem_cnt + uint32_t(dd_rs2_mem);
}

void perf_cpu::collect_data(uint32_t *perf_array)
{
    *(perf_array+0) = cycle_cnt;
    *(perf_array+1) = inst_cnt;
    *(perf_array+2) = tot_nop;
    *(perf_array+3) = hw_nop;
    *(perf_array+4) = dd_rs1_ex_cnt;
    *(perf_array+5) = dd_rs2_ex_cnt;
    *(perf_array+6) = dd_rs1_mem_cnt;
    *(perf_array+7) = dd_rs2_mem_cnt;
}

void perf_cpu::status_log(uint32_t *perf_array) 
{
    uint32_t l_cycle_cnt = *(perf_array+0);
    uint32_t l_inst_cnt = *(perf_array+1);
    uint32_t l_tot_nop = *(perf_array+2);
    uint32_t l_hw_nop = *(perf_array+3);
    uint32_t l_dd_rs1_ex_cnt = *(perf_array+4);
    uint32_t l_dd_rs2_ex_cnt = *(perf_array+5);
    uint32_t l_dd_rs1_mem_cnt = *(perf_array+6);
    uint32_t l_dd_rs2_mem_cnt = *(perf_array+7);

    float l_cpi = float(l_cycle_cnt) / float(l_inst_cnt);
    LOG_M("    Cycle_cnt: " << l_cycle_cnt);
    LOG_M("    Instruction_cnt: " << l_inst_cnt);
    LOG_M("    CPI: " << l_cpi);
    LOG_M("    Total NOPs: " << l_tot_nop);
    LOG_M("    HW NOPs: " << l_hw_nop);
    LOG_M("    Compiler NOPs: " << l_tot_nop - l_hw_nop);
    LOG_M("");
    LOG_M("    Dependency detection:");
    LOG_M("        On RS1 in EX stage: " << l_dd_rs1_ex_cnt);
    LOG_M("        On RS2 in EX stage: " << l_dd_rs2_ex_cnt);
    LOG_M("        On RS1 in MEM stage: " << l_dd_rs1_mem_cnt);
    LOG_M("        On RS2 in MEM stage: " << l_dd_rs2_mem_cnt);
}
