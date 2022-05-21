#pragma once

#include "defines.h"
#include "inst_field.h"
#include "interfaces.h"
#include "seq_queue.h"
#include "logic_t.h"
#include <array>
//#include "memory.h"

class core
{
private:
    ctrl_conn_t ctrl_conn_id;
    //ctrl_if_ex_t ctrl_if_ex;
    sys_conn_t sys_conn;
    seq_queue queue;

    //datapath signals
    logic_t alu_ex;

public:

private:
    void init();    // initialize all signals within structures (names and initial/reset values)

    void m_decode();

    void ex_stage();
    void m_alu();
    // all update:
    void m_cl_update();
    void m_seq_update();

public:
    core();
    void m_reset(bool rst_in);
    void m_update(std::array<uint32_t, IMEM_SIZE>& imem_ptr, std::array<uint32_t, DMEM_SIZE>& dmem_ptr)
    {
        LOG("imem from core: " << imem_ptr[0] << "\ndmem from core: " << dmem_ptr[0]);
        imem_ptr[1] = 55;
    }

};
