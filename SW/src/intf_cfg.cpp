#include "../include/intf_cfg.h"

void intf_cfg::init(seq_queue *q, logic_init_cfg_t *logic_init_cfg, uint32_t loop)
{
    uint32_t i = 0u;
    logic_t *reg;
    for (; i < loop; i++)
    {
        reg = new logic_t(q, logic_init_cfg[i].init, logic_init_cfg[i].id);
        reg->connect_rst(logic_init_cfg[i].reset);
        reg->connect_en(logic_init_cfg[i].enable);
        reg->connect_in(logic_init_cfg[i].input);
        reg->connect_out(logic_init_cfg[i].output);
        LOG("Logic created; ID: " << logic_init_cfg[i].id);
        LOG("connected input addr: " << logic_init_cfg[i].input);
    }
}
#ifndef MULTI_LOGIC
void intf_cfg::init_sys(seq_queue *q, sys_intf_t *sys_intf)
{
    logic_init_cfg_t sys[SYS_SIZE] = {
        {"rst_seq_id", 1, &sys_intf->rst, &unsued_ture, &sys_intf->rst_seq_d1, &sys_intf->rst_seq_id},
        {"rst_seq_ex", 1, &sys_intf->rst, &unsued_ture, &sys_intf->rst_seq_d2, &sys_intf->rst_seq_ex},
        {"rst_seq_mem", 1, &sys_intf->rst, &unsued_ture, &sys_intf->rst_seq_d3, &sys_intf->rst_seq_mem}
    };
    init(q, sys, SYS_SIZE);
}

void intf_cfg::init_if_id(seq_queue *q, sys_intf_t *sys_intf, if_intf_t *if_intf, id_intf_t *id_intf, 
    uint32_t *imem_dout)
{
    logic_init_cfg_t if_id[IF_ID_SIZE] = {
        {"stall_if_d", 1, &sys_intf->rst, &unsued_ture, &id_intf->stall_if, &id_intf->stall_if_d},
//FIXME {"nx_pc", RESET_VECTOR, &sys_intf->rst, &id_intf->dec_pc_we_if, &if_intf->pc, &id_intf->nx_pc},
        {"nx_pc", RESET_VECTOR, &sys_intf->rst, &unsued_ture, &if_intf->pc, &id_intf->nx_pc},
        {"imem", 0, &unsued_false, &unsued_ture, imem_dout, &id_intf->inst_id}
    };
    init(q, if_id, IF_ID_SIZE);
}

void intf_cfg::init_id_ex(seq_queue *q, sys_intf_t *sys_intf, id_intf_t *id_intf, ex_intf_t *ex_intf)
{
    logic_init_cfg_t id_ex[ID_EX_SIZE] = {
        {"inst_ex", NOP, &sys_intf->rst, &id_intf->clear_ex, &id_intf->inst_id, &ex_intf->inst_ex},
        {"funct3_ex", 0, &sys_intf->rst, &id_intf->clear_ex, &id_intf->funct3_id, &ex_intf->funct3_ex},
        {"rs1_addr_ex", 0, &sys_intf->rst, &id_intf->clear_ex, &id_intf->rs1_addr_id, &ex_intf->rs1_addr_ex},
        {"rs2_addr_ex", 0, &sys_intf->rst, &id_intf->clear_ex, &id_intf->rs2_addr_id, &ex_intf->rs2_addr_ex},
        {"rd_addr_ex", 0, &sys_intf->rst, &id_intf->clear_ex, &id_intf->rd_addr_id, &ex_intf->rd_addr_ex},
        {"rd_we_ex", 0, &sys_intf->rst, &id_intf->clear_ex, &id_intf->dec_rd_we_id, &ex_intf->rd_we_ex},
        {"store_inst_ex", 0, &sys_intf->rst, &id_intf->clear_ex, &id_intf->dec_store_inst_id, &ex_intf->store_inst_ex},
        {"branch_inst_ex", 0, &sys_intf->rst, &id_intf->clear_ex, &id_intf->dec_branch_inst_id, &ex_intf->branch_inst_ex},
        {"jump_inst_ex", 0, &sys_intf->rst, &id_intf->clear_ex, &id_intf->dec_jump_inst_id, &ex_intf->jump_inst_ex},
    };
    init(q, id_ex, ID_EX_SIZE);
}

void intf_cfg::init_ex_mem(seq_queue *q, sys_intf_t *sys_intf, id_intf_t *id_intf, ex_intf_t *ex_intf, mem_intf_t *mem_intf)
{
    logic_init_cfg_t ex_mem[EX_MEM_SIZE] = {
        {"inst_mem", NOP, &sys_intf->rst, &id_intf->clear_mem, &ex_intf->inst_ex, &mem_intf->inst_mem},
        {"funct3_mem", 0, &sys_intf->rst, &id_intf->clear_mem, &ex_intf->funct3_ex, &mem_intf->funct3_mem},
        {"rs1_addr_mem", 0, &sys_intf->rst, &id_intf->clear_mem, &ex_intf->rs1_addr_ex, &mem_intf->rs1_addr_mem},
        {"rs2_addr_mem", 0, &sys_intf->rst, &id_intf->clear_mem, &ex_intf->rs2_addr_ex, &mem_intf->rs2_addr_mem},
        {"rd_addr_mem", 0, &sys_intf->rst, &id_intf->clear_mem, &ex_intf->rd_addr_ex, &mem_intf->rd_addr_mem},
        {"rd_we_mem", 0, &sys_intf->rst, &id_intf->clear_mem, &ex_intf->rd_we_ex, &mem_intf->rd_we_mem}
    };
    init(q, ex_mem, EX_MEM_SIZE);
}

#else // !MULTI_LOGIC

void intf_cfg::init_pipelines(seq_queue *q, sys_intf_t *sys_intf, if_intf_t *if_intf, id_intf_t *id_intf,
    ex_intf_t *ex_intf, mem_intf_t *mem_intf)
{
    logic_init_cfg_t pipelines_cfg[4] = {
        {"IF_ID", NOP, &sys_intf->rst, &id_intf->clear_if, &unsued_ture},
        {"ID_EX", NOP, &sys_intf->rst, &id_intf->clear_id, &unsued_ture},
        {"EX_MEM", NOP, &sys_intf->rst, &id_intf->clear_ex, &unsued_ture},
        {"MEM_WB", NOP, &sys_intf->rst, &id_intf->clear_mem, &unsued_ture},
    };
}

#endif