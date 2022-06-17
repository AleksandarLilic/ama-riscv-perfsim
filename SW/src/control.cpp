#include "../include/control.h"

//extern void imemc_print_ex();
control::control(sys_intf_t *sys_intf, if_intf_t *if_intf, id_intf_t *id_intf,
    ex_intf_t *ex_intf, mem_intf_t *mem_intf)
{
    this->sys_intf = sys_intf;
    this->if_intf = if_intf;
    this->id_intf = id_intf;
    this->ex_intf = ex_intf;
    this->mem_intf = mem_intf;
}

void control::update()
{
    id_intf->stall_if = sys_intf->rst;
    if (id_intf->stall_if_d)
        id_intf->inst_id = NOP;
    update(sys_intf, id_intf, ex_intf, mem_intf);
}

void control::update(sys_intf_t *sys_intf, id_intf_t *id_intf, ex_intf_t *ex_intf,
    mem_intf_t *mem_intf)
{
    LOG("--- control called");
    id_intf->opc7_id = inst_field::opc7(id_intf->inst_id);
    id_intf->funct3_id = inst_field::funct3(id_intf->inst_id);
    id_intf->funct7_id = inst_field::funct7(id_intf->inst_id);
    id_intf->rs1_addr_id = inst_field::rs1_addr(id_intf->inst_id);
    id_intf->rs2_addr_id = inst_field::rs2_addr(id_intf->inst_id);
    id_intf->rd_addr_id = inst_field::rd_addr(id_intf->inst_id);
    // from ex
    //id_intf->funct3_ex = inst_field::funct3(ex_intf->inst_ex);
    //id_intf->rs1_addr_ex = inst_field::rs1_addr(ex_intf->inst_ex);
    //id_intf->rs2_addr_ex = inst_field::rs2_addr(ex_intf->inst_ex);
    //id_intf->rd_addr_ex = inst_field::rd_addr(ex_intf->inst_ex);
    // from mem
    //id_intf->rs1_addr_mem = inst_field::rs1_addr(mem_intf->inst_mem);
    //id_intf->rs2_addr_mem = inst_field::rs2_addr(mem_intf->inst_mem);
    //id_intf->rd_addr_mem = inst_field::rd_addr(mem_intf->inst_mem);
    //LOG("ctrl, funct7 = " << std::hex << (id_intf->funct7_id) << std::dec);

    decoder.update(sys_intf, id_intf);
    op_fwd.update(sys_intf, id_intf, ex_intf, mem_intf);
    branch_resolution(sys_intf, id_intf, ex_intf);
    store_mask(id_intf, ex_intf);

    //id_intf->dec_pc_we_if = id_intf->dec_pc_we_if && !id_intf->stall_if;
    pipeline_ctrl(sys_intf, id_intf);

    // /* new func: */ control_store_mask(id_intf);


    // ------- for test only, imitate sequential assignment
    /*id_intf->in_inst_mem = id_intf->in_inst_ex;
    id_intf->rd_we_mem = id_intf->rd_we_ex;

    id_intf->in_inst_ex = id_intf->in_inst_id;
    id_intf->rd_we_ex = id_intf->dec_rd_we_id;
    id_intf->dec_store_inst_ex = id_intf->dec_store_inst_id;*/

    // id_intf->dec_branch_inst_ex = id_intf->dec_branch_inst_id;
    // id_intf->dec_jump_inst_ex = id_intf->dec_jump_inst_id;
}

void control::pipeline_ctrl(sys_intf_t *sys_intf, id_intf_t *id_intf)
{
    LOG("--- pipeline control called");

    id_intf->stall_if = id_intf->dec_branch_inst_id | 
        id_intf->dec_jump_inst_id /* ||
        dut_m_dd_bubble_ex*/;
    
    id_intf->clear_if = id_intf->dec_branch_inst_id |
        id_intf->dec_jump_inst_id;

    // FIXME: inverted logic due to logic_t enable issue
    id_intf->clear_id = !sys_intf->rst_seq_id /* || dut_m_dd_bubble_ex*/;
    id_intf->clear_ex = !sys_intf->rst_seq_ex;
    id_intf->clear_mem = !sys_intf->rst_seq_mem;

    //LOG("id_intf->clear_id: " << id_intf->clear_id);
    //LOG("id_intf->clear_ex: " << id_intf->clear_ex);
    //LOG("id_intf->clear_mem: " << id_intf->clear_mem);
    
    id_intf->dec_pc_we_if = id_intf->dec_pc_we_if & (~id_intf->stall_if);
}

void control::branch_resolution(sys_intf_t *sys_intf, id_intf_t *id_intf, ex_intf_t *ex_intf)
{
    LOG("--- branch resolution called");
    LOG("branch inst ex: " << ex_intf->branch_inst_ex);

    // this function works on data in the ex stage

    br_sel_t branch_type = br_sel_t(((ex_intf->funct3_ex & 0b100) >> 1) |
        (ex_intf->funct3_ex & 0b001));
    uint32_t branch_taken = 0;

    switch (branch_type) {
    case br_sel_t::beq: branch_taken = ex_intf->bc_a_eq_b; break;
    case br_sel_t::bne: branch_taken = ~ex_intf->bc_a_eq_b; break;
    case br_sel_t::blt: branch_taken = ex_intf->bc_a_lt_b; break;
    case br_sel_t::bge: branch_taken = ex_intf->bc_a_eq_b | ~ex_intf->bc_a_lt_b; break;
    default: LOGW("Branch Resolution default case");
    }

    branch_taken &= ~sys_intf->rst; // if in reset, override branch taken
    branch_taken &= ex_intf->branch_inst_ex;  // only taken if it actually is branch
    if (branch_taken || ex_intf->jump_inst_ex) id_intf->dec_pc_sel_if = uint32_t(pc_sel_t::alu);
    
    LOG("branch taken: " << branch_taken);
    LOG("dec_sel: " << static_cast<int>(id_intf->dec_pc_sel_if));
}

void control::store_mask(id_intf_t *id_intf, ex_intf_t *ex_intf)
{
    // dummy, will cause unaligned value issue 
    // id_intf->alu_out = 3;

    uint32_t mask_width = ex_intf->funct3_ex & 0b11;
    uint32_t mask_offset = ex_intf->alu_out & 0b11;
    
    // uint32_t b0 = 1;
    // uint32_t b1 = ((mask_width & 0b1) << 1) | (mask_width & 0b10);
    // uint32_t b2 = (mask_width & 0b10) << 1;
    // uint32_t b3 = (mask_width & 0b10) << 2;
    // uint32_t mask = (b3 | b2 | b1 | b0) << 4;

    uint32_t mask = 1;
    mask = ~(~mask << mask_width);  // fill with 1s from right
    mask = mask | ((mask << 1) & 0x8);  // add bit[3] if word
    mask = mask << 4;   // prepare for potential offset
    // byte 0001'0000
    // half 0011'0000
    // word 1111'0000
    uint32_t inst_en = ~(~ex_intf->store_inst_ex << 3); // fill with 1s from right

    id_intf->dec_store_mask_ex = inst_en & (mask >> (4 - mask_offset));
    if (ex_intf->store_inst_ex) {
        LOG("id_intf->dec_store_mask_id: " << id_intf->dec_store_mask_ex);
        //LOG("b3: " << b3 << "; b2: " << b2 << "; b1: " << b1 << "; b0: " << b0);
    }
}