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


    // interfaces
    sys_intf_t *sys_intf_p;
    ctrl_intf_t *ctrl_intf_p;
    //ctrl_if_ex_t ctrl_if_ex;

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
    void init(intf_cfg *intf_cfg);    // initialize all signals within structures (names and initial/reset values)

private:

    // control
    void ex_stage();
    void alu();

public:
    core(intf_cfg *intf_cfg);
    void reset(bool rst_in);
    void update(std::array<uint32_t, IMEM_SIZE> &imem_ptr, std::array<uint32_t, DMEM_SIZE> &dmem_ptr);

};
