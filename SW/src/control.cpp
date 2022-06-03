#include "../include/control.h"

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

    // ----- includes:
    // dut_m_stall_if = dut_m_branch_inst_id || dut_m_jump_inst_id || dut_m_dd_bubble_ex;
    // dut_m_clear_if = dut_m_branch_inst_id || dut_m_jump_inst_id;
    // 
    // dut_m_stall_id = 1'b0;
    //     dut_m_clear_id = dut_m_rst_seq_id || dut_m_dd_bubble_ex;
    // 
    // dut_m_stall_ex = 1'b0;
    //     dut_m_clear_ex = dut_m_rst_seq_ex;
    // 
    // dut_m_stall_mem = 1'b0;
    //     dut_m_clear_mem = dut_m_rst_seq_mem;
    // 
    // // update pc if stall
    // dut_m_pc_we_if = dut_m_pc_we_if && (!dut_m_stall_if);
    // /* new func: */ control_pipeline_ctrl(ctrl_intf);

    // /* new func: */ control_store_mask(ctrl_intf);

    // /* new func: */ control_branch_resolution(ctrl_intf);

    // for test only, imitate sequential assignment
    ctrl_intf->in_inst_mem = ctrl_intf->in_inst_ex;
    ctrl_intf->rd_we_mem = ctrl_intf->rd_we_ex;

    ctrl_intf->in_inst_ex = ctrl_intf->in_inst_id;
    ctrl_intf->rd_we_ex = ctrl_intf->dec_reg_we_id;

}