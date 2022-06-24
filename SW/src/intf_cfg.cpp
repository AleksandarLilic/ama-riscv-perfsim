#include "../include/intf_cfg.h"

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

void intf_cfg::init_regs(seq_queue *q, sys_intf_t *sys_intf, reg_file_intf_t *reg_file_intf, 
    if_intf_t *if_intf, id_intf_t *id_intf, ex_intf_t *ex_intf, mem_intf_t *mem_intf, uint32_t *imem_dout)
{
    logic_init_cfg_t regs_cfg[CFG_REGS] = {
     //      {"ID", <reset_sig>, <clear_sig>, <enable_sig>},
     /* 0 */ {"SYS", &sys_intf->rst, &unused_false, &unused_true},
     /* 1 */ //{"PC", &sys_intf->rst, &unused_false, &id_intf->dec_pc_we_if},   // FIXME: use pc_we_if
     /* 1 */ {"PC", &sys_intf->rst, &unused_false, &unused_true},
     /* 2 */ {"stall_if_d", &sys_intf->rst, &unused_false, &unused_true},
     /* 3 */ {"IMEM", &unused_false, &unused_false, &unused_true},
     /* 4 */ {"ID_EX", &sys_intf->rst, &id_intf->clear_id_ex, &unused_true},
     /* 5 */ {"EX_MEM", &sys_intf->rst, &id_intf->clear_ex_mem, &unused_true},
     /* 6 */ {"MEM_WB", &sys_intf->rst, &id_intf->clear_mem_wb, &unused_true},
     /* 7 */ {"reg_file", &sys_intf->rst, &unused_false, &unused_true}
    };

    init(q, regs_cfg, CFG_REGS);

    // FIXME: ordering is important (from regs_cfg); consider maps so it can be referenced by name
    init_sys(logic_ptr[0], sys_intf);
    init_if_id(logic_ptr[1], logic_ptr[2], logic_ptr[3], if_intf, id_intf, imem_dout);
    init_id_ex(logic_ptr[4], id_intf, ex_intf);
    init_ex_mem(logic_ptr[5], id_intf, ex_intf, mem_intf);
    init_reg_file(logic_ptr[7], reg_file_intf);
}

void intf_cfg::init_sys(logic_t *logic_ptr, sys_intf_t *sys_intf)
{
    //                     ("ID", <reset_val>, <input>, <output>)
    logic_ptr->connect_port("rst_seq_id_ex", 1, &sys_intf->rst_seq_d1, &sys_intf->rst_seq_id_ex);
    logic_ptr->connect_port("rst_seq_ex_mem", 1, &sys_intf->rst_seq_d2, &sys_intf->rst_seq_ex_mem);
    logic_ptr->connect_port("rst_seq_mem_wb", 1, &sys_intf->rst_seq_d3, &sys_intf->rst_seq_mem_wb);
}

void intf_cfg::init_if_id(logic_t *logic_ptr_pc, logic_t *logic_ptr_stall, logic_t *logic_ptr_imem, 
    if_intf_t *if_intf, id_intf_t *id_intf, uint32_t *imem_dout)
{
    logic_ptr_pc->connect_port("nx_pc", RESET_VECTOR, &if_intf->pc, &id_intf->nx_pc);
    logic_ptr_stall->connect_port("stall_if_id_d", 1, &id_intf->stall_if_id, &id_intf->stall_if_id_d);
    logic_ptr_imem->connect_port("imem", 0, imem_dout, &id_intf->inst_id);
}

void intf_cfg::init_id_ex(logic_t *logic_ptr, id_intf_t *id_intf, ex_intf_t *ex_intf)
{
    logic_ptr->connect_port("inst_ex", NOP, &id_intf->inst_id, &ex_intf->inst_ex);
    logic_ptr->connect_port("pc_ex", 0, &id_intf->nx_pc, &ex_intf->pc_ex);
    logic_ptr->connect_port("funct3_ex", 0, &id_intf->funct3_id, &ex_intf->funct3_ex);
    logic_ptr->connect_port("rs1_addr_ex", 0, &id_intf->rs1_addr_id, &ex_intf->rs1_addr_ex);
    logic_ptr->connect_port("rs2_addr_ex", 0, &id_intf->rs2_addr_id, &ex_intf->rs2_addr_ex);
    logic_ptr->connect_port("rd_addr_ex", 0, &id_intf->rd_addr_id, &ex_intf->rd_addr_ex);
    logic_ptr->connect_port("rd_we_ex", 0, &id_intf->dec_rd_we_id, &ex_intf->rd_we_ex);
    logic_ptr->connect_port("store_inst_ex", 0, &id_intf->dec_store_inst_id, &ex_intf->store_inst_ex);
    logic_ptr->connect_port("branch_inst_ex", 0, &id_intf->dec_branch_inst_id, &ex_intf->branch_inst_ex);
    logic_ptr->connect_port("jump_inst_ex", 0, &id_intf->dec_jump_inst_id, &ex_intf->jump_inst_ex);
    logic_ptr->connect_port("wb_sel_ex", 0, &id_intf->dec_wb_sel_id, &ex_intf->wb_sel_ex);
}

void intf_cfg::init_ex_mem(logic_t *logic_ptr, id_intf_t *id_intf, ex_intf_t *ex_intf, 
    mem_intf_t *mem_intf)
{
    logic_ptr->connect_port("inst_mem", NOP, &ex_intf->inst_ex, &mem_intf->inst_mem);
    logic_ptr->connect_port("pc_mem", 0, &ex_intf->pc_ex, &mem_intf->pc_mem);
    logic_ptr->connect_port("alu_mem", 0, &ex_intf->alu_out, &mem_intf->alu_mem);
    logic_ptr->connect_port("funct3_mem", 0, &ex_intf->funct3_ex, &mem_intf->funct3_mem);
    logic_ptr->connect_port("rs1_addr_mem", 0, &ex_intf->rs1_addr_ex, &mem_intf->rs1_addr_mem);
    logic_ptr->connect_port("rs2_addr_mem", 0, &ex_intf->rs2_addr_ex, &mem_intf->rs2_addr_mem);
    logic_ptr->connect_port("rd_addr_mem", 0, &ex_intf->rd_addr_ex, &mem_intf->rd_addr_mem);
    logic_ptr->connect_port("rd_we_mem", 0, &ex_intf->rd_we_ex, &mem_intf->rd_we_mem);
    logic_ptr->connect_port("wb_sel_mem", 0, &ex_intf->wb_sel_ex, &mem_intf->wb_sel_mem);
}

void intf_cfg::init_reg_file(logic_t *logic_ptr, reg_file_intf_t *reg_file_intf)
{
    logic_ptr->connect_port("r1", 0, &reg_file_intf->in_r1, &reg_file_intf->out_r1);
    logic_ptr->connect_port("r2", 0, &reg_file_intf->in_r2, &reg_file_intf->out_r2);
    logic_ptr->connect_port("r3", 0, &reg_file_intf->in_r3, &reg_file_intf->out_r3);
    logic_ptr->connect_port("r4", 0, &reg_file_intf->in_r4, &reg_file_intf->out_r4);
    logic_ptr->connect_port("r5", 0, &reg_file_intf->in_r5, &reg_file_intf->out_r5);
    logic_ptr->connect_port("r6", 0, &reg_file_intf->in_r6, &reg_file_intf->out_r6);
    logic_ptr->connect_port("r7", 0, &reg_file_intf->in_r7, &reg_file_intf->out_r7);
    logic_ptr->connect_port("r8", 0, &reg_file_intf->in_r8, &reg_file_intf->out_r8);
    logic_ptr->connect_port("r9", 0, &reg_file_intf->in_r9, &reg_file_intf->out_r9);
    logic_ptr->connect_port("r10", 0, &reg_file_intf->in_r10, &reg_file_intf->out_r10);
    logic_ptr->connect_port("r11", 0, &reg_file_intf->in_r11, &reg_file_intf->out_r11);
    logic_ptr->connect_port("r12", 0, &reg_file_intf->in_r12, &reg_file_intf->out_r12);
    logic_ptr->connect_port("r13", 0, &reg_file_intf->in_r13, &reg_file_intf->out_r13);
    logic_ptr->connect_port("r14", 0, &reg_file_intf->in_r14, &reg_file_intf->out_r14);
    logic_ptr->connect_port("r15", 0, &reg_file_intf->in_r15, &reg_file_intf->out_r15);
    logic_ptr->connect_port("r16", 0, &reg_file_intf->in_r16, &reg_file_intf->out_r16);
    logic_ptr->connect_port("r17", 0, &reg_file_intf->in_r17, &reg_file_intf->out_r17);
    logic_ptr->connect_port("r18", 0, &reg_file_intf->in_r18, &reg_file_intf->out_r18);
    logic_ptr->connect_port("r19", 0, &reg_file_intf->in_r19, &reg_file_intf->out_r19);
    logic_ptr->connect_port("r20", 0, &reg_file_intf->in_r20, &reg_file_intf->out_r20);
    logic_ptr->connect_port("r21", 0, &reg_file_intf->in_r21, &reg_file_intf->out_r21);
    logic_ptr->connect_port("r22", 0, &reg_file_intf->in_r22, &reg_file_intf->out_r22);
    logic_ptr->connect_port("r23", 0, &reg_file_intf->in_r23, &reg_file_intf->out_r23);
    logic_ptr->connect_port("r24", 0, &reg_file_intf->in_r24, &reg_file_intf->out_r24);
    logic_ptr->connect_port("r25", 0, &reg_file_intf->in_r25, &reg_file_intf->out_r25);
    logic_ptr->connect_port("r26", 0, &reg_file_intf->in_r26, &reg_file_intf->out_r26);
    logic_ptr->connect_port("r27", 0, &reg_file_intf->in_r27, &reg_file_intf->out_r27);
    logic_ptr->connect_port("r28", 0, &reg_file_intf->in_r28, &reg_file_intf->out_r28);
    logic_ptr->connect_port("r29", 0, &reg_file_intf->in_r29, &reg_file_intf->out_r29);
    logic_ptr->connect_port("r30", 0, &reg_file_intf->in_r30, &reg_file_intf->out_r30);
    logic_ptr->connect_port("r31", 0, &reg_file_intf->in_r31, &reg_file_intf->out_r31);
}
