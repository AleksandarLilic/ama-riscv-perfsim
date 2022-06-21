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
        {"rst_seq_id_ex", 1, &sys_intf->rst, &unused_true, &sys_intf->rst_seq_d1, &sys_intf->rst_seq_id_ex},
        {"rst_seq_ex_mem", 1, &sys_intf->rst, &unused_true, &sys_intf->rst_seq_d2, &sys_intf->rst_seq_ex_mem},
        {"rst_seq_mem_wb", 1, &sys_intf->rst, &unused_true, &sys_intf->rst_seq_d3, &sys_intf->rst_seq_mem_wb}
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
    ex_intf_t *ex_intf, mem_intf_t *mem_intf, uint32_t *imem_dout)
{
    logic_init_cfg_t regs_cfg[CFG_REGS] = {
     //      {"ID", <reset_sig>, <clear_sig>, <enable_sig>},
     /* 0 */ {"SYS", &sys_intf->rst, &unused_false, &unused_true},
     /* 1 */ //{"PC", &sys_intf->rst, &unused_false, &id_intf->dec_pc_we_if},
     /* 1 */ {"PC", &sys_intf->rst, &unused_false, &unused_true},
     /* 2 */ {"stall_if_d", &sys_intf->rst, &unused_false, &unused_true},
     /* 3 */ {"IMEM", &unused_false, &unused_false, &unused_true},
     /* 4 */ {"ID_EX", &sys_intf->rst, &id_intf->clear_id_ex, &unused_true},
     /* 5 */ {"EX_MEM", &sys_intf->rst, &id_intf->clear_ex_mem, &unused_true},
     /* 6 */ {"MEM_WB", &sys_intf->rst, &id_intf->clear_mem_wb, &unused_true}
    };
    init(q, regs_cfg, CFG_REGS);

    // FIXME: ordering is important (from regs_cfg); consider maps so it can be referenced by name
    init_sys(logic_ptr[0], sys_intf);
    init_if_id(logic_ptr[1], logic_ptr[2], logic_ptr[3], sys_intf, if_intf, id_intf, imem_dout);
    init_id_ex(logic_ptr[4], sys_intf, id_intf, ex_intf);
    init_ex_mem(logic_ptr[5], sys_intf, id_intf, ex_intf, mem_intf);
}

void intf_cfg::init_sys(logic_t *logic_ptr, sys_intf_t *sys_intf)
{
    //                     ("ID", <reset_val>, <input>, <output>)
    logic_ptr->connect_port("rst_seq_id_ex", 1, &sys_intf->rst_seq_d1, &sys_intf->rst_seq_id_ex);
    logic_ptr->connect_port("rst_seq_ex_mem", 1, &sys_intf->rst_seq_d2, &sys_intf->rst_seq_ex_mem);
    logic_ptr->connect_port("rst_seq_mem_wb", 1, &sys_intf->rst_seq_d3, &sys_intf->rst_seq_mem_wb);
}

void intf_cfg::init_if_id(logic_t *logic_ptr_pc, logic_t *logic_ptr_stall, logic_t *logic_ptr_imem, 
    sys_intf_t *sys_intf, if_intf_t *if_intf, id_intf_t *id_intf, uint32_t *imem_dout)
{
    //                         ("ID", <reset_val>, <input>, <output>)
    logic_ptr_pc->connect_port("nx_pc", RESET_VECTOR, &if_intf->pc, &id_intf->nx_pc);
    logic_ptr_stall->connect_port("stall_if_id_d", 1, &id_intf->stall_if_id, &id_intf->stall_if_id_d);
    logic_ptr_imem->connect_port("imem", 0, imem_dout, &id_intf->inst_id);
}

void intf_cfg::init_id_ex(logic_t *logic_ptr, sys_intf_t *sys_intf, id_intf_t *id_intf, ex_intf_t *ex_intf)
{
    logic_ptr->connect_port("inst_ex", NOP, &id_intf->inst_id, &ex_intf->inst_ex);
    logic_ptr->connect_port("funct3_ex", 0, &id_intf->funct3_id, &ex_intf->funct3_ex);
    logic_ptr->connect_port("rs1_addr_ex", 0, &id_intf->rs1_addr_id, &ex_intf->rs1_addr_ex);
    logic_ptr->connect_port("rs2_addr_ex", 0, &id_intf->rs2_addr_id, &ex_intf->rs2_addr_ex);
    logic_ptr->connect_port("rd_addr_ex", 0, &id_intf->rd_addr_id, &ex_intf->rd_addr_ex);
    logic_ptr->connect_port("rd_we_ex", 0, &id_intf->dec_rd_we_id, &ex_intf->rd_we_ex);
    logic_ptr->connect_port("store_inst_ex", 0, &id_intf->dec_store_inst_id, &ex_intf->store_inst_ex);
    logic_ptr->connect_port("branch_inst_ex", 0, &id_intf->dec_branch_inst_id, &ex_intf->branch_inst_ex);
    logic_ptr->connect_port("jump_inst_ex", 0, &id_intf->dec_jump_inst_id, &ex_intf->jump_inst_ex);
}

void intf_cfg::init_ex_mem(logic_t *logic_ptr, sys_intf_t *sys_intf, id_intf_t *id_intf, ex_intf_t *ex_intf, 
    mem_intf_t *mem_intf)
{
    logic_ptr->connect_port("inst_mem", NOP, &ex_intf->inst_ex, &mem_intf->inst_mem);
    logic_ptr->connect_port("funct3_mem", 0, &ex_intf->funct3_ex, &mem_intf->funct3_mem);
    logic_ptr->connect_port("rs1_addr_mem", 0, &ex_intf->rs1_addr_ex, &mem_intf->rs1_addr_mem);
    logic_ptr->connect_port("rs2_addr_mem", 0, &ex_intf->rs2_addr_ex, &mem_intf->rs2_addr_mem);
    logic_ptr->connect_port("rd_addr_mem", 0, &ex_intf->rd_addr_ex, &mem_intf->rd_addr_mem);
    logic_ptr->connect_port("rd_we_mem", 0, &ex_intf->rd_we_ex, &mem_intf->rd_we_mem);
}

#endif