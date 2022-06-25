#pragma once

#include "defines.h"
#include "intf.h"
#include "logic_t.h"
#include <string>

typedef struct logic_init_cfg_t {
    std::string id;
    uint32_t *reset;
    uint32_t *clear;
    uint32_t *enable;
} logic_init_cfg_t;

class intf_cfg
{
private:
    uint32_t unused_true = 1;
    uint32_t unused_false = 0;
    std::vector <logic_t *> logic_ptr;

private:
    void init(seq_queue *q, logic_init_cfg_t *logic_init_cfg, uint32_t loop);
    void init_if_id(logic_t *logic_ptr_pc, logic_t *logic_ptr_stall, logic_t *logic_ptr_imem, 
        if_intf_t *if_intf, id_intf_t *id_intf, uint32_t *imem_dout);
    void init_id_ex(logic_t *logic_ptr, id_intf_t *id_intf, ex_intf_t *ex_intf);
    void init_ex_mem(logic_t *logic_ptr, id_intf_t *id_intf, ex_intf_t *ex_intf, mem_intf_t *mem_intf);
    void init_reg_file(logic_t *logic_ptr, reg_file_intf_t *reg_file_intf);

public:
    void init_regs(seq_queue *q, sys_intf_t *sys_intf, reg_file_intf_t *reg_file_intf, 
        if_intf_t *if_intf, id_intf_t *id_intf, ex_intf_t *ex_intf, mem_intf_t *mem_intf, uint32_t *imem_dout);

private:
    logic_init_cfg_t regs_cfg[CFG_REGS];
    void init_sys(logic_t *logic_ptr, sys_intf_t *sys_intf);
};
