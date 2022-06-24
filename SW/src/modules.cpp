#include "../include/modules.h"

imm_gen::imm_gen(id_intf_t *id_intf)
{
    this->id_intf = id_intf;
}

void imm_gen::update()
{
    LOG("imm_gen called");
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
    LOG("    imm_gen_sel: " << id_intf->dec_ig_sel_id);
    LOG("    imm_gen_out: " << int(id_intf->imm_gen_out));
}

branch_compare::branch_compare(ex_intf_t *ex_intf)
{
    this->ex_intf = ex_intf;
}

void branch_compare::update()
{
    LOG("branch_compare called -> placeholder");
}

alu::alu(ex_intf_t *ex_intf)
{
    this->ex_intf = ex_intf;
}

void alu::update()
{
    LOG("alu called -> placeholder");
    ex_intf->alu_out++;
}

store_shift::store_shift(ex_intf_t *ex_intf)
{
    this->ex_intf = ex_intf;
}

void store_shift::update()
{
    LOG("store_shift called -> placeholder");
}

load_shift_mask::load_shift_mask(mem_intf_t *mem_intf)
{
    this->mem_intf = mem_intf;
}

void load_shift_mask::update()
{
    LOG("store_shift called -> placeholder");
}