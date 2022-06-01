#include "../include/control.h"

void control::update(ctrl_intf_t *ctrl_intf, sys_intf_t *sys_intf)
{
    LOG("--- control called");

    ctrl_intf->opc7_id = inst_field::opc7(ctrl_intf->in_inst_id);
    ctrl_intf->funct3_id = inst_field::funct3(ctrl_intf->in_inst_id);
    ctrl_intf->funct7_id = inst_field::funct7(ctrl_intf->in_inst_id);
    ctrl_intf->funct3_ex = inst_field::funct3(ctrl_intf->in_inst_ex);

    //LOG("ctrl, funct7 = " << std::hex << (ctrl_intf->funct7_id) << std::dec);

    decoder.update(ctrl_intf, sys_intf);

    // ----- includes:
    // dut_m_decode_dd();
    // // Operand Forwarding
    // dut_m_decode_op_fwd_alu();
    // dut_m_decode_op_fwd_bcs();
    // dut_m_decode_op_fwd_rf();
    // /* new func: */ control_op_fwd(ctrl_intf);

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

    
}