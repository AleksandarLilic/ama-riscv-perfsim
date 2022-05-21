#include "defines.h"
#include "logic_t.h"
#include "cl.h"
#include "seq_queue.h"
#include "core.h"
#include "memory.h"
#include <array>

void main()
{
    logic_t test(5u);
    test.m_clk_update();
    LOG(test.m_get());

    uint32_t f1 = 0xAA005500 >> 15;
    uint32_t f2 = 55;

    uint32_t res = 0;
    res = cl::mux2(1u, f1, f2);

    LOG("res: " << res);
    res = cl::mux4(0u, f1, 22u, CL_UNUSED, 44u);
    LOG("res: " << res);
    
    res = cl::mux4(2u, 11u, 22u, CL_UNUSED, 44u);
    LOG("res: " << res);
    
    res = cl::mux4(15u, 11u, 22u, CL_UNUSED, 44u);
    LOG("res: " << res);

    res = cl::mux8(7u, 11u, 22u, CL_UNUSED, 44u, 55u, 66u, CL_UNUSED, CL_UNUSED);
    LOG("res: " << res);

    res = cl::mux8(9u, 11u, 22u, CL_UNUSED, 44u, 55u, 66u, CL_UNUSED, CL_UNUSED);
    LOG("res: " << res);

    seq_queue q;
    logic_t reg1(5, "reg1");
    logic_t reg2(22, "reg2");

    q.add(&reg1);
    q.add(&reg2);

    reg1 = 17;
    reg2 = 44;

    q.update();

    // -------------------------- cpu ideas:

    core core;
    std::array<uint32_t, IMEM_SIZE> imem{};
    std::array<uint32_t, DMEM_SIZE> dmem{};

    imem[0] = 55;

    core.m_reset(1);
    core.m_update(imem, dmem);
    // core.update();
    core.m_reset(0);

    LOG("imem[1], written inside func: " << imem[1]);
    LOG("dmem " << dmem[2]);

    /*
    
    core.m_seq_update()
        //----- MEM/WB stage updates
        dut_m_reg_file_write_update();
        dut_m_csr_write_update();
        dut_m_mem_wb_pipeline_update();

        //----- EX/MEM stage updates
        dut_m_load_sm_seq_update();
        dut_m_ex_mem_pipeline_update();

        //----- ID/EX stage updates
        dut_m_imm_gen_seq_update();
        dut_m_id_ex_pipeline_update();
        dut_m_rst_sequence_update();

        //----- IF/ID stage updates
        dut_m_imem_update();
        dut_m_pc_update();
        dut_m_if_pipeline_update();

    
    */




    std::cin.get();
}