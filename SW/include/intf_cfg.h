#pragma once

#include "defines.h"
#include "logic_t.h"
#include "intf.h"
#include "seq_queue.h"

#ifndef MULTI_LOGIC
#define SYS_SIZE 3
#define IF_ID_SIZE 3
#define ID_EX_SIZE 9
#define EX_MEM_SIZE 6
#endif

#define RESET_VECTOR 0

#define CFG_REGS 8

#ifndef MULTI_LOGIC
typedef struct logic_init_cfg_t {
    std::string id;
    uint32_t init;
    uint32_t *reset;
    uint32_t *enable;
    uint32_t *input;
    uint32_t *output;
} logic_init_cfg_t;
#endif

#ifdef MULTI_LOGIC
typedef struct logic_init_cfg_t {
    std::string id;
    uint32_t *reset;
    uint32_t *clear;
    uint32_t *enable;
} logic_init_cfg_t;
#endif

class intf_cfg
{
public:
private:
    uint32_t unused_true = 1;
    uint32_t unused_false = 0;
    std::vector <logic_t *> logic_ptr;
#ifndef MULTI_LOGIC
public:
    void init_sys(seq_queue *q, sys_intf_t *sys_intf);
    void init_if_id(seq_queue *q, sys_intf_t *sys_intf, if_intf_t *if_intf, id_intf_t *id_intf, 
        uint32_t *imem_dout);
    void init_id_ex(seq_queue *q, sys_intf_t *sys_intf, id_intf_t *id_intf, ex_intf_t *ex_intf);
    void init_ex_mem(seq_queue *q, sys_intf_t *sys_intf, id_intf_t *id_intf, ex_intf_t *ex_intf, mem_intf_t *mem_intf);
    void init_mem_wb();
private:
    void init(seq_queue *q, logic_init_cfg_t *logic_init_cfg, uint32_t loop);
    logic_init_cfg_t sys[SYS_SIZE];
    logic_init_cfg_t if_id[IF_ID_SIZE];
    logic_init_cfg_t id_ex[ID_EX_SIZE];
    logic_init_cfg_t ex_mem[EX_MEM_SIZE];
#endif

#ifdef MULTI_LOGIC
private:
    void init(seq_queue *q, logic_init_cfg_t *logic_init_cfg, uint32_t loop);
    void init_if_id(logic_t *logic_ptr_pc, logic_t *logic_ptr_stall, logic_t *logic_ptr_imem, 
        if_intf_t *if_intf, id_intf_t *id_intf, uint32_t *imem_dout);
    void init_id_ex(logic_t *logic_ptr, id_intf_t *id_intf, ex_intf_t *ex_intf);
    void init_ex_mem(logic_t *logic_ptr, id_intf_t *id_intf, ex_intf_t *ex_intf,
        mem_intf_t *mem_intf);
    void init_reg_file(logic_t *logic_ptr, reg_file_intf_t *reg_file_intf);

public:
    void init_regs(seq_queue *q, sys_intf_t *sys_intf, reg_file_intf_t *reg_file_intf, 
        if_intf_t *if_intf, id_intf_t *id_intf, ex_intf_t *ex_intf, mem_intf_t *mem_intf, 
        uint32_t *imem_dout);

private:
    logic_init_cfg_t regs_cfg[CFG_REGS];
    void init_sys(logic_t *logic_ptr, sys_intf_t *sys_intf);

#endif

};





