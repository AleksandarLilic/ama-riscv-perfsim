#pragma once

#include "defines.h"

namespace inst_field
{
    uint32_t opc7(uint32_t inst);
    uint32_t funct7(uint32_t inst);
    uint32_t funct7_b5(uint32_t inst);
    uint32_t funct3(uint32_t inst);
    uint32_t rd_addr(uint32_t inst);
    uint32_t rs1_addr(uint32_t inst);
    uint32_t rs2_addr(uint32_t inst);
    uint32_t uimm_zero_ext(uint32_t inst);
    uint32_t imm_i(uint32_t inst);
    uint32_t imm_s(uint32_t inst);
    uint32_t imm_b(uint32_t inst);
    uint32_t imm_j(uint32_t inst);
    uint32_t imm_u(uint32_t inst);
}
