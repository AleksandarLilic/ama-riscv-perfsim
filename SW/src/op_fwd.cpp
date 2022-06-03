#include "../include/op_fwd.h"

void op_fwd::update(ctrl_intf_t *ctrl_intf, sys_intf_t *sys_intf)
{
    LOG("--- op_fwd called");
    dependency_detection(ctrl_intf);
    LOG("dependency detection: ");
    LOG ("    dd_rs1_ex: " << dd_rs1_ex);
    LOG ("    dd_rs2_ex: " << dd_rs2_ex);
    LOG ("    dd_rs1_mem: " << dd_rs1_mem);
    LOG ("    dd_rs2_mem: " << dd_rs2_mem);
    op_fwd_rf(ctrl_intf);
    op_fwd_bcs(ctrl_intf);
    op_fwd_alu(ctrl_intf);
}

void op_fwd::dependency_detection(ctrl_intf_t *ctrl_intf)
{
    // EX stage
    dd_rs1_ex = ((ctrl_intf->rs1_addr_id != RF_X0_ZERO) && 
        (ctrl_intf->rs1_addr_id == ctrl_intf->rd_addr_ex) &&
        (ctrl_intf->rd_addr_ex));
    dd_rs2_ex = ((ctrl_intf->rs2_addr_id != RF_X0_ZERO) &&
        (ctrl_intf->rs2_addr_id == ctrl_intf->rd_addr_ex) &&
        (ctrl_intf->rd_addr_ex));

    // MEM stage
    dd_rs1_mem = ((ctrl_intf->rs1_addr_id != RF_X0_ZERO) &&
        (ctrl_intf->rs1_addr_id == ctrl_intf->rd_addr_mem) &&
        (ctrl_intf->rd_addr_mem));
    dd_rs2_mem = ((ctrl_intf->rs2_addr_id != RF_X0_ZERO) &&
        (ctrl_intf->rs2_addr_id == ctrl_intf->rd_addr_mem) &&
        (ctrl_intf->rd_addr_mem));
}

void op_fwd::op_fwd_rf(ctrl_intf_t *ctrl_intf)
{
    // RF A
    ctrl_intf->of_rf_a_sel_fwd_id = dd_rs1_mem &&
        ((ctrl_intf->dec_alu_a_sel_id == ALU_A_SEL_RS1) ||
            (ctrl_intf->dec_branch_inst_id));

    // RF B
    ctrl_intf->of_rf_b_sel_fwd_id = dd_rs2_mem &&
        ((ctrl_intf->dec_alu_b_sel_id == ALU_B_SEL_RS2) ||
            (ctrl_intf->dec_branch_inst_id) || 
            (ctrl_intf->dec_store_inst_id));
}

void op_fwd::op_fwd_bcs(ctrl_intf_t *ctrl_intf)
{
    // BC A
    ctrl_intf->of_bc_a_sel_fwd_id = dd_rs1_ex &&
        (ctrl_intf->dec_branch_inst_id) /* &&
        !bubble_load */;

    // BC B or DMEM din for store
    ctrl_intf->of_bcs_b_sel_fwd_id = dd_rs2_ex &&
        (ctrl_intf->dec_branch_inst_id || ctrl_intf->dec_store_inst_id) /* &&
        !bubble_load */;
}

void op_fwd::op_fwd_alu(ctrl_intf_t *ctrl_intf)
{
    // Operand A
    if ((dd_rs1_ex) && ((ctrl_intf->dec_alu_a_sel_id == ALU_A_SEL_RS1)) /* && (!bubble_load) */)
        ctrl_intf->of_alu_a_sel_fwd_id = ALU_A_SEL_FWD_ALU;             // forward previous ALU result
    else
        ctrl_intf->of_alu_a_sel_fwd_id = ctrl_intf->dec_alu_a_sel_id;   // don't forward

    // Operand B
    if ((dd_rs2_ex) && ((ctrl_intf->dec_alu_b_sel_id == ALU_B_SEL_RS2)) /* && (!bubble_load) */)
        ctrl_intf->of_alu_b_sel_fwd_id = ALU_B_SEL_FWD_ALU;             // forward previous ALU result
    else
        ctrl_intf->of_alu_b_sel_fwd_id = ctrl_intf->dec_alu_b_sel_id;   // don't forward
}
