#include "../include/core.h"


void core::reset(bool rst_in)
{
    sys_intf.rst = rst_in;
    if (sys_intf.rst)
        queue.reset();
}

core::core()
{
    

}

//void core::control(ctrl_intf_t *ctrl_intf_id, ctrl_intf_private_t *ctrl_intf_private)
//{
//    ctrl_intf_private->opc7_id = inst_field::opc7(ctrl_intf_id->inst_id);
//    ctrl_intf_private->funct3_id = inst_field::funct3(ctrl_intf_id->inst_id);
//    ctrl_intf_private->funct7_id = inst_field::funct7(ctrl_intf_id->inst_id);
//    ctrl_intf_private->funct3_ex = inst_field::funct3(ctrl_intf_id->inst_ex);
//
//    decode(ctrl_intf_id, ctrl_intf_private);
//
//    // ----- includes:
//    // dut_m_decode_dd();
//    // // Operand Forwarding
//    // dut_m_decode_op_fwd_alu();
//    // dut_m_decode_op_fwd_bcs();
//    // dut_m_decode_op_fwd_rf();
//    control_op_fwd(ctrl_intf_id);
//
//    // ----- includes:
//    // dut_m_stall_if = dut_m_branch_inst_id || dut_m_jump_inst_id || dut_m_dd_bubble_ex;
//    // dut_m_clear_if = dut_m_branch_inst_id || dut_m_jump_inst_id;
//    // 
//    // dut_m_stall_id = 1'b0;
//    //     dut_m_clear_id = dut_m_rst_seq_id || dut_m_dd_bubble_ex;
//    // 
//    // dut_m_stall_ex = 1'b0;
//    //     dut_m_clear_ex = dut_m_rst_seq_ex;
//    // 
//    // dut_m_stall_mem = 1'b0;
//    //     dut_m_clear_mem = dut_m_rst_seq_mem;
//    // 
//    // // update pc if stall
//    // dut_m_pc_we_if = dut_m_pc_we_if && (!dut_m_stall_if);
//    control_pipeline_ctrl(ctrl_intf_id);
//
//
//    control_store_mask(ctrl_intf_id);
//
//    control_branch_resolution(ctrl_intf_id);
//}
//
//void core::decode(ctrl_intf_t *ctrl_intf_id, ctrl_intf_private_t *ctrl_intf_private)
//{
//    if (!sys_intf.rst) {
//        switch (ctrl_intf_private->opc7_id) {
//        case OPC7_R_TYPE: decode_r_type(ctrl_intf_id); break;
//        case OPC7_I_TYPE: decode_i_type(ctrl_intf_id); break;
//        case OPC7_LOAD: decode_load(ctrl_intf_id); break;
//        case OPC7_STORE: decode_store(ctrl_intf_id, ctrl_intf_private); break;
//        case OPC7_BRANCH: decode_branch(ctrl_intf_id, ctrl_intf_private); break;
//        case OPC7_JALR: decode_jalr(ctrl_intf_id); break;
//        case OPC7_JAL: decode_jal (ctrl_intf_id); break;
//        case OPC7_LUI: decode_lui(ctrl_intf_id); break;
//        case OPC7_AUIPC: decode_auipc(ctrl_intf_id); break;
//        case OPC7_SYSTEM: decode_system(ctrl_intf_id); break;
//        default: decode_unsupported(ctrl_intf_id); break;
//        }
//    } else /* (sys_intf.rst) */ {
//        decode_reset(ctrl_intf_id);
//    }
//    
//    // test things, to remove 
//    int32_t imm_s = inst_field::imm_s(0xF000'0800);
//    LOG("imm_s mask test: " << std::hex << imm_s);
//
//}

//void core::ex_stage()
//{
//   uint32_t mux_out = cl::mux4(ctrl_if_ex.test_logic_type.get(), seq_dp_id_ex_if.rf_read_op_a, fwd_mem, pc_ex, CL_UNUSED);
//   
//   alu_in_a = mux_out;
//   core::alu(alu_in_a, alu_in_b, alu_sel, &alu_out);
//
//   dp_ex_mem.mux_out_ex = mux_out;
//}
//
//uint32_t core::alu()
//{
//    uint32_t local_alu = 0;
//    switch(alu_sel)
//        case 0x00: *alu_out = alu_in_a + alu_in_b; break;
//        case 0x08: *alu_out = alu_in_a - alu_in_b; break;
//
//}
