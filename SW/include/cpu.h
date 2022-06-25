#pragma once

#include "defines.h"
#include "intf.h"
#include "intf_cfg.h"
#include "seq_queue.h"
#include "core.h"
//#include "memory.h"

#include <array>

class cpu
{
private:
    std::array<uint32_t, IMEM_SIZE> imem{};
    uint32_t imem_dout;
    std::array<uint32_t, DMEM_SIZE> dmem{};
    uint32_t dmem_dout;
    seq_queue *q;

    bool reset_status;

    std::array<std::string, IMEM_SIZE> imemc{};

private:
    core core;

public:
    cpu() = delete;
    cpu(seq_queue *q);
    void update();
    void reset(reset_t status) { reset_status = bool(status); };
private:
};
