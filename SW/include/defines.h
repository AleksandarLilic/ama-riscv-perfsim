#pragma once

#include <iostream>
#include <iomanip>
#include <bitset>
#include <vector>

// Paths
#define ASM_RISCV_SANITY_TEST "../../../../../riscv-tests/sanity/asm_test/asm_test.hex"
#define ASM_RISCV_ISA_TESTS "../../../../../riscv-tests/riscv-isa-tests/"

// Tests
#define RISCV_SANITY_TESTS 0
#define RISCV_ISA_REGR 1
#define RISCV_ISA_REGR_NUM 38
const std::string riscv_regr_tests[39] = {
    "add","addi","and","andi","auipc","beq","bge","bgeu","blt","bltu",
    "bne"/*,"fence_i"*/,"jal","jalr","lb","lbu","lh","lhu","lui","lw",
    "or","ori","sb","sh","simple","sll","slli","slt","slti","sltiu",
    "sltu","sra","srai","srl","srli","sub","sw","xor","xori" };

// Debug, verbosity
#define LOG_DBG 0
#define LOG_LOGIC_T 0
#define ASM_IMEM 1
#define FORCE_RF 0
#define HEX_READ 0

// LOG features
#define LOG(x) std::cout << x << std::endl
#define LOG_L(x) std::cout << x
#define LOGW(x) std::cout << " >>> WARNING: "<< x << std::endl
#define LOGE(x) std::cout << " >>> ERROR: "<< x << std::endl
// Format HEX, Format BIN
#define FHEX_PAD std::right << std::setw(8) << std::setfill('0')
#define FHEX(x) "0x" << std::hex << FHEX_PAD << x << std::dec
#define FHEXI(x) std::hex << FHEX_PAD << x << std::dec // Instruction print, exact match with .dump
#define FHEXC(x) std::hex << x << std::dec // Clean print - no padding, no prefix
#define FBIN(x,num) "0b" << std::bitset<num>(x)
// Format Reg File
#define FRF_DEF(x,y) std::left << std::setw(2) << std::setfill(' ')<< x << ": " << std::left << std::setw(12) << int32_t(y)
#define FRF(x,y) "  x" << FRF_DEF(x,y) << "  "
#define FRF_M(x,y) "> x" << FRF_DEF(x,y) << "< "

// Combinational logic
#define CL_UNUSED uint32_t(0xFFFF'FFFF)

// Memory sizes
#define IMEM_SIZE 16'384
#define DMEM_SIZE 16'384
#define MEM_M 0x3FFF

// Patterns
#define NOT_RESET 0xAAAA'AAAA
#define NO_REG_UPDATE 32

// CFG
#define CFG_REGS 10

// Instruction field masks
#define OPC7_M uint32_t(0b0111'1111)
#define FUNCT7_M uint32_t((0b111'1111)<<25)
#define FUNCT7_B5_M uint32_t((0b1)<<30)
#define FUNCT3_M uint32_t((0b111)<<12)
#define RD_M uint32_t((0b1'1111)<<7)
#define RS1_M uint32_t((0b1'1111)<<15)
#define RS2_M uint32_t((0b1'1111)<<20)
#define IMM_SHAMT uint32_t(0b1'1111)
#define IMM_31_M int32_t((0b1)<<31)
#define IMM_31_20_M int32_t((0b1111'1111'1111)<<20)
#define IMM_30_25_M int32_t((0b111'1111)<<25)
#define IMM_20_M uint32_t((0b1)<<20)
#define IMM_19_12_M int32_t((0b1111'1111)<<12)
#define IMM_24_21_M int32_t((0b1111)<<21)
#define IMM_11_8_M uint32_t((0b1111)<<8)
#define IMM_7_M uint32_t((0b1)<<7)

// Instructions hard-coded
#define NOP             uint32_t(0x13)  // addi x0 x0 0

// System level items
#define RESET_VECTOR 0

// CSR
#define TOHOST 0x51E

enum class reset_t {
    set = 1,
    clear = 0
};

// Decoder types
enum class opc7_t{ 
    r_type = 0b011'0011,  // R-type
    i_type = 0b001'0011,  // I-type
    load = 0b000'0011,  // I-type
    store = 0b010'0011,  // S-type
    branch = 0b110'0011,  // B-type
    jalr = 0b110'0111,  // J-type, I-format
    jal = 0b110'1111,  // J-type
    lui = 0b011'0111,  // U-type
    auipc = 0b001'0111,  // U-type
    system = 0b111'0011  // System, I-format
};

// MUX select signals
enum class pc_sel_t{ 
    inc4 = 0, // PC = PC + 4
    alu = 1, // ALU output, used for jump & branch
    bp = 2 // PC = Branch prediction output
}; 

enum class alu_op_a_sel_t {
    rs1 = 0,
    pc = 1
};

enum class alu_op_b_sel_t {
    rs2 = 0,
    imm = 1
};

enum class alu_op_a_sel_fwd_t {
    rs1 = 0,
    pc = 1,
    fwd_mem = 2 // forwarding from mem stage
};

enum class alu_op_b_sel_fwd_t {
    rs2 = 0,
    imm = 1,
    fwd_mem = 2 // forwarding from mem stage
};

enum class br_sel_t {
    beq = 0,
    bne = 1,
    blt = 2,
    bge = 3
};

enum class imm_gen_t {
    disabled = 0,
    i_type = 1,
    s_type = 2,
    b_type = 3,
    j_type = 4,
    u_type = 5
};

#define ALU_OPERATIONS uint32_t(16)

enum class alu_op_t {
    op_add = (0b0000),
    op_sub = (0b1000),
    op_sll = (0b0001),
    op_srl = (0b0101),
    op_sra = (0b1101),
    op_slt = (0b0010),
    op_sltu = (0b0011),
    op_xor = (0b0100),
    op_or = (0b0110),
    op_and = (0b0111),
    op_pass_b = (0b1111)
};

enum class wb_sel_t {
    dmem = 0,
    alu = 1,
    inc4 = 2,
    csr = 3
};

enum class rf_t {

    x0_zero = 0, // hard-wired zero
    x1_ra = 1, // return address
    x2_sp = 2, // stack pointer 
    x3_gp = 3, // global pointer
    x4_tp = 4, // thread pointer
    x5_t0 = 5, // temporary/alternate link register
    x6_t1 = 6, // temporary
    x7_t2 = 7, // temporary
    x8_s0 = 8, // saved register/frame pointer
    x9_s1 = 9, // saved register
    x10_a0 = 10, // function argument/return value
    x11_a1 = 11, // function argument/return value
    x12_a2 = 12, // function argument
    x13_a3 = 13, // function argument
    x14_a4 = 14, // function argument
    x15_a5 = 15, // function argument
    x16_a6 = 16, // function argument
    x17_a7 = 17, // function argument
    x18_s2 = 18, // saved register
    x19_s3 = 19, // saved register
    x20_s4 = 20, // saved register
    x21_s5 = 21, // saved register
    x22_s6 = 22, // saved register
    x23_s7 = 23, // saved register
    x24_s8 = 24, // saved register
    x25_s9 = 25, // saved register
    x26_s10 = 26, // saved register
    x27_s11 = 27, // saved register
    x28_t3 = 28, // temporary
    x29_t4 = 29, // temporary
    x30_t5 = 30, // temporary
    x31_t6 = 31  // temporary
};
