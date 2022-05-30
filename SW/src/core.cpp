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

// Core ID stage
void core::control(ctrl_intf_t *ctrl_intf)
{
    LOG("control called");
    //ctrl_intf->in_inst_id = 0x00b500b3; // add     x1, x10, x11
    ctrl_intf->in_inst_id = 0x41ba02b3; // sub     x5, x20, x27

    ctrl_intf->opc7_id = inst_field::opc7(ctrl_intf->in_inst_id);
    ctrl_intf->funct3_id = inst_field::funct3(ctrl_intf->in_inst_id);
    ctrl_intf->funct7_id = inst_field::funct7(ctrl_intf->in_inst_id);
    ctrl_intf->funct3_ex = inst_field::funct3(ctrl_intf->in_inst_ex);

    LOG("ctrl, funct7= " << std::hex << (ctrl_intf->funct7_id) << std::dec);

    decode(ctrl_intf);

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

void core::decode(ctrl_intf_t *ctrl_intf)
{
    LOG("dec called");
    LOG("dec !sys_intf.rst = " << !sys_intf.rst);
    LOG("dec ctrl_intf->opc7_id hex = " << std::hex << ctrl_intf->opc7_id << std::dec);
    if (!sys_intf.rst) {
        switch (ctrl_intf->opc7_id) {
        case OPC7_R_TYPE: decode_r_type(ctrl_intf); break;
        //case OPC7_I_TYPE: decode_i_type(ctrl_intf); break;
        //case OPC7_LOAD: decode_load(ctrl_intf); break;
        //case OPC7_STORE: decode_store(ctrl_intf); break;
        //case OPC7_BRANCH: decode_branch(ctrl_intf); break;
        //case OPC7_JALR: decode_jalr(ctrl_intf); break;
        //case OPC7_JAL: decode_jal (ctrl_intf); break;
        //case OPC7_LUI: decode_lui(ctrl_intf); break;
        //case OPC7_AUIPC: decode_auipc(ctrl_intf); break;
        //case OPC7_SYSTEM: decode_system(ctrl_intf); break;
        //default: decode_unsupported(ctrl_intf); break;
        }
    } else /* (sys_intf.rst) */ {
        decode_reset(ctrl_intf);
    }
    

    //LOG(ctrl_intf->ctrl_intf_out_decoder.alu_op_sel_id);

    int32_t imm_s = inst_field::imm_s(0xF000'0800);
    LOG("imm_s mask test: " << std::hex << imm_s);

}

void core::decode_r_type(ctrl_intf_t *ctrl_intf)
{
    LOG("dec r type called");
    uint32_t alu_op_sel = ((inst_field::funct7_b5(ctrl_intf->in_inst_id)) << 3) | ctrl_intf->funct3_id;
    LOG("dec, alu sel op = " << alu_op_sel);

    ctrl_intf->dec_branch_inst_id = 0;
    ctrl_intf->dec_jump_inst_id = 0;
    ctrl_intf->dec_store_inst_id = 0;
    ctrl_intf->dec_load_inst_id = 0;

    ctrl_intf->dec_pc_sel_if = PC_SEL_INC4;
    ctrl_intf->dec_pc_we_if = 1;
    ctrl_intf->dec_ig_sel_id = IG_DISABLED;

    ctrl_intf->dec_csr_en_id = 0;
    ctrl_intf->dec_csr_we_id = 0;
    ctrl_intf->dec_csr_ui_id = 0;

    ctrl_intf->dec_bc_uns_id = 0;

    ctrl_intf->dec_alu_a_sel_id = ALU_A_SEL_RS1;
    ctrl_intf->dec_alu_b_sel_id = ALU_B_SEL_RS2;
    ctrl_intf->dec_alu_op_sel_id = alu_op_sel;

    ctrl_intf->dec_store_mask_id = 0;
    ctrl_intf->dec_dmem_en_id = 0;
    ctrl_intf->dec_load_sm_en_id = 0;

    ctrl_intf->dec_wb_sel_id = WB_SEL_ALU;
    ctrl_intf->dec_reg_we_id = 1;
}

void core::decode_reset(ctrl_intf_t *ctrl_intf)
{
    ctrl_intf->dec_branch_inst_id = 0;
    ctrl_intf->dec_jump_inst_id = 0;
    ctrl_intf->dec_store_inst_id = 0;
    ctrl_intf->dec_load_inst_id = 0;

    ctrl_intf->dec_pc_sel_if = PC_SEL_INC4;
    ctrl_intf->dec_pc_we_if = 1;
    ctrl_intf->dec_ig_sel_id = IG_DISABLED;

    ctrl_intf->dec_csr_en_id = 0;
    ctrl_intf->dec_csr_we_id = 0;
    ctrl_intf->dec_csr_ui_id = 0;

    ctrl_intf->dec_bc_uns_id = 0;

    ctrl_intf->dec_alu_a_sel_id = ALU_A_SEL_RS1;
    ctrl_intf->dec_alu_b_sel_id = ALU_B_SEL_RS2;
    ctrl_intf->dec_alu_op_sel_id = 0x0;

    ctrl_intf->dec_store_mask_id = 0;
    ctrl_intf->dec_dmem_en_id = 0;
    ctrl_intf->dec_load_sm_en_id = 0;

    ctrl_intf->dec_wb_sel_id = WB_SEL_ALU;
    ctrl_intf->dec_reg_we_id = 0;

}

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
