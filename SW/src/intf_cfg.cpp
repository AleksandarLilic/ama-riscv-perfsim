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
    if_intf_t *if_intf, id_intf_t *id_intf, ex_intf_t *ex_intf, mem_intf_t *mem_intf, 
    uint32_t *imem_dout, uint32_t *dmem_dout)
{
    logic_init_cfg_t regs_cfg[CFG_REGS] = {
     //      {"ID", <reset_sig>, <clear_sig>, <enable_sig>},
     /* 0 */ {"SYS", &sys_intf->rst, &unused_false, &unused_true},
     /* 1 */ {"PC", &sys_intf->rst, &unused_false, &id_intf->dec_pc_we_if},   // FIXME: use pc_we_if
     /* 1 */ //{"PC", &sys_intf->rst, &unused_false, &unused_true},
     /* 2 */ {"stall_if_d", &sys_intf->rst, &unused_false, &unused_true},
     /* 3 */ {"IMEM", &unused_false, &unused_false, &unused_true},
     /* 4 */ {"ID_EX", &sys_intf->rst, &id_intf->clear_id_ex, &unused_true},
     /* 5 */ {"EX_MEM", &sys_intf->rst, &id_intf->clear_ex_mem, &unused_true},
     /* 6 */ {"DMEM", &unused_false, &unused_false, &unused_true},
     /* 7 */ {"MEM_WB", &sys_intf->rst, &id_intf->clear_mem_wb, &unused_true},
     /* 8 */ {"reg_file", &sys_intf->rst, &unused_false, &unused_true}
    };

    init(q, regs_cfg, CFG_REGS);

    // FIXME: ordering is important (from regs_cfg); consider maps so it can be referenced by name
    init_sys(logic_ptr[0], sys_intf);
    init_if_id(logic_ptr[1], logic_ptr[2], logic_ptr[3], if_intf, id_intf, imem_dout);
    init_id_ex(logic_ptr[4], id_intf, ex_intf);
    init_ex_mem(logic_ptr[5], logic_ptr[6], id_intf, ex_intf, mem_intf, dmem_dout);
    init_reg_file(logic_ptr[8], reg_file_intf);
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
    logic_ptr_pc->connect_port("nx_pc", RESET_VECTOR, &if_intf->pc_inc4, &id_intf->nx_pc);
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
    logic_ptr->connect_port("rf_data_a_ex", 0, &id_intf->rf_data_a_fwd, &ex_intf->rf_data_a_ex);
    logic_ptr->connect_port("rf_data_b_ex", 0, &id_intf->rf_data_b_fwd, &ex_intf->rf_data_b_ex);
    logic_ptr->connect_port("rd_we_ex", 0, &id_intf->dec_rd_we_id, &ex_intf->rd_we_ex);
    logic_ptr->connect_port("rd_addr_ex", 0, &id_intf->rd_addr_id, &ex_intf->rd_addr_ex);
    logic_ptr->connect_port("imm_gen_out_ex", 0, &id_intf->imm_gen_out, &ex_intf->imm_gen_out_ex);
    
    logic_ptr->connect_port("alu_a_sel_ex", 0, &id_intf->of_alu_a_sel_fwd_id, &ex_intf->alu_a_sel_ex);
    logic_ptr->connect_port("alu_b_sel_ex", 0, &id_intf->of_alu_b_sel_fwd_id, &ex_intf->alu_b_sel_ex);
    logic_ptr->connect_port("alu_op_sel_ex", 0, &id_intf->dec_alu_op_sel_id, &ex_intf->alu_op_sel_ex);
    logic_ptr->connect_port("bc_a_sel_ex", 0, &id_intf->of_bc_a_sel_fwd_id, &ex_intf->bc_a_sel_ex);
    logic_ptr->connect_port("bcs_b_sel_ex", 0, &id_intf->of_bcs_b_sel_fwd_id, &ex_intf->bcs_b_sel_ex);
    logic_ptr->connect_port("bc_uns_ex", 0, &id_intf->dec_bc_uns_id, &ex_intf->bc_uns_ex);
    
    logic_ptr->connect_port("store_inst_ex", 0, &id_intf->dec_store_inst_id, &ex_intf->store_inst_ex);
    logic_ptr->connect_port("branch_inst_ex", 0, &id_intf->dec_branch_inst_id, &ex_intf->branch_inst_ex);
    logic_ptr->connect_port("jump_inst_ex", 0, &id_intf->dec_jump_inst_id, &ex_intf->jump_inst_ex);
    
    logic_ptr->connect_port("dmem_en_id", 0, &id_intf->dec_dmem_en_id, &ex_intf->dmem_en_ex);
    logic_ptr->connect_port("dmem_we_id", 0, &id_intf->dec_store_mask_ex, &ex_intf->dmem_we_ex);

    logic_ptr->connect_port("load_sm_en_ex", 0, &id_intf->dec_load_sm_en_id, &ex_intf->load_sm_en_ex);
    logic_ptr->connect_port("wb_sel_ex", 0, &id_intf->dec_wb_sel_id, &ex_intf->wb_sel_ex);
}

void intf_cfg::init_ex_mem(logic_t *logic_ptr, logic_t *logic_ptr_dmem, id_intf_t *id_intf, ex_intf_t *ex_intf,
    mem_intf_t *mem_intf, uint32_t *dmem_dout)
{
    logic_ptr->connect_port("inst_mem", NOP, &ex_intf->inst_ex, &mem_intf->inst_mem);
    logic_ptr->connect_port("pc_mem", 0, &ex_intf->pc_ex, &mem_intf->pc_mem);
    logic_ptr->connect_port("alu_mem", 0, &ex_intf->alu_out, &mem_intf->alu_mem);
    logic_ptr->connect_port("funct3_mem", 0, &ex_intf->funct3_ex, &mem_intf->funct3_mem);
    logic_ptr->connect_port("rs1_addr_mem", 0, &ex_intf->rs1_addr_ex, &mem_intf->rs1_addr_mem);
    logic_ptr->connect_port("rs2_addr_mem", 0, &ex_intf->rs2_addr_ex, &mem_intf->rs2_addr_mem);
    logic_ptr->connect_port("rd_addr_mem", 0, &ex_intf->rd_addr_ex, &mem_intf->rd_addr_mem);
    logic_ptr->connect_port("rd_we_mem", 0, &ex_intf->rd_we_ex, &mem_intf->rd_we_mem);
    
    logic_ptr_dmem->connect_port("dmem_dout", 0, dmem_dout, &mem_intf->dmem_dout);

    logic_ptr->connect_port("load_sm_en_mem", 0, &ex_intf->load_sm_en_ex, &mem_intf->load_sm_en_mem );
    logic_ptr->connect_port("wb_sel_mem", 0, &ex_intf->wb_sel_ex, &mem_intf->wb_sel_mem);
}

void intf_cfg::init_reg_file(logic_t *logic_ptr, reg_file_intf_t *reg_file_intf)
{
    for (uint32_t i = 1; i < reg_file_intf->in.size(); i++) {
        std::string s = "x" + std::to_string(i);
        logic_ptr->connect_port(s, 0, &reg_file_intf->in[i], &reg_file_intf->out[i]);
    }
}
