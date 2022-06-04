#include "../include/control.h"

//extern void imemc_print_ex();

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

    // /* new func: */ control_store_mask(ctrl_intf);

    // for test only, imitate sequential assignment
    ctrl_intf->in_inst_mem = ctrl_intf->in_inst_ex;
    ctrl_intf->rd_we_mem = ctrl_intf->rd_we_ex;

    ctrl_intf->in_inst_ex = ctrl_intf->in_inst_id;
    ctrl_intf->rd_we_ex = ctrl_intf->dec_reg_we_id;

    ctrl_intf->dec_branch_inst_ex = ctrl_intf->dec_branch_inst_id;
    ctrl_intf->dec_jump_inst_ex = ctrl_intf->dec_jump_inst_id;
}

void control::pipeline_ctrl(ctrl_intf_t *ctrl_intf, sys_intf_t *sys_intf)
{
    LOG("--- pipeline control called");
    LOGW("function not tested, needs system intf inputs implementation");

    ctrl_intf->stall_if = ctrl_intf->dec_branch_inst_id || 
        ctrl_intf->dec_jump_inst_id /* ||
        dut_m_dd_bubble_ex*/;
    
    ctrl_intf->clear_if = ctrl_intf->dec_branch_inst_id ||
        ctrl_intf->dec_jump_inst_id;

    ctrl_intf->clear_id = sys_intf->rst_seq_id /* || dut_m_dd_bubble_ex*/;
    ctrl_intf->clear_ex = sys_intf->rst_seq_ex;
    ctrl_intf->clear_mem = sys_intf->rst_seq_mem;
    
    ctrl_intf->dec_pc_we_if = ctrl_intf->dec_pc_we_if && (!ctrl_intf->stall_if);
}

void control::branch_resolution(ctrl_intf_t *ctrl_intf, sys_intf_t *sys_intf)
{
    LOG("--- branch resolution called");
    LOG("branch inst ex: " << ctrl_intf->dec_branch_inst_ex);

    // this function works on data in the ex stage

    uint32_t branch_type = ((ctrl_intf->funct3_ex & 0b100) >> 1) |
        (ctrl_intf->funct3_ex & 0b001);
    bool branch_taken = 0;

    switch (branch_type) {
    case BR_SEL_BEQ: branch_taken = ctrl_intf->bc_a_eq_b; break;
    case BR_SEL_BNE: branch_taken = !ctrl_intf->bc_a_eq_b; break;
    case BR_SEL_BLT: branch_taken = ctrl_intf->bc_a_lt_b; break;
    case BR_SEL_BGE: branch_taken = ctrl_intf->bc_a_eq_b || !ctrl_intf->bc_a_lt_b; break;
    default: LOGW("Branch Resolution default case");
    }

    branch_taken &= !sys_intf->rst; // if in reset, override branch taken
    branch_taken &= ctrl_intf->dec_branch_inst_ex;  // only taken if it actually is branch
    if (branch_taken || ctrl_intf->dec_jump_inst_ex) ctrl_intf->dec_pc_sel_if = PC_SEL_ALU;
    
    LOG("branch taken: " << branch_taken);
    LOG("dec_sel: " << ctrl_intf->dec_pc_sel_if);
}
