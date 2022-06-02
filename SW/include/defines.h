#pragma once

#include <iostream>

#define LOG(x) std::cout << x << std::endl
#define LOGW(x) std::cout << " >>> WARNING: "<< x << std::endl
#define LOGE(x) std::cout << " >>> ERROR: "<< x << std::endl

#define CL_UNUSED uint32_t(0xFFFF'FFFF)

// Memory sizes
#define IMEM_SIZE 16'384
#define DMEM_SIZE 16'384

// Instruction field masks
#define OPC7_M uint32_t(0b0111'1111)
#define FUNCT7_M uint32_t((0b111'1111)<<25)
#define FUNCT7_B5_M uint32_t((0b1)<<30)
#define FUNCT3_M uint32_t((0b111)<<12)
#define RD_M uint32_t((0b1'1111)<<7)
#define RS1_M uint32_t((0b1'1111)<<15)
#define RS2_M uint32_t((0b1'1111)<<20)
#define IMM_I_M uint32_t((0b111'1111'1111)<<20)
#define IMM_S_HI_M int32_t((0b111'1111)<<25)
#define IMM_S_LO_M uint32_t((0b1'1111)<<7)
#define IMM_B_SB_M int32_t((0b1)<<31)
#define IMM_B_HI_M uint32_t((0b11'1111)<<25)
#define IMM_B_LO_M uint32_t((0b1111)<<8)
#define IMM_B_11_M uint32_t((0b1)<<7)

// Decoder types
#define OPC7_R_TYPE     0b011'0011      // R-type
#define OPC7_I_TYPE     0b001'0011      // I-type
#define OPC7_LOAD       0b000'0011      // I-type
#define OPC7_STORE      0b010'0011      // S-type
#define OPC7_BRANCH     0b110'0011      // B-type
#define OPC7_JALR       0b110'0111      // J-type, I-format
#define OPC7_JAL        0b110'1111      // J-type
#define OPC7_LUI        0b011'0111      // U-type
#define OPC7_AUIPC      0b001'0111      // U-type
#define OPC7_SYSTEM     0b111'0011      // System, I-format

// NOP
#define NOP             uint32_t(0x13)  // addi x0 x0 0

// MUX select signals
// PC select
#define PC_SEL_INC4         uint32_t(0)     // PC = PC + 4
#define PC_SEL_ALU          uint32_t(1)     // ALU output, used for jump/branch
#define PC_SEL_BP           uint32_t(2)     // PC = Branch prediction output
//#define PC_SEL_START_ADDR   uint32_t(3)     // PC = Hardwired start address

// ALU A operand select
#define ALU_A_SEL_RS1       uint32_t(0)     // A = Reg[rs1]
#define ALU_A_SEL_PC        uint32_t(1)     // A = PC
#define ALU_A_SEL_FWD_ALU   uint32_t(2)     // A = ALU; forwarding from MEM stage

// ALU B operand select
#define ALU_B_SEL_RS2       uint32_t(0)     // B = Reg[rs2]
#define ALU_B_SEL_IMM       uint32_t(1)     // B = Immediate value; from Imm Gen
#define ALU_B_SEL_FWD_ALU   uint32_t(2)     // B = ALU; forwarding from MEM stage

// Write back select
#define WB_SEL_DMEM         uint32_t(0)     // Reg[rd] = DMEM[ALU]
#define WB_SEL_ALU          uint32_t(1)     // Reg[rd] = ALU
#define WB_SEL_INC4         uint32_t(2)     // Reg[rd] = PC + 4
#define WB_SEL_CSR          uint32_t(3)     // Reg[rd] = CSR data

// Branch Resolution
#define BR_SEL_BEQ          uint32_t(0)     // Branch Equal
#define BR_SEL_BNE          uint32_t(1)     // Branch Not Equal
#define BR_SEL_BLT          uint32_t(2)     // Branch Less Than
#define BR_SEL_BGE          uint32_t(3)     // Branch Greater Than

// Imm Gen
#define IG_DISABLED         uint32_t(0)
#define IG_I_TYPE           uint32_t(1)
#define IG_S_TYPE           uint32_t(2)
#define IG_B_TYPE           uint32_t(3)
#define IG_J_TYPE           uint32_t(4)
#define IG_U_TYPE           uint32_t(5)

// ALU
// TODO: convert to enum
#define ALU_ADD     uint32_t(0b0000)
#define ALU_SUB     uint32_t(0b1000)
#define ALU_SLL     uint32_t(0b0001)
#define ALU_SRL     uint32_t(0b0101)
#define ALU_SRA     uint32_t(0b1101)
#define ALU_SLT     uint32_t(0b0010)
#define ALU_SLTU    uint32_t(0b0011)
#define ALU_XOR     uint32_t(0b0100)
#define ALU_OR      uint32_t(0b0110)
#define ALU_AND     uint32_t(0b0111)
#define ALU_PASS_B  uint32_t(0b1111)

