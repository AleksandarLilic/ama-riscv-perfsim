#pragma once

#include "defines.h"
#include "logic_t.h"
#include "intf.h"
#include "seq_queue.h"

typedef struct logic_init_cfg_t {
    std::string id;
    uint32_t init;
    uint32_t *input;
    uint32_t *output;
} logic_init_cfg_t;

class intf_cfg
{
public:
    ctrl_intf_t ctrl_intf{};
    seq_queue q;
private:
    seq_queue *qp = &q;
public:
    intf_cfg();
private:
    logic_init_cfg_t logic_init_cfg[2] = {
        {"branch_inst_id_ex", 0, &ctrl_intf.dec_branch_inst_id, &ctrl_intf.dec_branch_inst_ex},
        {"jump_inst_id_ex", 0, &ctrl_intf.dec_jump_inst_id, &ctrl_intf.dec_jump_inst_ex}
    };

    void init();

    // { "PC", 0, &pc_input, &pc_output},

};




