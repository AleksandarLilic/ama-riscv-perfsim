#pragma once

#include "defines.h"
#include "inst_field.h"
#include "intf.h"
#include "seq_queue.h"
#include "logic_t.h"
#include "cl.h"
#include "intf_cfg.h"
#include "control.h"
#include "reg_file.h"

#include <array>

class core
{
public:
    // interfaces
    intf_cfg intf_cfg;

    uint32_t *imem_ptr;
    uint32_t *dmem_ptr;

    sys_intf_t sys_intf{};

    if_intf_t if_intf{};
    id_intf_t id_intf{};
    ex_intf_t ex_intf{};
    mem_intf_t mem_intf{};
    wb_intf_t wb_intf{};

private: // modules
    // system
    void  reset_seq(sys_intf_t *sys_intf);
    // if
    void front_end(if_intf_t *if_intf, id_intf_t *id_intf);
    // id
    void inst_check();
    control control;
    reg_file reg_file;
    //reg_file reg_file(sys_intf_t *sys_intf, id_intf_t *id_intf, mem_intf_t *mem_intf);
    void imm_gen(id_intf_t *id_intf);
    // ex
    void branch_compare(ex_intf_t *ex_intf);
    void alu(ex_intf_t *ex_intf);
    void store_shift(ex_intf_t *ex_intf);
    // mem
    void load_shift_mask(mem_intf_t *mem_intf);
    // wb
    void writeback(wb_intf_t *wb_intf);

public:
    //uint32_t pc_mock = 0;

private:
    void init(seq_queue *q);    // initialize all signals within structures (names and initial/reset values)
    void mem_init(uint32_t *imem_ptr, uint32_t *dmem_ptr);

public:
    core() = delete;
    core(seq_queue *q, uint32_t *imem_ptr, uint32_t *dmem_ptr);
    void reset(bool rst_in);
    void update();
    void update_fe();

};
