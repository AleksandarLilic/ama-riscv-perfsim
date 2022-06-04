#pragma once

#include "defines.h"

namespace inst_field
{
    opc7_t opc7(uint32_t inst);
    uint32_t funct7(uint32_t inst);
    uint32_t funct7_b5(uint32_t inst);
    uint32_t funct3(uint32_t inst);
    rf_t rd_addr(uint32_t inst);
    rf_t rs1_addr(uint32_t inst);
    rf_t rs2_addr(uint32_t inst);
    uint32_t imm_i(uint32_t inst);
    uint32_t imm_s(uint32_t inst);
    uint32_t imm_b(uint32_t inst);
    uint32_t imm_j(uint32_t inst);
    uint32_t imm_u(uint32_t inst);
}
