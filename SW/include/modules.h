#pragma once

#include "defines.h"
#include "intf.h"
#include "inst_field.h"
#include "cl.h"

#include "control.h"
#include "reg_file.h"

#include <array>

class imm_gen
{
private:
    id_intf_t *id_intf;

public:
    void update();
    imm_gen() = delete;
    imm_gen(id_intf_t *id_intf);
};

class branch_compare
{
private:
    ex_intf_t *ex_intf;

public:
    void update();
    branch_compare() = delete;
    branch_compare(ex_intf_t *ex_intf);
};

class alu
{
private:
    uint32_t alu_add(uint32_t a, uint32_t b) { return a + b; };
    uint32_t alu_sub(uint32_t a, uint32_t b) { return a - b; };
    uint32_t alu_sll(uint32_t a, uint32_t b) { return a << b; };
    uint32_t alu_srl(uint32_t a, uint32_t b) { return a >> b; };
    uint32_t alu_sra(uint32_t a, uint32_t b) { return int32_t(a) >> b; };
    uint32_t alu_slt(uint32_t a, uint32_t b) { return int32_t(a) < int32_t(b); };
    uint32_t alu_sltu(uint32_t a, uint32_t b) { return a < b; };
    uint32_t alu_xor(uint32_t a, uint32_t b) { return a ^ b; };
    uint32_t alu_or(uint32_t a, uint32_t b) { return a | b; };
    uint32_t alu_and(uint32_t a, uint32_t b) { return a & b; };
    uint32_t alu_pass_b(uint32_t a, uint32_t b) { return b; };
    uint32_t alu_unsued(uint32_t a, uint32_t b) { LOGE("ALU unused function");  return 1u; };

private:
    using alu_func = uint32_t(alu::*)(uint32_t, uint32_t);
    std::array<alu_func, ALU_OPERATIONS> alu_func_array{};

private:
    ex_intf_t *ex_intf;

public:
    void update();
    alu() = delete;
    alu(ex_intf_t *ex_intf);
};

class store_shift
{
private:
    ex_intf_t *ex_intf;

public:
    void update();
    store_shift() = delete;
    store_shift(ex_intf_t *ex_intf);
};

class load_shift_mask
{
private:
    mem_intf_t *mem_intf;

public:
    void update();
    load_shift_mask() = delete;
    load_shift_mask(mem_intf_t *mem_intf);
};

class csr_file
{
private:
    id_intf_t *id_intf;
    mem_intf_t *mem_intf;
    wb_intf_t *wb_intf;
    csr_file_intf_t *csr_file_intf;
    bool updated_register;

public:
    csr_file() = delete;
    csr_file(csr_file_intf_t *csr_file_intf, id_intf_t *id_intf, mem_intf_t *mem_intf, wb_intf_t *wb_intf);
    void write();
    void read();
    void status_log();

};