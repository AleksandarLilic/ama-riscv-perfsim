#pragma once

#include "defines.h"
#include "intf.h"
#include "intf_cfg.h"
#include "seq_queue.h"
#include "core.h"
#include "memory.h"

#include <array>

class cpu
{
private:
    uint32_t imem_dout;
    uint32_t dmem_dout;
    seq_queue *q;
    bool reset_status;

private:
    core core;
    imem imem;

public:
    cpu() = delete;
    cpu(seq_queue *q);
    void update();
    void reset(reset_t status) { reset_status = bool(status); };
};
