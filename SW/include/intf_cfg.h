#pragma once

#include "defines.h"
#include "logic_t.h"
#include "intf.h"
#include "seq_queue.h"

#define IF_ID_SIZE 2

typedef struct logic_init_cfg_t {
    std::string id;
    uint32_t init;
    uint32_t *reset;
    uint32_t *enable;
    uint32_t *input;
    uint32_t *output;
} logic_init_cfg_t;

class intf_cfg
{
public:
private:
    uint32_t in_true = 1;
    uint32_t in_false = 0;
public:
    void init_if_id(seq_queue *q, sys_intf_t *sys_intf, if_intf_t *if_intf, id_intf_t *id_intf, uint32_t *imem_dout);
    void init_id_ex();
    void init_ex_mem();
    void init_mem_wb();
private:
    logic_init_cfg_t if_id[IF_ID_SIZE];

private:
    void init(seq_queue *q, logic_init_cfg_t *logic_init_cfg, uint32_t loop);


};





