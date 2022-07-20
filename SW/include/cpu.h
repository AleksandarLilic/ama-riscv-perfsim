#pragma once

#include "defines.h"
#include "intf.h"
#include "intf_cfg.h"
#include "seq_queue.h"
#include "core.h"
#include "memory.h"

#include <array>

extern uint32_t global_inst_to_ctrl;

class cpu
{
private:
    // uint32_t imem_dout;
    // uint32_t *imem_address;
    // uint32_t dmem_dout;
    // uint32_t *dmem_address;
    seq_queue *q;
    bool reset_status;
    core_intf_t core_intf{};

private:
    core core;
    imem imem;
    dmem dmem;

public:
    cpu() = delete;
    cpu(seq_queue *q);
    void update();
    void reset(reset_t status) { reset_status = bool(status); };
    void burn_mem();
};
