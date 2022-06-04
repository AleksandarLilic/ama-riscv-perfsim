#include "../include/inst_field.h"

opc7_t inst_field::opc7(uint32_t inst) { return opc7_t(inst & OPC7_M); }
uint32_t inst_field::funct7(uint32_t inst) { return (inst & FUNCT7_M) >> 25; }
uint32_t inst_field::funct7_b5(uint32_t inst) { return (inst & FUNCT7_B5_M) >> 30; }
uint32_t inst_field::funct3(uint32_t inst) { return (inst & FUNCT3_M) >> 12; }
rf_t inst_field::rd_addr(uint32_t inst) { return rf_t((inst & RD_M) >> 7); }
rf_t inst_field::rs1_addr(uint32_t inst) { return rf_t((inst & RS1_M) >> 15); }
rf_t inst_field::rs2_addr(uint32_t inst) { return rf_t((inst & RS2_M) >> 20); }
uint32_t inst_field::imm_i(uint32_t inst) { return (inst & IMM_I_M) >> 20; }
uint32_t inst_field::imm_s(uint32_t inst) { return ((int(inst) & IMM_S_HI_M) >> 20) | ((inst & IMM_S_LO_M) >> 7); }
uint32_t inst_field::imm_b(uint32_t inst) 
{ 
    return ((int(inst) & IMM_B_SB_M) >> 17) | 
        ((inst & IMM_B_HI_M) >> 1)// |
        ; 
}
