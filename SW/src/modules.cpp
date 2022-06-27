#include "../include/modules.h"

imm_gen::imm_gen(id_intf_t *id_intf)
{
    this->id_intf = id_intf;
}

void imm_gen::update()
{
    switch (imm_gen_t(id_intf->dec_ig_sel_id)) {
    case imm_gen_t::disabled: id_intf->imm_gen_out = 0u; break;
    case imm_gen_t::i_type: id_intf->imm_gen_out = inst_field::imm_i(id_intf->inst_id); break;
    case imm_gen_t::s_type: id_intf->imm_gen_out = inst_field::imm_s(id_intf->inst_id); break;
    case imm_gen_t::b_type: id_intf->imm_gen_out = inst_field::imm_b(id_intf->inst_id); break;
    case imm_gen_t::j_type: id_intf->imm_gen_out = inst_field::imm_j(id_intf->inst_id); break;
    case imm_gen_t::u_type: id_intf->imm_gen_out = inst_field::imm_u(id_intf->inst_id); break;
    default: LOGE("Immediate Generate invalid type");
    }
    // fix srai
    if (id_intf->dec_ig_sel_id == uint32_t(imm_gen_t::i_type) && (id_intf->funct3_id == 0b101))
        id_intf->imm_gen_out &= IMM_SHAMT;
#if LOG_DBG
    LOG("    imm_gen_sel: " << id_intf->dec_ig_sel_id);
    LOG("    imm_gen_out: " << int(id_intf->imm_gen_out));
#endif
}

branch_compare::branch_compare(ex_intf_t *ex_intf)
{
    this->ex_intf = ex_intf;
}

void branch_compare::update()
{
    LOGW("branch_compare called -> placeholder");
}

alu::alu(ex_intf_t *ex_intf)
{
    this->ex_intf = ex_intf;
}

void alu::update()
{
    LOGW("alu called -> placeholder");
    ex_intf->alu_out--;
//    uint32_t local_alu = 0;
//    switch(alu_sel)
//        case 0x00: *alu_out = alu_in_a + alu_in_b; break;
//        case 0x08: *alu_out = alu_in_a - alu_in_b; break;
}

store_shift::store_shift(ex_intf_t *ex_intf)
{
    this->ex_intf = ex_intf;
}

void store_shift::update()
{
    LOGW("store_shift called -> placeholder");
}

load_shift_mask::load_shift_mask(mem_intf_t *mem_intf)
{
    this->mem_intf = mem_intf;
}

void load_shift_mask::update()
{
    LOGW("store_shift called -> placeholder");
}