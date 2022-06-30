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
    LOG("    Imm Gen Select: " << id_intf->dec_ig_sel_id);
#endif
    LOG("    Imm Gen Output: " << int32_t(id_intf->imm_gen_out));
}

branch_compare::branch_compare(ex_intf_t *ex_intf)
{
    this->ex_intf = ex_intf;
}

void branch_compare::update()
{
    if (ex_intf->bc_uns_ex) {
        ex_intf->bc_a_eq_b = ex_intf->bc_in_a == ex_intf->bcs_in_b;
        ex_intf->bc_a_lt_b = ex_intf->bc_in_a < ex_intf->bcs_in_b;
    }
    else {
        ex_intf->bc_a_eq_b = int32_t(ex_intf->bc_in_a) == int32_t(ex_intf->bcs_in_b);
        ex_intf->bc_a_lt_b = int32_t(ex_intf->bc_in_a) < int32_t(ex_intf->bcs_in_b);
    }
#if LOG_DBG
    LOG("    BC unsigned: " << ex_intf->bc_uns_ex);
#endif
    LOG("    BC output - eq: " << ex_intf->bc_a_eq_b << "; lt: " << ex_intf->bc_a_lt_b);
}

alu::alu(ex_intf_t *ex_intf)
{
    this->ex_intf = ex_intf;

    // TODO: possibly rework this unused init into constructor initialization list
    for (uint32_t i = 0; i < alu_func_array.size(); i++)
        alu_func_array[i] = &alu::alu_unsued;
    
    alu_func_array[uint32_t(alu_op_t::op_add)] = &alu::alu_add;
    alu_func_array[uint32_t(alu_op_t::op_sub)] = &alu::alu_sub;
    alu_func_array[uint32_t(alu_op_t::op_sll)] = &alu::alu_sll;
    alu_func_array[uint32_t(alu_op_t::op_srl)] = &alu::alu_srl;
    alu_func_array[uint32_t(alu_op_t::op_sra)] = &alu::alu_sra;
    alu_func_array[uint32_t(alu_op_t::op_slt)] = &alu::alu_slt;
    alu_func_array[uint32_t(alu_op_t::op_sltu)] = &alu::alu_sltu;
    alu_func_array[uint32_t(alu_op_t::op_xor)] = &alu::alu_xor;
    alu_func_array[uint32_t(alu_op_t::op_or)] = &alu::alu_or;
    alu_func_array[uint32_t(alu_op_t::op_and)] = &alu::alu_and;
    alu_func_array[uint32_t(alu_op_t::op_pass_b)] = &alu::alu_pass_b;
}

void alu::update()
{
    alu_func f_ptr = alu_func_array[ex_intf->alu_op_sel_ex];
    ex_intf->alu_out = (this->*f_ptr)(ex_intf->alu_in_a, ex_intf->alu_in_b);
#if LOG_DBG
    LOG("    ALU Select: " << ex_intf->alu_op_sel_ex);
#endif
    LOG("    ALU Output: " << int32_t(ex_intf->alu_out));
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