#include "../include/decoder.h"

void decoder::update(ctrl_intf_t *ctrl_intf, sys_intf_t *sys_intf)
{
    LOG("--- dec called");
    //LOG("dec !sys_intf.rst = " << !sys_intf.rst);
    if (!sys_intf->rst) {
        LOG("dec ctrl_intf->opc7_id hex = " << std::hex << ctrl_intf->opc7_id << std::dec);
        switch (ctrl_intf->opc7_id) {
        case OPC7_R_TYPE: r_type(ctrl_intf); break;
        case OPC7_I_TYPE: i_type(ctrl_intf); break;
        case OPC7_LOAD: load(ctrl_intf); break;
        case OPC7_STORE: store(ctrl_intf); break;
            //case OPC7_BRANCH: branch(ctrl_intf); break;
            //case OPC7_JALR: jalr(ctrl_intf); break;
            //case OPC7_JAL: jal (ctrl_intf); break;
            //case OPC7_LUI: lui(ctrl_intf); break;
            //case OPC7_AUIPC: auipc(ctrl_intf); break;
            //case OPC7_SYSTEM: system(ctrl_intf); break;
            //default: unsupported(ctrl_intf); break;
        }
    }
    else /* (sys_intf.rst) */ {
        reset(ctrl_intf);
        LOG("dec sys_intf.rst = " << sys_intf->rst);
    }


    //LOG(ctrl_intf->ctrl_intf_out_decoder.alu_op_sel_id);

    //int32_t imm_s = inst_field::imm_s(0xF000'0800);
    //LOG("imm_s mask test: " << std::hex << imm_s);

}

void decoder::r_type(ctrl_intf_t *ctrl_intf)
{
    LOG("dec r type called");
    uint32_t alu_op_sel = ((inst_field::funct7_b5(ctrl_intf->in_inst_id)) << 3) | ctrl_intf->funct3_id;
    LOG("dec alu sel op = " << alu_op_sel);

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

void decoder::i_type(ctrl_intf_t *ctrl_intf)
{
    LOG("dec i type called");
    uint32_t alu_op_sel_other = ctrl_intf->funct3_id;
    uint32_t alu_op_sel_shift = ((inst_field::funct7_b5(ctrl_intf->in_inst_id)) << 3) | ctrl_intf->funct3_id;
    uint32_t alu_op_sel = ((ctrl_intf->funct3_id & 0x3) == 1) ? alu_op_sel_shift : alu_op_sel_other;
    LOG("dec alu sel op = " << alu_op_sel);

    ctrl_intf->dec_branch_inst_id = 0;
    ctrl_intf->dec_jump_inst_id = 0;
    ctrl_intf->dec_store_inst_id = 0;
    ctrl_intf->dec_load_inst_id = 0;

    ctrl_intf->dec_pc_sel_if = PC_SEL_INC4;
    ctrl_intf->dec_pc_we_if = 1;
    ctrl_intf->dec_ig_sel_id = IG_I_TYPE;

    ctrl_intf->dec_csr_en_id = 0;
    ctrl_intf->dec_csr_we_id = 0;
    ctrl_intf->dec_csr_ui_id = 0;

    ctrl_intf->dec_bc_uns_id = 0;

    ctrl_intf->dec_alu_a_sel_id = ALU_A_SEL_RS1;
    ctrl_intf->dec_alu_b_sel_id = ALU_B_SEL_IMM;
    ctrl_intf->dec_alu_op_sel_id = alu_op_sel;

    ctrl_intf->dec_store_mask_id = 0;
    ctrl_intf->dec_dmem_en_id = 0;
    ctrl_intf->dec_load_sm_en_id = 0;

    ctrl_intf->dec_wb_sel_id = WB_SEL_ALU;
    ctrl_intf->dec_reg_we_id = 1;
}

void decoder::load(ctrl_intf_t *ctrl_intf)
{
    LOG("dec load called");

    ctrl_intf->dec_branch_inst_id = 0;
    ctrl_intf->dec_jump_inst_id = 0;
    ctrl_intf->dec_store_inst_id = 0;
    ctrl_intf->dec_load_inst_id = 1;

    ctrl_intf->dec_pc_sel_if = PC_SEL_INC4;
    ctrl_intf->dec_pc_we_if = 1;
    ctrl_intf->dec_ig_sel_id = IG_DISABLED;

    ctrl_intf->dec_csr_en_id = 0;
    ctrl_intf->dec_csr_we_id = 0;
    ctrl_intf->dec_csr_ui_id = 0;

    ctrl_intf->dec_bc_uns_id = 0;

    ctrl_intf->dec_alu_a_sel_id = ALU_A_SEL_RS1;
    ctrl_intf->dec_alu_b_sel_id = ALU_B_SEL_RS2;
    ctrl_intf->dec_alu_op_sel_id = ALU_ADD;

    ctrl_intf->dec_store_mask_id = 0;
    ctrl_intf->dec_dmem_en_id = 1;
    ctrl_intf->dec_load_sm_en_id = 1;

    ctrl_intf->dec_wb_sel_id = WB_SEL_DMEM;
    ctrl_intf->dec_reg_we_id = 1;
}

void decoder::store(ctrl_intf_t *ctrl_intf)
{
    LOG("dec store called");

    ctrl_intf->dec_branch_inst_id = 0;
    ctrl_intf->dec_jump_inst_id = 0;
    ctrl_intf->dec_store_inst_id = 1;
    ctrl_intf->dec_load_inst_id = 0;

    ctrl_intf->dec_pc_sel_if = PC_SEL_INC4;
    ctrl_intf->dec_pc_we_if = 1;
    ctrl_intf->dec_ig_sel_id = IG_S_TYPE;

    ctrl_intf->dec_csr_en_id = 0;
    ctrl_intf->dec_csr_we_id = 0;
    ctrl_intf->dec_csr_ui_id = 0;

    ctrl_intf->dec_bc_uns_id = 0;

    ctrl_intf->dec_alu_a_sel_id = ALU_A_SEL_RS1;
    ctrl_intf->dec_alu_b_sel_id = ALU_B_SEL_IMM;
    ctrl_intf->dec_alu_op_sel_id = ALU_ADD;

    ctrl_intf->dec_store_mask_id = store_mask(ctrl_intf);
    ctrl_intf->dec_dmem_en_id = 1;
    ctrl_intf->dec_load_sm_en_id = 0;

    ctrl_intf->dec_wb_sel_id = WB_SEL_ALU;
    ctrl_intf->dec_reg_we_id = 0;
}

void decoder::reset(ctrl_intf_t *ctrl_intf)
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

uint32_t decoder::store_mask(ctrl_intf_t *ctrl_intf)
{
    LOGW("placeholder store called");
    return 0;
}