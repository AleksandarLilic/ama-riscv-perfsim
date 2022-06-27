#pragma once

#include "defines.h"
#include "intf.h"
#include "intf_cfg.h"
#include "modules.h"

class core
{
public:
    if_intf_t if_intf{};    // FIXME: hack, move back to private

private:
    intf_cfg intf_cfg;

    uint32_t *imem_ptr;
    uint32_t *dmem_ptr;

    sys_intf_t sys_intf{};

    //if_intf_t if_intf{};
    id_intf_t id_intf{};
    ex_intf_t ex_intf{};
    mem_intf_t mem_intf{};
    wb_intf_t wb_intf{};
    
    reg_file_intf_t reg_file_intf{};

public:
    core() = delete;
    core(seq_queue *q, uint32_t *imem_ptr, uint32_t *dmem_ptr);
    void reset(bool rst_in);
    
    void update_system();
    void update_if();
    void update_id();
    void update_ex();
    void update_mem();
    void update_wb();

    void status_log();

private:
    // id
    control control;
    reg_file reg_file;
    imm_gen imm_gen;
    // ex
    branch_compare branch_compare;
    alu alu;
    store_shift store_shift;
    // mem
    load_shift_mask load_shift_mask;
};
