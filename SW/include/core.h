#pragma once

#include "defines.h"
#include "inst_field.h"
#include "intf.h"
#include "seq_queue.h"
#include "logic_t.h"
#include "cl.h"
#include <array>

#include "control.h"
#include "intf_cfg.h"

class core
{
private:

    intf_cfg intf_cfg;

    // interfaces
    // send interfaces to stage constructors?
    // ctrl_intf_t ctrl_intf{};
    ctrl_intf_t *ctrl_intf_p;
    //ctrl_if_ex_t ctrl_if_ex;
    sys_intf_t sys_intf;

    // LOGIC    
    // if_stage if_stage(if_stage_intf)
    // imem in level up hier

    // if_id_pipeline(id_stage_intf, id_stage_intf)

    // id_stage id_stage(id_stage_intf)
            control control; // incl control into id stage
            //control control(ctrl_intf); // incl control into id stage

    // ex_stage ex_stage

    // mem_stage mem_stage
    // dmem in level up hier

    // wb_stage wb_stage

    // framework stuff, probably to be moved to main
    // seq_queue queue;

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
