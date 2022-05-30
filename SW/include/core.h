#pragma once

#include "defines.h"
#include "inst_field.h"
#include "intf.h"
#include "seq_queue.h"
#include "logic_t.h"
#include "cl.h"
#include <array>

class core
{
private:
    // control interfaces
    ctrl_intf_t ctrl_intf{};

    //ctrl_if_ex_t ctrl_if_ex;
    sys_intf_t sys_intf;
    seq_queue queue;

    //datapath signals
    logic_t alu_ex;

public:

private:
    void init();    // initialize all signals within structures (names and initial/reset values)

    // control
    void control(ctrl_intf_t *ctrl_intf);
    void control_op_fwd(ctrl_intf_t* ctrl_intf);
    void control_pipeline_ctrl(ctrl_intf_t* ctrl_intf);
    void control_store_mask(ctrl_intf_t* ctrl_intf);
    void control_branch_resolution(ctrl_intf_t* ctrl_intf);
    // decoder instructions
    void decode(ctrl_intf_t *ctrl_intf);

    void decode_r_type(ctrl_intf_t *ctrl_intf);
    void decode_i_type(ctrl_intf_t* ctrl_intf);
    void decode_load(ctrl_intf_t* ctrl_intf);
    void decode_store(ctrl_intf_t* ctrl_intf);
    void decode_branch(ctrl_intf_t* ctrl_intf);
    void decode_jalr(ctrl_intf_t* ctrl_intf);
    void decode_jal(ctrl_intf_t* ctrl_intf);
    void decode_lui(ctrl_intf_t* ctrl_intf);
    void decode_auipc(ctrl_intf_t* ctrl_intf);
    void decode_system(ctrl_intf_t* ctrl_intf);
    void decode_unsupported(ctrl_intf_t* ctrl_intf);
    void decode_reset(ctrl_intf_t* ctrl_intf);

    void ex_stage();
    void alu();
    // all update:
    void cl_update();
    void seq_update();

public:
    core();
    void reset(bool rst_in);
    void update(std::array<uint32_t, IMEM_SIZE> &imem_ptr, std::array<uint32_t, DMEM_SIZE> &dmem_ptr)
    {
        control(&(this->ctrl_intf));
        //LOG("imem from core: " << imem_ptr[0] << "\ndmem from core: " << dmem_ptr[0]);
        //imem_ptr[1] = 55;
        //decode(&ctrl_intf, &ctrl_intf_private);
    }

};
