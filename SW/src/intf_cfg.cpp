#include "../include/intf_cfg.h"

#ifndef MULTI_LOGIC
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
void intf_cfg::init_sys(seq_queue *q, sys_intf_t *sys_intf)
{
    logic_init_cfg_t sys[SYS_SIZE] = {
        {"rst_seq_id", 1, &sys_intf->rst, &unused_true, &sys_intf->rst_seq_d1, &sys_intf->rst_seq_id},
        {"rst_seq_ex", 1, &sys_intf->rst, &unused_true, &sys_intf->rst_seq_d2, &sys_intf->rst_seq_ex},
        {"rst_seq_mem", 1, &sys_intf->rst, &unused_true, &sys_intf->rst_seq_d3, &sys_intf->rst_seq_mem}
    };
    init(q, sys, SYS_SIZE);
}

void intf_cfg::init_if_id(seq_queue *q, sys_intf_t *sys_intf, if_intf_t *if_intf, id_intf_t *id_intf, 
    uint32_t *imem_dout)
{
    logic_init_cfg_t if_id[IF_ID_SIZE] = {
        {"stall_if_d", 1, &sys_intf->rst, &unused_true, &id_intf->stall_if, &id_intf->stall_if_d},
//FIXME {"nx_pc", RESET_VECTOR, &sys_intf->rst, &id_intf->dec_pc_we_if, &if_intf->pc, &id_intf->nx_pc},
        {"nx_pc", RESET_VECTOR, &sys_intf->rst, &unused_true, &if_intf->pc, &id_intf->nx_pc},
        {"imem", 0, &unused_false, &unused_true, imem_dout, &id_intf->inst_id}
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
void intf_cfg::init(seq_queue *q, logic_init_cfg_t *logic_init_cfg, uint32_t loop)
{
    uint32_t i = 0u;
    logic_t *reg;
    for (; i < loop; i++)
    {
        reg = new logic_t(q, logic_init_cfg[i].id);
        reg->connect_rst(logic_init_cfg[i].reset);
        reg->connect_clr(logic_init_cfg[i].clear);
        reg->connect_en(logic_init_cfg[i].enable);
        LOG("Logic created; ID: " << logic_init_cfg[i].id);
        logic_ptr.push_back(reg);
    }
}

void intf_cfg::init_regs(seq_queue *q, sys_intf_t *sys_intf, if_intf_t *if_intf, id_intf_t *id_intf,
    ex_intf_t *ex_intf, mem_intf_t *mem_intf)
{
    logic_init_cfg_t regs_cfg[CFG_REGS] = {
     // {"ID", <reset_val>, <reset_sig>, <clear_sig>, <enable_sig>},
        {"SYS", &sys_intf->rst, &unused_false, &unused_true},
        {"IF_ID", &sys_intf->rst, &id_intf->clear_if, &unused_true},
        {"ID_EX", &sys_intf->rst, &id_intf->clear_id, &unused_true},
        {"EX_MEM", &sys_intf->rst, &id_intf->clear_ex, &unused_true},
        {"MEM_WB", &sys_intf->rst, &id_intf->clear_mem, &unused_true}
    };
    init(q, regs_cfg, CFG_REGS);

    // FIXME: ordering is important (from regs_cfg); consider maps so it can be referenced by name
    init_sys(logic_ptr[0], sys_intf);
}

void intf_cfg::init_sys(logic_t *logic_ptr, sys_intf_t *sys_intf)
{
    logic_ptr->connect_port("rst_seq_id", 1, &sys_intf->rst_seq_d1, &sys_intf->rst_seq_id);
    logic_ptr->connect_port("rst_seq_ex", 1, &sys_intf->rst_seq_d2, &sys_intf->rst_seq_ex);
    logic_ptr->connect_port("rst_seq_mem", 1, &sys_intf->rst_seq_d3, &sys_intf->rst_seq_mem);
}

#endif