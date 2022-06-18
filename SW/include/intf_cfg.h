#pragma once

#include "defines.h"
#include "logic_t.h"
#include "intf.h"
#include "seq_queue.h"

#define SYS_SIZE 3
#define IF_ID_SIZE 3
#define ID_EX_SIZE 9
#define EX_MEM_SIZE 6
#define RESET_VECTOR 0

typedef struct logic_init_cfg_t {
    std::string id;
    uint32_t init;
    uint32_t *reset;
#ifdef MULTI_LOGIC
    uint32_t *clear;
#endif
    uint32_t *enable;
#ifndef MULTI_LOGIC
    uint32_t *input;
    uint32_t *output;
#endif
} logic_init_cfg_t;

class intf_cfg
{
public:
private:
    uint32_t unsued_ture = 1;
    uint32_t unsued_false = 0;
public:
    void init_sys(seq_queue *q, sys_intf_t *sys_intf);
    void init_if_id(seq_queue *q, sys_intf_t *sys_intf, if_intf_t *if_intf, id_intf_t *id_intf, 
        uint32_t *imem_dout);
    void init_id_ex(seq_queue *q, sys_intf_t *sys_intf, id_intf_t *id_intf, ex_intf_t *ex_intf);
    void init_ex_mem(seq_queue *q, sys_intf_t *sys_intf, id_intf_t *id_intf, ex_intf_t *ex_intf, mem_intf_t *mem_intf);
    void init_mem_wb();
private:
    logic_init_cfg_t sys[SYS_SIZE];
    logic_init_cfg_t if_id[IF_ID_SIZE];
    logic_init_cfg_t id_ex[ID_EX_SIZE];
    logic_init_cfg_t ex_mem[EX_MEM_SIZE];

private:
    void init(seq_queue *q, logic_init_cfg_t *logic_init_cfg, uint32_t loop);

public:
    void init_pipelines(seq_queue *q, sys_intf_t *sys_intf, if_intf_t *if_intf, id_intf_t *id_intf,
        ex_intf_t *ex_intf, mem_intf_t *mem_intf);
private:
    logic_init_cfg_t pipelines_cfg[4];

};





