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
    id_intf->ctrl_intf.in_inst_id = id_intf->inst_id;   // FIXME: not a way to do it....
    update(&id_intf->ctrl_intf, sys_intf);
}

void control::update(ctrl_intf_t *ctrl_intf, sys_intf_t *sys_intf)
{
    LOG("--- control called");

    ctrl_intf->opc7_id = inst_field::opc7(ctrl_intf->in_inst_id);
    ctrl_intf->funct3_id = inst_field::funct3(ctrl_intf->in_inst_id);
    ctrl_intf->funct7_id = inst_field::funct7(ctrl_intf->in_inst_id);
    ctrl_intf->funct3_ex = inst_field::funct3(ctrl_intf->in_inst_ex);
    ctrl_intf->rs1_addr_id = inst_field::rs1_addr(ctrl_intf->in_inst_id);
    ctrl_intf->rs2_addr_id = inst_field::rs2_addr(ctrl_intf->in_inst_id);
    ctrl_intf->rd_addr_id = inst_field::rd_addr(ctrl_intf->in_inst_id);
    ctrl_intf->rs1_addr_ex = inst_field::rs1_addr(ctrl_intf->in_inst_ex);
    ctrl_intf->rs2_addr_ex = inst_field::rs2_addr(ctrl_intf->in_inst_ex);
    ctrl_intf->rd_addr_ex = inst_field::rd_addr(ctrl_intf->in_inst_ex);
    ctrl_intf->rs1_addr_mem = inst_field::rs1_addr(ctrl_intf->in_inst_mem);
    ctrl_intf->rs2_addr_mem = inst_field::rs2_addr(ctrl_intf->in_inst_mem);
    ctrl_intf->rd_addr_mem = inst_field::rd_addr(ctrl_intf->in_inst_mem);
    //LOG("ctrl, funct7 = " << std::hex << (ctrl_intf->funct7_id) << std::dec);

    decoder.update(ctrl_intf, sys_intf);
    op_fwd.update(ctrl_intf, sys_intf);
    // pipeline_ctrl(ctrl_intf, sys_intf);
    branch_resolution(ctrl_intf, sys_intf);
    store_mask(ctrl_intf);

    // /* new func: */ control_store_mask(ctrl_intf);


    // ------- for test only, imitate sequential assignment
    ctrl_intf->in_inst_mem = ctrl_intf->in_inst_ex;
    ctrl_intf->rd_we_mem = ctrl_intf->rd_we_ex;

    ctrl_intf->in_inst_ex = ctrl_intf->in_inst_id;
    ctrl_intf->rd_we_ex = ctrl_intf->dec_reg_we_id;
    ctrl_intf->dec_store_inst_ex = ctrl_intf->dec_store_inst_id;

    // ctrl_intf->dec_branch_inst_ex = ctrl_intf->dec_branch_inst_id;
    // ctrl_intf->dec_jump_inst_ex = ctrl_intf->dec_jump_inst_id;
}

void control::pipeline_ctrl(ctrl_intf_t *ctrl_intf, sys_intf_t *sys_intf)
{
    LOG("--- pipeline control called");
    LOGW("function not tested, needs system intf inputs implementation");

    ctrl_intf->stall_if = ctrl_intf->dec_branch_inst_id | 
        ctrl_intf->dec_jump_inst_id /* ||
        dut_m_dd_bubble_ex*/;
    
    ctrl_intf->clear_if = ctrl_intf->dec_branch_inst_id |
        ctrl_intf->dec_jump_inst_id;

    ctrl_intf->clear_id = sys_intf->rst_seq_id /* || dut_m_dd_bubble_ex*/;
    ctrl_intf->clear_ex = sys_intf->rst_seq_ex;
    ctrl_intf->clear_mem = sys_intf->rst_seq_mem;
    
    ctrl_intf->dec_pc_we_if = ctrl_intf->dec_pc_we_if & (~ctrl_intf->stall_if);
}

void control::branch_resolution(ctrl_intf_t *ctrl_intf, sys_intf_t *sys_intf)
{
    LOG("--- branch resolution called");
    LOG("branch inst ex: " << ctrl_intf->dec_branch_inst_ex);

    // this function works on data in the ex stage

    br_sel_t branch_type = br_sel_t(((ctrl_intf->funct3_ex & 0b100) >> 1) |
        (ctrl_intf->funct3_ex & 0b001));
    uint32_t branch_taken = 0;

    switch (branch_type) {
    case br_sel_t::beq: branch_taken = ctrl_intf->bc_a_eq_b; break;
    case br_sel_t::bne: branch_taken = ~ctrl_intf->bc_a_eq_b; break;
    case br_sel_t::blt: branch_taken = ctrl_intf->bc_a_lt_b; break;
    case br_sel_t::bge: branch_taken = ctrl_intf->bc_a_eq_b | ~ctrl_intf->bc_a_lt_b; break;
    default: LOGW("Branch Resolution default case");
    }

    branch_taken &= ~sys_intf->rst; // if in reset, override branch taken
    branch_taken &= ctrl_intf->dec_branch_inst_ex;  // only taken if it actually is branch
    if (branch_taken || ctrl_intf->dec_jump_inst_ex) ctrl_intf->dec_pc_sel_if = pc_sel_t::alu;
    
    LOG("branch taken: " << branch_taken);
    LOG("dec_sel: " << static_cast<int>(ctrl_intf->dec_pc_sel_if));
}

void control::store_mask(ctrl_intf_t *ctrl_intf)
{
    // dummy, will cause unaligned value issue 
    // ctrl_intf->alu_out = 3;

    uint32_t mask_width = ctrl_intf->funct3_ex & 0b11;
    uint32_t mask_offset = ctrl_intf->alu_out & 0b11;
    
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
    uint32_t inst_en = ~(~ctrl_intf->dec_store_inst_ex << 3); // fill with 1s from right

    ctrl_intf->dec_store_mask_ex = inst_en & (mask >> (4 - mask_offset));
    if (ctrl_intf->dec_store_inst_ex) {
        LOG("ctrl_intf->dec_store_mask_id: " << ctrl_intf->dec_store_mask_ex);
        //LOG("b3: " << b3 << "; b2: " << b2 << "; b1: " << b1 << "; b0: " << b0);
    }
}