#include "defines.h"
#include "logic_t.h"
#include "cl.h"
#include "seq_queue.h"
#include "core.h"
#include "memory.h"
#include <array>

#define TEST 0

void main()
{
#if TEST
    logic_t test();
    //test.clk_update();
    //LOG(test.get());

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
    uint32_t reg1_out;
    uint32_t reg2_out;
    logic_t reg1(5, "reg1");
    logic_t reg2(22, "reg2");

    uint32_t conn_test;
    reg1.connect(&conn_test);
    LOG("conn_test: " << conn_test);
    uint32_t conn_test2;
    reg1.connect(&conn_test2);
    LOG("conn_test2: " << conn_test2);

    q.add(&reg1);
    q.add(&reg2);

    reg1 = 17;
    reg2 = 44;

    q.update();
    LOG("conn_test after update: " << conn_test);
    LOG("conn_test2 after update: " << conn_test2);

    res = cl::mux4(1u, reg1.out(), reg2.out(), CL_UNUSED, 0u);
    LOG("res: " << res);

    logic_t ff1(5, "ff1");
    logic_t ff2(22, "ff2");

    q.add(&ff1);
    q.add(&ff2);
    LOG("ff1: " << ff1.out());
    LOG("ff2: " << ff2.out());

    ff1 = 99;
    // check this, does not work 
    ff2 = ff1;
    //ff2 = ff1.out();


    q.update();

    LOG(ff1.get_name() << " on update: " << ff1.out());
    LOG(ff2.get_name() << " on update: " << ff2.out());

    LOG(ff2);



#else
    // -------------------------- cpu ideas:
    uint32_t clk_count = 1+10+9+5+3;
    core core;
    std::array<uint32_t, IMEM_SIZE> imem{};
    std::array<uint32_t, DMEM_SIZE> dmem{};

    std::array<std::string, IMEM_SIZE> imemc{};

    imem[0]  = 0x00c58533;  imemc[0]  = "add   x10,x11,x12";
    imem[1]  = 0x40c58533;  imemc[1]  = "sub   x10,x11,x12";
    imem[2]  = 0x00c59533;  imemc[2]  = "sll   x10,x11,x12";
    imem[3]  = 0x00c5d533;  imemc[3]  = "srl   x10,x11,x12";
    imem[4]  = 0x40c5d533;  imemc[4]  = "sra   x10,x11,x12";
    imem[5]  = 0x00c5a533;  imemc[5]  = "slt   x10,x11,x12";
    imem[6]  = 0x00c5b533;  imemc[6]  = "sltu  x10,x11,x12";
    imem[7]  = 0x00c5c533;  imemc[7]  = "xor   x10,x11,x12";
    imem[8]  = 0x00c5e533;  imemc[8]  = "or    x10,x11,x12";
    imem[9]  = 0x00c5f533;  imemc[9]  = "and   x10,x11,x12";
    imem[10] = 0x02558513;  imemc[10] = "addi  x10,x11,37";
    imem[11] = 0x0255a513;  imemc[11] = "slti  x10,x11,37";
    imem[12] = 0x0255b513;  imemc[12] = "sltiu x10,x11,37";
    imem[13] = 0x0255c513;  imemc[13] = "xori  x10,x11,37";
    imem[14] = 0x0255e513;  imemc[14] = "ori   x10,x11,37";
    imem[15] = 0x0255f513;  imemc[15] = "andi  x10,x11,37";
    imem[16] = 0x00359513;  imemc[16] = "slli  x10,x11,0x3";
    imem[17] = 0x0035d513;  imemc[17] = "srli  x10,x11,0x3";
    imem[18] = 0x4035d513;  imemc[18] = "srai  x10,x11,0x3";
    imem[19] = 0x00458503;  imemc[19] = "lb    x10,4(x11)";
    imem[20] = 0x00459503;  imemc[20] = "lh    x10,4(x11)";
    imem[21] = 0x0045a503;  imemc[21] = "lw    x10,4(x11)";
    imem[22] = 0x0045c503;  imemc[22] = "lbu   x10,4(x11)";
    imem[23] = 0x0045d503;  imemc[23] = "lhu   x10,4(x11)";
    imem[24] = 0x00a58223;  imemc[24] = "sb    x10,4(x11)";
    imem[25] = 0x00a59223;  imemc[25] = "sh    x10,4(x11)";
    imem[26] = 0x00a5a223;  imemc[26] = "sw    x10,4(x11)";
    //imem[27] = 0x
    //imem[28] = 0x
    //imem[29] = 0x

    core.reset(1);
    LOG("\n---------- inst fetched: " << imemc[core.pc_mock]);
    core.update(imem, dmem);
    clk_count--;
    // core.update();
    core.reset(0);

    while (clk_count) {
        LOG("\n---------- inst fetched: " << imemc[core.pc_mock]);
        core.update(imem, dmem);
        clk_count--;
    }


#endif

    /*
    
    core.seq_update()
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