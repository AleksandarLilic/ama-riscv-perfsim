#pragma once

#include "defines.h"
#include "seq_queue.h"
#include "logic_t.h"
#include "intf.h"
#include "inst_field.h"
#include "cl.h"
#include "control.h"
#include "reg_file.h"

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