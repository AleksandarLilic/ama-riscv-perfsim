#include "../include/inst_field.h"

uint32_t inst_field::opc7(uint32_t inst) { return (inst & OPC7_M); }
uint32_t inst_field::funct7(uint32_t inst) { return (inst & FUNCT7_M) >> 25; }
uint32_t inst_field::funct7_b5(uint32_t inst) { return (inst & FUNCT7_B5_M) >> 30; }
uint32_t inst_field::funct3(uint32_t inst) { return (inst & FUNCT3_M) >> 12; }
uint32_t inst_field::rd_addr(uint32_t inst) { return (inst & RD_M) >> 7; }
uint32_t inst_field::rs1_addr(uint32_t inst) { return (inst & RS1_M) >> 15; }
uint32_t inst_field::rs2_addr(uint32_t inst) { return (inst & RS2_M) >> 20; }
uint32_t inst_field::imm_i(uint32_t inst) { return int32_t(inst & IMM_31_20_M) >> 20; }
uint32_t inst_field::imm_s(uint32_t inst) 
{ 
    return ((int32_t(inst) & IMM_30_25_M) >> 20) |
        ((inst & IMM_11_8_M) >> 7) |
        ((inst & IMM_7_M) >> 7);
}
uint32_t inst_field::imm_b(uint32_t inst)
{ 
    return ((int32_t(inst) & IMM_31_M) >> 19) |
        ((inst & IMM_7_M) << 4) |
        ((inst & IMM_30_25_M) >> 20) |
        ((inst & IMM_11_8_M) >> 7);
}

uint32_t inst_field::imm_j(uint32_t inst)
{
    return ((int32_t(inst) & IMM_31_M) >> 11) |
        ((inst & IMM_19_12_M)) |
        ((inst & IMM_20_M) >> 9) |
        ((inst & IMM_30_25_M) >> 20) |
        ((inst & IMM_24_21_M) >> 20);
}

uint32_t inst_field::imm_u(uint32_t inst)
{
    return ((int32_t(inst) & IMM_31_20_M)) |
        ((inst & IMM_19_12_M));
}