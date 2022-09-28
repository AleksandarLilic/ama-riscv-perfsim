#include "../include/decoder.h"

decoder::decoder(sys_intf_t *sys_intf, id_intf_t *id_intf)
{
    this->sys_intf = sys_intf;
    this->id_intf = id_intf;
}

void decoder::update()
{
    if (!sys_intf->rst) {
        LOG("    Decoder OPC7 = " << 
            FHEX(static_cast<uint32_t>(id_intf->opc7_id)) << "; "
            FBIN(static_cast<uint32_t>(id_intf->opc7_id), 7));
        switch (opc7_t(id_intf->opc7_id)) {
        case opc7_t::r_type: r_type(id_intf); break;
        case opc7_t::i_type: i_type(id_intf); break;
        case opc7_t::load: load(id_intf); break;
        case opc7_t::store: store(id_intf); break;
        case opc7_t::branch: branch(id_intf); break;
        case opc7_t::jalr: jalr(id_intf); break;
        case opc7_t::jal: jal (id_intf); break;
        case opc7_t::lui: lui(id_intf); break;
        case opc7_t::auipc: auipc(id_intf); break;
        case opc7_t::system: system(id_intf); break;
        default: unsupported(id_intf); break;
        }
    }
    else /* (sys_intf.rst) */ {
        reset(id_intf);
    }
}

void decoder::r_type(id_intf_t *id_intf)
{
#if LOG_DBG 
    LOG("    Decoder R-type"); 
#endif
    uint32_t alu_op_sel = ((inst_field::funct7_b5(id_intf->inst_id)) << 3) | id_intf->funct3_id;

    id_intf->dec_branch_inst_id = 0;
    id_intf->dec_jump_inst_id = 0;
    id_intf->dec_store_inst_id = 0;
    id_intf->dec_load_inst_id = 0;

    id_intf->dec_pc_sel_if = uint32_t(pc_sel_t::inc4);
    id_intf->dec_pc_we_if = 1;
    id_intf->dec_ig_sel_id = uint32_t(imm_gen_t::disabled);

    id_intf->dec_csr_en_id = 0;
    id_intf->dec_csr_we_id = 0;
    id_intf->dec_csr_ui_id = 0;

    id_intf->dec_bc_uns_id = 0;

    id_intf->dec_alu_a_sel_id = uint32_t(alu_op_a_sel_t::rs1);
    id_intf->dec_alu_b_sel_id = uint32_t(alu_op_b_sel_t::rs2);
    id_intf->dec_alu_op_sel_id = uint32_t(alu_op_t(alu_op_sel));

    id_intf->dec_dmem_en_id = 0;
    id_intf->dec_load_sm_en_id = 0;

    id_intf->dec_wb_sel_id = uint32_t(wb_sel_t::alu);
    id_intf->dec_rd_we_id = 1;
}

void decoder::i_type(id_intf_t *id_intf)
{
#if LOG_DBG 
    LOG("    Decoder I-type");
#endif
    uint32_t alu_op_sel_other = id_intf->funct3_id;
    uint32_t alu_op_sel_shift = ((inst_field::funct7_b5(id_intf->inst_id)) << 3) | id_intf->funct3_id;
    uint32_t alu_op_sel = ((id_intf->funct3_id & 0x3) == 1) ? alu_op_sel_shift : alu_op_sel_other;

    //id_intf->assign_dec(decoder_out::r_type);
    //id_intf->dec_alu_op_sel_id = uint32_t(alu_op_sel);

    //id_intf_t dummy{
    //    .dec_branch_inst_id = 1
    //};
    //dummy = (id_intf_t) { 0, 1 };

    id_intf->dec_branch_inst_id = 0;
    id_intf->dec_jump_inst_id = 0;
    id_intf->dec_store_inst_id = 0;
    id_intf->dec_load_inst_id = 0;

    id_intf->dec_pc_sel_if = uint32_t(pc_sel_t::inc4);
    id_intf->dec_pc_we_if = 1;
    id_intf->dec_ig_sel_id = uint32_t(imm_gen_t::i_type);

    id_intf->dec_csr_en_id = 0;
    id_intf->dec_csr_we_id = 0;
    id_intf->dec_csr_ui_id = 0;

    id_intf->dec_bc_uns_id = 0;

    id_intf->dec_alu_a_sel_id = uint32_t(alu_op_a_sel_t::rs1);
    id_intf->dec_alu_b_sel_id = uint32_t(alu_op_b_sel_t::imm);
    id_intf->dec_alu_op_sel_id = uint32_t(alu_op_sel);

    id_intf->dec_dmem_en_id = 0;
    id_intf->dec_load_sm_en_id = 0;

    id_intf->dec_wb_sel_id = uint32_t(wb_sel_t::alu);
    id_intf->dec_rd_we_id = 1;
}

void decoder::load(id_intf_t *id_intf)
{
#if LOG_DBG 
    LOG("    Decoder Load");
#endif

    id_intf->dec_branch_inst_id = 0;
    id_intf->dec_jump_inst_id = 0;
    id_intf->dec_store_inst_id = 0;
    id_intf->dec_load_inst_id = 1;

    id_intf->dec_pc_sel_if = uint32_t(pc_sel_t::inc4);
    id_intf->dec_pc_we_if = 1;
    id_intf->dec_ig_sel_id = uint32_t(imm_gen_t::i_type);

    id_intf->dec_csr_en_id = 0;
    id_intf->dec_csr_we_id = 0;
    id_intf->dec_csr_ui_id = 0;

    id_intf->dec_bc_uns_id = 0;

    id_intf->dec_alu_a_sel_id = uint32_t(alu_op_a_sel_t::rs1);
    id_intf->dec_alu_b_sel_id = uint32_t(alu_op_b_sel_t::imm);
    id_intf->dec_alu_op_sel_id = uint32_t(alu_op_t::op_add);

    id_intf->dec_dmem_en_id = 1;
    id_intf->dec_load_sm_en_id = 1;

    id_intf->dec_wb_sel_id = uint32_t(wb_sel_t::dmem);
    id_intf->dec_rd_we_id = 1;
}

void decoder::store(id_intf_t *id_intf)
{
#if LOG_DBG 
    LOG("    Decoder Store");
#endif

    id_intf->dec_branch_inst_id = 0;
    id_intf->dec_jump_inst_id = 0;
    id_intf->dec_store_inst_id = 1;
    id_intf->dec_load_inst_id = 0;

    id_intf->dec_pc_sel_if = uint32_t(pc_sel_t::inc4);
    id_intf->dec_pc_we_if = 1;
    id_intf->dec_ig_sel_id = uint32_t(imm_gen_t::s_type);

    id_intf->dec_csr_en_id = 0;
    id_intf->dec_csr_we_id = 0;
    id_intf->dec_csr_ui_id = 0;

    id_intf->dec_bc_uns_id = 0;

    id_intf->dec_alu_a_sel_id = uint32_t(alu_op_a_sel_t::rs1);
    id_intf->dec_alu_b_sel_id = uint32_t(alu_op_b_sel_t::imm);
    id_intf->dec_alu_op_sel_id = uint32_t(alu_op_t::op_add);

    id_intf->dec_dmem_en_id = 1;
    id_intf->dec_load_sm_en_id = 0;

    id_intf->dec_wb_sel_id = uint32_t(wb_sel_t::alu);
    id_intf->dec_rd_we_id = 0;
}

void decoder::branch(id_intf_t *id_intf)
{
#if LOG_DBG 
    LOG("    Decoder Branch");
#endif

    id_intf->dec_branch_inst_id = 1;
    id_intf->dec_jump_inst_id = 0;
    id_intf->dec_store_inst_id = 0;
    id_intf->dec_load_inst_id = 0;

    id_intf->dec_pc_sel_if = uint32_t(pc_sel_t::inc4);
    id_intf->dec_pc_we_if = 0;
    id_intf->dec_ig_sel_id = uint32_t(imm_gen_t::b_type);

    id_intf->dec_csr_en_id = 0;
    id_intf->dec_csr_we_id = 0;
    id_intf->dec_csr_ui_id = 0;

    id_intf->dec_bc_uns_id = (id_intf->funct3_id & 0b010) >> 1;

    id_intf->dec_alu_a_sel_id = uint32_t(alu_op_a_sel_t::pc);
    id_intf->dec_alu_b_sel_id = uint32_t(alu_op_b_sel_t::imm);
    id_intf->dec_alu_op_sel_id = uint32_t(alu_op_t::op_add);

    id_intf->dec_dmem_en_id = 0;
    id_intf->dec_load_sm_en_id = 0;

    id_intf->dec_wb_sel_id = uint32_t(wb_sel_t::alu);
    id_intf->dec_rd_we_id = 0;
}

void decoder::jalr(id_intf_t *id_intf)
{
#if LOG_DBG 
    LOG("    Decoder JALR");
#endif

    id_intf->dec_branch_inst_id = 0;
    id_intf->dec_jump_inst_id = 1;
    id_intf->dec_store_inst_id = 0;
    id_intf->dec_load_inst_id = 0;

    id_intf->dec_pc_sel_if = uint32_t(pc_sel_t::alu);
    id_intf->dec_pc_we_if = 0;
    id_intf->dec_ig_sel_id = uint32_t(imm_gen_t::i_type);

    id_intf->dec_csr_en_id = 0;
    id_intf->dec_csr_we_id = 0;
    id_intf->dec_csr_ui_id = 0;

    id_intf->dec_bc_uns_id = 0;

    id_intf->dec_alu_a_sel_id = uint32_t(alu_op_a_sel_t::rs1);
    id_intf->dec_alu_b_sel_id = uint32_t(alu_op_b_sel_t::imm);
    id_intf->dec_alu_op_sel_id = uint32_t(alu_op_t::op_add);

    id_intf->dec_dmem_en_id = 0;
    id_intf->dec_load_sm_en_id = 0;

    id_intf->dec_wb_sel_id = uint32_t(wb_sel_t::inc4);
    id_intf->dec_rd_we_id = 1;
}

void decoder::jal(id_intf_t *id_intf)
{
#if LOG_DBG 
    LOG("    Decoder JAL");
#endif

    id_intf->dec_branch_inst_id = 0;
    id_intf->dec_jump_inst_id = 1;
    id_intf->dec_store_inst_id = 0;
    id_intf->dec_load_inst_id = 0;

    id_intf->dec_pc_sel_if = uint32_t(pc_sel_t::alu);
    id_intf->dec_pc_we_if = 0;
    id_intf->dec_ig_sel_id = uint32_t(imm_gen_t::j_type);

    id_intf->dec_csr_en_id = 0;
    id_intf->dec_csr_we_id = 0;
    id_intf->dec_csr_ui_id = 0;

    id_intf->dec_bc_uns_id = 0;

    id_intf->dec_alu_a_sel_id = uint32_t(alu_op_a_sel_t::pc);
    id_intf->dec_alu_b_sel_id = uint32_t(alu_op_b_sel_t::imm);
    id_intf->dec_alu_op_sel_id = uint32_t(alu_op_t::op_add);

    id_intf->dec_dmem_en_id = 0;
    id_intf->dec_load_sm_en_id = 0;

    id_intf->dec_wb_sel_id = uint32_t(wb_sel_t::inc4);
    id_intf->dec_rd_we_id = 1;
}

void decoder::lui(id_intf_t *id_intf)
{
#if LOG_DBG 
    LOG("    Decoder LUI");
#endif

    id_intf->dec_branch_inst_id = 0;
    id_intf->dec_jump_inst_id = 0;
    id_intf->dec_store_inst_id = 0;
    id_intf->dec_load_inst_id = 0;

    id_intf->dec_pc_sel_if = uint32_t(pc_sel_t::inc4);
    id_intf->dec_pc_we_if = 1;
    id_intf->dec_ig_sel_id = uint32_t(imm_gen_t::u_type);

    id_intf->dec_csr_en_id = 0;
    id_intf->dec_csr_we_id = 0;
    id_intf->dec_csr_ui_id = 0;

    id_intf->dec_bc_uns_id = 0;

    id_intf->dec_alu_a_sel_id = uint32_t(alu_op_a_sel_t::rs1);
    id_intf->dec_alu_b_sel_id = uint32_t(alu_op_b_sel_t::imm);
    id_intf->dec_alu_op_sel_id = uint32_t(alu_op_t::op_pass_b);

    id_intf->dec_dmem_en_id = 0;
    id_intf->dec_load_sm_en_id = 0;

    id_intf->dec_wb_sel_id = uint32_t(wb_sel_t::alu);
    id_intf->dec_rd_we_id = 1;
}

void decoder::auipc(id_intf_t *id_intf)
{
#if LOG_DBG 
    LOG("    Decoder AUIPC");
#endif

    id_intf->dec_branch_inst_id = 0;
    id_intf->dec_jump_inst_id = 0;
    id_intf->dec_store_inst_id = 0;
    id_intf->dec_load_inst_id = 0;

    id_intf->dec_pc_sel_if = uint32_t(pc_sel_t::inc4);
    id_intf->dec_pc_we_if = 1;
    id_intf->dec_ig_sel_id = uint32_t(imm_gen_t::u_type);

    id_intf->dec_csr_en_id = 0;
    id_intf->dec_csr_we_id = 0;
    id_intf->dec_csr_ui_id = 0;

    id_intf->dec_bc_uns_id = 0;

    id_intf->dec_alu_a_sel_id = uint32_t(alu_op_a_sel_t::pc);
    id_intf->dec_alu_b_sel_id = uint32_t(alu_op_b_sel_t::imm);
    id_intf->dec_alu_op_sel_id = uint32_t(alu_op_t::op_add);

    id_intf->dec_dmem_en_id = 0;
    id_intf->dec_load_sm_en_id = 0;

    id_intf->dec_wb_sel_id = uint32_t(wb_sel_t::alu);
    id_intf->dec_rd_we_id = 1;
}

void decoder::system(id_intf_t *id_intf)
{
#if LOG_DBG 
    LOG("    Decoder System type");
#endif
    bool valid_addr = (inst_field::imm_i(id_intf->inst_id) == TOHOST);
    
    if (!valid_addr) {
        LOGE("Unsupported CSR address");
        return;
    }

    uint32_t ui = (id_intf->funct3_id) >> 2;
    uint32_t rf_read_en = id_intf->rd_addr_id != uint32_t(rf_t::x0_zero);

    id_intf->dec_branch_inst_id = 0;
    id_intf->dec_jump_inst_id = 0;
    id_intf->dec_store_inst_id = 0;
    id_intf->dec_load_inst_id = 0;

    id_intf->dec_pc_sel_if = uint32_t(pc_sel_t::inc4);
    id_intf->dec_pc_we_if = 1;
    // id_intf->dec_ig_sel_id = uint32_t(imm_gen_t::disabled); // keep last one

    id_intf->dec_csr_en_id = rf_read_en;
    id_intf->dec_csr_we_id = 1;
    id_intf->dec_csr_ui_id = ui;

    //id_intf->dec_bc_uns_id = 0;

    id_intf->dec_alu_a_sel_id = uint32_t(alu_op_a_sel_t::rs1);
    //id_intf->dec_alu_b_sel_id = uint32_t(alu_op_b_sel_t::rs2);
    //id_intf->dec_alu_op_sel_id = uint32_t(alu_op_t::op_add);

    id_intf->dec_dmem_en_id = 0;
    //id_intf->dec_load_sm_en_id = 0;

    id_intf->dec_wb_sel_id = uint32_t(wb_sel_t::csr);
    id_intf->dec_rd_we_id = rf_read_en;
}

void decoder::unsupported(id_intf_t *id_intf)
{
    LOGE("Unsupported instruction. Opcode: " << static_cast<uint32_t>(id_intf->opc7_id));
}

void decoder::reset(id_intf_t *id_intf)
{
#if LOG_DBG 
    LOG("    Decoder Reset");
#endif
    id_intf->dec_branch_inst_id = 0;
    id_intf->dec_jump_inst_id = 0;
    id_intf->dec_store_inst_id = 0;
    id_intf->dec_load_inst_id = 0;

    id_intf->dec_pc_sel_if = uint32_t(pc_sel_t::inc4);
    id_intf->dec_pc_we_if = 1;
    id_intf->dec_ig_sel_id = uint32_t(imm_gen_t::disabled);

    id_intf->dec_csr_en_id = 0;
    id_intf->dec_csr_we_id = 0;
    id_intf->dec_csr_ui_id = 0;

    id_intf->dec_bc_uns_id = 0;

    id_intf->dec_alu_a_sel_id = uint32_t(alu_op_a_sel_t::rs1);
    id_intf->dec_alu_b_sel_id = uint32_t(alu_op_b_sel_t::rs2);
    id_intf->dec_alu_op_sel_id = uint32_t(alu_op_t::op_add);

    id_intf->dec_store_mask_ex = 0;
    id_intf->dec_dmem_en_id = 0;
    id_intf->dec_load_sm_en_id = 0;

    id_intf->dec_wb_sel_id = uint32_t(wb_sel_t::alu);
    id_intf->dec_rd_we_id = 0;

}
