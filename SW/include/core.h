#pragma once

#include "defines.h"
#include "inst_field.h"
#include "intf.h"
#include "seq_queue.h"
#include "logic_t.h"
#include "cl.h"
#include "intf_cfg.h"
#include <array>

#include "control.h"

class core
{
private:

    intf_cfg intf_cfg;

    // interfaces
    ctrl_intf_t ctrl_intf{};
    sys_intf_t sys_intf{};
    //seq_id_ex_intf_t seq_id_ex_intf;
    //ctrl_if_ex_t ctrl_if_ex{};

    if_intf_t if_intf{};
    id_intf_t id_intf;


    // LOGIC    
    // if_stage if_stage(if_stage_intf);
    // imem in level up hier

    // if_id_pipeline(if_stage_intf, id_stage_intf);

    // id_stage id_stage(seq_if_id_intf, seq_id_ex_intf)
            control control; // incl control into id stage
            //control control(ctrl_intf); // incl control into id stage

    // ex_stage ex_stage
    // id_ex_pipeline(id_stage_intf, ex_stage_intf);
    //      id_stage_intf.update();
    //      


    // mem_stage mem_stage
    // dmem in level up hier

    // wb_stage wb_stage

    // framework stuff, probably to be moved to main
    // seq_queue queue;

public:
    uint32_t pc_mock = 0;

private:

    // control
    void ex_stage();
    void alu();
    void init(seq_queue *q);    // initialize all signals within structures (names and initial/reset values)

public:
    core() = delete;
    core(seq_queue *q);
    void reset(bool rst_in);
    void update(std::array<uint32_t, IMEM_SIZE> &imem_ptr, std::array<uint32_t, DMEM_SIZE> &dmem_ptr);

};
