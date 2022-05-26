#include "../include/inst_field.h"

uint32_t inst_field::opc7(uint32_t inst) { return (inst & OPC7_M); }
uint32_t inst_field::funct7(uint32_t inst) { return (inst & FUNCT7_M) >> 25; }
uint32_t inst_field::funct3(uint32_t inst) { return (inst & FUNCT3_M) >> 12; }
uint32_t inst_field::rd(uint32_t inst) { return (inst & RD_M) >> 7; }
uint32_t inst_field::rs1(uint32_t inst) { return (inst & RS1_M) >> 15; }
uint32_t inst_field::rs2(uint32_t inst) { return (inst & RS2_M) >> 20; }
uint32_t inst_field::imm_i(uint32_t inst) { return (inst & IMM_I_M) >> 20; }
uint32_t inst_field::imm_s(uint32_t inst) { return ((int(inst) & IMM_S_HI_M) >> 20) | ((inst & IMM_S_LO_M) >> 7); }
uint32_t inst_field::imm_b(uint32_t inst) 
{ 
    return ((int(inst) & IMM_B_SB_M) >> 17) | 
        ((inst & IMM_B_HI_M) >> 1)// |
        ; 
}

