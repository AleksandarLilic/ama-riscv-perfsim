#pragma once

#include "defines.h"
#include "inst_field.h"
#include "intf.h"
#include "seq_queue.h"
#include "logic_t.h"
#include "cl.h"
#include <array>

#include "control.h"

class core
{
private:
    // modules
    control control;

    // control interfaces
    ctrl_intf_t ctrl_intf{};

    //ctrl_if_ex_t ctrl_if_ex;
    sys_intf_t sys_intf;
    seq_queue queue;

    //datapath signals for test
    //logic_t alu_ex;

public:
    uint32_t pc_mock = 0;

private:
    void init();    // initialize all signals within structures (names and initial/reset values)

    // control    

    void ex_stage();
    void alu();
    // all update:
    void cl_update();
    void seq_update();

public:
    core();
    void reset(bool rst_in);
    void update(std::array<uint32_t, IMEM_SIZE> &imem_ptr, std::array<uint32_t, DMEM_SIZE> &dmem_ptr);

};
