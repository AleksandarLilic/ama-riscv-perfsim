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
#define OPC7_M uint32_t(0b0111'0000)
#define FUNCT7_M uint32_t((0b111'1111)<<25)
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
#define OPC7_R_TYPE     0b011'0011     // R-type
#define OPC7_I_TYPE     0b001'0011     // I-type
#define OPC7_LOAD       0b000'0011     // I-type
#define OPC7_STORE      0b010'0011     // S-type
#define OPC7_BRANCH     0b110'0011     // B-type
#define OPC7_JALR       0b110'0111     // J-type, I-format
#define OPC7_JAL        0b110'1111     // J-type
#define OPC7_LUI        0b011'0111     // U-type
#define OPC7_AUIPC      0b001'0111     // U-type
#define OPC7_SYSTEM     0b111'0011     // System, I-format
