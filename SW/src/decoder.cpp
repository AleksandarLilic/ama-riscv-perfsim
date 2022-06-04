#include "../include/decoder.h"

void decoder::update(ctrl_intf_t *ctrl_intf, sys_intf_t *sys_intf)
{
    LOG("--- dec called");
    if (!sys_intf->rst) {
        LOG("dec ctrl_intf->opc7_id hex = " << std::hex << static_cast<int>(ctrl_intf->opc7_id) << std::dec);
        switch (ctrl_intf->opc7_id) {
        case opc7_t::r_type: r_type(ctrl_intf); break;
        case opc7_t::i_type: i_type(ctrl_intf); break;
        case opc7_t::load: load(ctrl_intf); break;
        case opc7_t::store: store(ctrl_intf); break;
        case opc7_t::branch: branch(ctrl_intf); LOGW("branching incomplete"); break;
        case opc7_t::jalr: jalr(ctrl_intf); break;
        case opc7_t::jal: jal (ctrl_intf); break;
        case opc7_t::lui: lui(ctrl_intf); break;
        case opc7_t::auipc: auipc(ctrl_intf); break;
        //case opc7_t::system: system(ctrl_intf); break;
        default: unsupported(ctrl_intf); break;
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

    ctrl_intf->dec_branch_inst_id = 0;
    ctrl_intf->dec_jump_inst_id = 0;
    ctrl_intf->dec_store_inst_id = 0;
    ctrl_intf->dec_load_inst_id = 0;

    ctrl_intf->dec_pc_sel_if = pc_sel_t::inc4;
    ctrl_intf->dec_pc_we_if = 1;
    ctrl_intf->dec_ig_sel_id = imm_gen_t::disabled;

    ctrl_intf->dec_csr_en_id = 0;
    ctrl_intf->dec_csr_we_id = 0;
    ctrl_intf->dec_csr_ui_id = 0;

    ctrl_intf->dec_bc_uns_id = 0;

    ctrl_intf->dec_alu_a_sel_id = alu_op_a_sel_t::rs1;
    ctrl_intf->dec_alu_b_sel_id = alu_op_b_sel_t::rs2;
    ctrl_intf->dec_alu_op_sel_id = alu_op_t(alu_op_sel);

    ctrl_intf->dec_dmem_en_id = 0;
    ctrl_intf->dec_load_sm_en_id = 0;

    ctrl_intf->dec_wb_sel_id = wb_sel_t::alu;
    ctrl_intf->dec_reg_we_id = 1;

    LOG("dec alu sel op = " << static_cast<int>(ctrl_intf->dec_alu_op_sel_id));
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

    ctrl_intf->dec_pc_sel_if = pc_sel_t::inc4;
    ctrl_intf->dec_pc_we_if = 1;
    ctrl_intf->dec_ig_sel_id = imm_gen_t::i_type;

    ctrl_intf->dec_csr_en_id = 0;
    ctrl_intf->dec_csr_we_id = 0;
    ctrl_intf->dec_csr_ui_id = 0;

    ctrl_intf->dec_bc_uns_id = 0;

    ctrl_intf->dec_alu_a_sel_id = alu_op_a_sel_t::rs1;
    ctrl_intf->dec_alu_b_sel_id = alu_op_b_sel_t::imm;
    ctrl_intf->dec_alu_op_sel_id = alu_op_t(alu_op_sel);

    ctrl_intf->dec_dmem_en_id = 0;
    ctrl_intf->dec_load_sm_en_id = 0;

    ctrl_intf->dec_wb_sel_id = wb_sel_t::alu;
    ctrl_intf->dec_reg_we_id = 1;
}

void decoder::load(ctrl_intf_t *ctrl_intf)
{
    LOG("dec load called");

    ctrl_intf->dec_branch_inst_id = 0;
    ctrl_intf->dec_jump_inst_id = 0;
    ctrl_intf->dec_store_inst_id = 0;
    ctrl_intf->dec_load_inst_id = 1;

    ctrl_intf->dec_pc_sel_if = pc_sel_t::inc4;
    ctrl_intf->dec_pc_we_if = 1;
    ctrl_intf->dec_ig_sel_id = imm_gen_t::disabled;

    ctrl_intf->dec_csr_en_id = 0;
    ctrl_intf->dec_csr_we_id = 0;
    ctrl_intf->dec_csr_ui_id = 0;

    ctrl_intf->dec_bc_uns_id = 0;

    ctrl_intf->dec_alu_a_sel_id = alu_op_a_sel_t::rs1;
    ctrl_intf->dec_alu_b_sel_id = alu_op_b_sel_t::rs2;
    ctrl_intf->dec_alu_op_sel_id = alu_op_t::op_add;

    ctrl_intf->dec_dmem_en_id = 1;
    ctrl_intf->dec_load_sm_en_id = 1;

    ctrl_intf->dec_wb_sel_id = wb_sel_t::dmem;
    ctrl_intf->dec_reg_we_id = 1;
}

void decoder::store(ctrl_intf_t *ctrl_intf)
{
    LOG("dec store called");

    ctrl_intf->dec_branch_inst_id = 0;
    ctrl_intf->dec_jump_inst_id = 0;
    ctrl_intf->dec_store_inst_id = 1;
    ctrl_intf->dec_load_inst_id = 0;

    ctrl_intf->dec_pc_sel_if = pc_sel_t::inc4;
    ctrl_intf->dec_pc_we_if = 1;
    ctrl_intf->dec_ig_sel_id = imm_gen_t::s_type;

    ctrl_intf->dec_csr_en_id = 0;
    ctrl_intf->dec_csr_we_id = 0;
    ctrl_intf->dec_csr_ui_id = 0;

    ctrl_intf->dec_bc_uns_id = 0;

    ctrl_intf->dec_alu_a_sel_id = alu_op_a_sel_t::rs1;
    ctrl_intf->dec_alu_b_sel_id = alu_op_b_sel_t::imm;
    ctrl_intf->dec_alu_op_sel_id = alu_op_t::op_add;

    ctrl_intf->dec_dmem_en_id = 1;
    ctrl_intf->dec_load_sm_en_id = 0;

    ctrl_intf->dec_wb_sel_id = wb_sel_t::alu;
    ctrl_intf->dec_reg_we_id = 0;

    LOG("dec store mask = " << ctrl_intf->dec_store_mask_ex);
}

void decoder::branch(ctrl_intf_t *ctrl_intf)
{
    LOG("dec branch called");

    ctrl_intf->dec_branch_inst_id = 1;
    ctrl_intf->dec_jump_inst_id = 0;
    ctrl_intf->dec_store_inst_id = 0;
    ctrl_intf->dec_load_inst_id = 0;

    ctrl_intf->dec_pc_sel_if = pc_sel_t::inc4;
    ctrl_intf->dec_pc_we_if = 0;
    ctrl_intf->dec_ig_sel_id = imm_gen_t::b_type;

    ctrl_intf->dec_csr_en_id = 0;
    ctrl_intf->dec_csr_we_id = 0;
    ctrl_intf->dec_csr_ui_id = 0;

    ctrl_intf->dec_bc_uns_id = (ctrl_intf->funct3_id & 0b010) >> 1;

    ctrl_intf->dec_alu_a_sel_id = alu_op_a_sel_t::rs1;
    ctrl_intf->dec_alu_b_sel_id = alu_op_b_sel_t::imm;
    ctrl_intf->dec_alu_op_sel_id = alu_op_t::op_add;

    ctrl_intf->dec_dmem_en_id = 0;
    ctrl_intf->dec_load_sm_en_id = 0;

    ctrl_intf->dec_wb_sel_id = wb_sel_t::alu;
    ctrl_intf->dec_reg_we_id = 0;

    LOG("dec branch compare unsigned = " << ctrl_intf->dec_bc_uns_id);
}

void decoder::jalr(ctrl_intf_t *ctrl_intf)
{
    LOG("dec jalr called");

    ctrl_intf->dec_branch_inst_id = 0;
    ctrl_intf->dec_jump_inst_id = 1;
    ctrl_intf->dec_store_inst_id = 0;
    ctrl_intf->dec_load_inst_id = 0;

    ctrl_intf->dec_pc_sel_if = pc_sel_t::alu;
    ctrl_intf->dec_pc_we_if = 0;
    ctrl_intf->dec_ig_sel_id = imm_gen_t::i_type;

    ctrl_intf->dec_csr_en_id = 0;
    ctrl_intf->dec_csr_we_id = 0;
    ctrl_intf->dec_csr_ui_id = 0;

    ctrl_intf->dec_bc_uns_id = 0;

    ctrl_intf->dec_alu_a_sel_id = alu_op_a_sel_t::pc;
    ctrl_intf->dec_alu_b_sel_id = alu_op_b_sel_t::imm;
    ctrl_intf->dec_alu_op_sel_id = alu_op_t::op_add;

    ctrl_intf->dec_dmem_en_id = 0;
    ctrl_intf->dec_load_sm_en_id = 0;

    ctrl_intf->dec_wb_sel_id = wb_sel_t::inc4;
    ctrl_intf->dec_reg_we_id = 1;
}

void decoder::jal(ctrl_intf_t *ctrl_intf)
{
    LOG("dec jal called");

    ctrl_intf->dec_branch_inst_id = 0;
    ctrl_intf->dec_jump_inst_id = 1;
    ctrl_intf->dec_store_inst_id = 0;
    ctrl_intf->dec_load_inst_id = 0;

    ctrl_intf->dec_pc_sel_if = pc_sel_t::alu;
    ctrl_intf->dec_pc_we_if = 0;
    ctrl_intf->dec_ig_sel_id = imm_gen_t::j_type;

    ctrl_intf->dec_csr_en_id = 0;
    ctrl_intf->dec_csr_we_id = 0;
    ctrl_intf->dec_csr_ui_id = 0;

    ctrl_intf->dec_bc_uns_id = 0;

    ctrl_intf->dec_alu_a_sel_id = alu_op_a_sel_t::pc;
    ctrl_intf->dec_alu_b_sel_id = alu_op_b_sel_t::imm;
    ctrl_intf->dec_alu_op_sel_id = alu_op_t::op_add;

    ctrl_intf->dec_dmem_en_id = 0;
    ctrl_intf->dec_load_sm_en_id = 0;

    ctrl_intf->dec_wb_sel_id = wb_sel_t::inc4;
    ctrl_intf->dec_reg_we_id = 1;
}

void decoder::lui(ctrl_intf_t *ctrl_intf)
{
    LOG("dec lui called");

    ctrl_intf->dec_branch_inst_id = 0;
    ctrl_intf->dec_jump_inst_id = 0;
    ctrl_intf->dec_store_inst_id = 0;
    ctrl_intf->dec_load_inst_id = 0;

    ctrl_intf->dec_pc_sel_if = pc_sel_t::inc4;
    ctrl_intf->dec_pc_we_if = 1;
    ctrl_intf->dec_ig_sel_id = imm_gen_t::u_type;

    ctrl_intf->dec_csr_en_id = 0;
    ctrl_intf->dec_csr_we_id = 0;
    ctrl_intf->dec_csr_ui_id = 0;

    ctrl_intf->dec_bc_uns_id = 0;

    ctrl_intf->dec_alu_a_sel_id = alu_op_a_sel_t::rs1;
    ctrl_intf->dec_alu_b_sel_id = alu_op_b_sel_t::imm;
    ctrl_intf->dec_alu_op_sel_id = alu_op_t::op_pass_b;

    ctrl_intf->dec_dmem_en_id = 0;
    ctrl_intf->dec_load_sm_en_id = 0;

    ctrl_intf->dec_wb_sel_id = wb_sel_t::alu;
    ctrl_intf->dec_reg_we_id = 1;
}

void decoder::auipc(ctrl_intf_t *ctrl_intf)
{
    LOG("dec auipc called");

    ctrl_intf->dec_branch_inst_id = 0;
    ctrl_intf->dec_jump_inst_id = 0;
    ctrl_intf->dec_store_inst_id = 0;
    ctrl_intf->dec_load_inst_id = 0;

    ctrl_intf->dec_pc_sel_if = pc_sel_t::inc4;
    ctrl_intf->dec_pc_we_if = 1;
    ctrl_intf->dec_ig_sel_id = imm_gen_t::u_type;

    ctrl_intf->dec_csr_en_id = 0;
    ctrl_intf->dec_csr_we_id = 0;
    ctrl_intf->dec_csr_ui_id = 0;

    ctrl_intf->dec_bc_uns_id = 0;

    ctrl_intf->dec_alu_a_sel_id = alu_op_a_sel_t::pc;
    ctrl_intf->dec_alu_b_sel_id = alu_op_b_sel_t::imm;
    ctrl_intf->dec_alu_op_sel_id = alu_op_t::op_add;

    ctrl_intf->dec_dmem_en_id = 0;
    ctrl_intf->dec_load_sm_en_id = 0;

    ctrl_intf->dec_wb_sel_id = wb_sel_t::alu;
    ctrl_intf->dec_reg_we_id = 1;
}

void decoder::unsupported(ctrl_intf_t *ctrl_intf)
{
    LOGE("Unsupported instruction. Opcode: " << static_cast<int>(ctrl_intf->opc7_id));
}


void decoder::reset(ctrl_intf_t *ctrl_intf)
{
    ctrl_intf->dec_branch_inst_id = 0;
    ctrl_intf->dec_jump_inst_id = 0;
    ctrl_intf->dec_store_inst_id = 0;
    ctrl_intf->dec_load_inst_id = 0;

    ctrl_intf->dec_pc_sel_if = pc_sel_t::inc4;
    ctrl_intf->dec_pc_we_if = 1;
    ctrl_intf->dec_ig_sel_id = imm_gen_t::disabled;

    ctrl_intf->dec_csr_en_id = 0;
    ctrl_intf->dec_csr_we_id = 0;
    ctrl_intf->dec_csr_ui_id = 0;

    ctrl_intf->dec_bc_uns_id = 0;

    ctrl_intf->dec_alu_a_sel_id = alu_op_a_sel_t::rs1;
    ctrl_intf->dec_alu_b_sel_id = alu_op_b_sel_t::rs2;
    ctrl_intf->dec_alu_op_sel_id = alu_op_t::op_add;

    ctrl_intf->dec_store_mask_ex = 0;
    ctrl_intf->dec_dmem_en_id = 0;
    ctrl_intf->dec_load_sm_en_id = 0;

    ctrl_intf->dec_wb_sel_id = wb_sel_t::alu;
    ctrl_intf->dec_reg_we_id = 0;

}

//uint32_t decoder::store_mask(ctrl_intf_t *ctrl_intf)
//{
//    LOGW("placeholder store called");
//    return 0;
//}