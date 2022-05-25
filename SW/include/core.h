#pragma once

#include "defines.h"
#include "inst_field.h"
#include "intf.h"
#include "seq_queue.h"
#include "logic_t.h"
#include <array>
//#include "memory.h"

class core
{
private:
    ctrl_intf_t ctrl_intf_id;
    //ctrl_if_ex_t ctrl_if_ex;
    sys_intf_t sys_intf;
    seq_queue queue;

    //datapath signals
    logic_t alu_ex;

public:

private:
    void init();    // initialize all signals within structures (names and initial/reset values)

    void decode(ctrl_intf_t* ctrl_intf_id);

    void ex_stage();
    void alu();
    // all update:
    void cl_update();
    void seq_update();

public:
    core();
    void reset(bool rst_in);
    void update(std::array<uint32_t, IMEM_SIZE>& imem_ptr, std::array<uint32_t, DMEM_SIZE>& dmem_ptr)
    {
        LOG("imem from core: " << imem_ptr[0] << "\ndmem from core: " << dmem_ptr[0]);
        imem_ptr[1] = 55;
    }

};
