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
    if ((id_intf->funct3_id == 0b101) && 
        (id_intf->funct7_id == uint32_t(opc7_t::i_type)))
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
    LOG("    ALU Output: " << int32_t(ex_intf->alu_out) << ", " << FHEX(ex_intf->alu_out));
}

store_shift::store_shift(ex_intf_t *ex_intf)
{
    this->ex_intf = ex_intf;
}

void store_shift::update()
{
    ex_intf->store_offset = ex_intf->alu_out & 0x3;
    ex_intf->dmem_din = ex_intf->bcs_in_b << (ex_intf->store_offset << 3); // First convert to byte shift
    LOG("    Store Bit Offset: " << ex_intf->store_offset << 3);
    LOG("    DMEM Store Input: " << ex_intf->dmem_din);
}

load_shift_mask::load_shift_mask(mem_intf_t *mem_intf)
{
    this->mem_intf = mem_intf;
}

void load_shift_mask::update()
{
    uint32_t load_width = mem_intf->funct3_mem & 0b11;
    uint32_t offset = mem_intf->alu_mem & 0b11;
    uint32_t load_sign_bit = (~(mem_intf->funct3_mem) & 0b100) >> 2; // flip because unsigned is encoded with 1
#ifdef LOG_DBG
    LOG("    Load SM input: " << mem_intf->dmem_dout);
    LOG("    Width: " << load_width);
    LOG("    Offset: " << offset);
    LOG("    Sign Bit: " << load_sign_bit);
#endif

    uint32_t unaligned_access = (
        (load_width == 0b01 && offset == 3) ||  // half out of bounds
        (load_width == 0b10 && offset != 0));   // word out of bounds

    if (unaligned_access) {
        LOGE("DMEM unaligned access not supported. Returning zero.");
        mem_intf->load_sm_out = 0u;
        return;
    }

    uint32_t load_width_bytes = load_width << 3;
    uint32_t mask = 0xFF;
    uint32_t get_data_sign = 0x80 << load_width_bytes;
    uint32_t sign_mask = 0x0;
    uint32_t offset_bytes = offset << 3;

    mask = ~(~mask << load_width_bytes);  // fill with 1s from right
    sign_mask = ~mask;
    mask = mask | ((mask << 8) & 0xFF00'0000);  // add byte[3] if word
    mask = mask << offset_bytes;

    mem_intf->load_sm_out = (mem_intf->dmem_dout & mask) >> offset_bytes;

    if (load_width == 0b10) // if word, can't adjust sign bit
        return;

    uint32_t data_neg = mem_intf->load_sm_out & get_data_sign;

    if (load_sign_bit && data_neg)
        mem_intf->load_sm_out |= sign_mask;
}

csr_file::csr_file(csr_file_intf_t *csr_file_intf, id_intf_t *id_intf, mem_intf_t *mem_intf, wb_intf_t *wb_intf)
{
    this->csr_file_intf = csr_file_intf;
    this->id_intf = id_intf;
    this->mem_intf = mem_intf;
    this->wb_intf = wb_intf;
}

void csr_file::write()
{
    // Default write, keep previous output
    csr_file_intf->tohost_in = csr_file_intf->tohost_out;

    // Actual write, if asserted from control
    if (mem_intf->csr_we_mem) {
        uint32_t csr_din = cl::mux2(mem_intf->csr_ui_mem, mem_intf->alu_mem, mem_intf->csr_uimm_mem);
        csr_file_intf->tohost_in = csr_din;
        LOG("    CSR tohost data write: " << csr_din);
        updated_register = true;
    }
}

void csr_file::read()
{
    if (id_intf->dec_csr_en_id) {
        id_intf->csr_data = csr_file_intf->tohost_out;
        LOG("    CSR tohost data read: " << id_intf->csr_data);
    }
#if RISCV_ISA_REGR
    global_tohost = csr_file_intf->tohost_out;
#endif
}

void csr_file::status_log()
{
    LOG("> Arch State - CSR");
    if(updated_register)
        LOG("    > CSR tohost - 0x51e: " << csr_file_intf->tohost_out << " < ");
    else
        LOG("      CSR tohost - 0x51e: " << csr_file_intf->tohost_out);

    std::cout << std::endl;
    updated_register = false;
}
