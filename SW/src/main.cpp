#include "defines.h"
#include "logic_t.h"
#include "cl.h"
#include "seq_queue.h"
#include "core.h"
#include "memory.h"
#include "intf_cfg.h"

#include <array>

#define TEST 0

#if TEST
//typedef struct
//{
//    uint32_t init;
//    std::string name;
//} log_cfg;
//
//
//log_cfg cfg1[] = {
//    {0, "pc"},
//    {0, "pc_sel"},
//    {0, "we_en"},
//    {0, "reg_we"},
//    {0, "b_uns"}
//};
//
//void initw()
//{
//    seq_queue q;
//    for (uint32_t i = 0; i < sizeof(cfg1) / sizeof(*cfg1); ++i) {
//        logic_t pc();
//        q.add(pc);
//    };
//}
#endif

void queue_update_all(seq_queue *q)
{
    // TODO:
    //std::vector<queue*> all_queues;
    //for (queue *i : all_queues)
    //    i->update_hold();
    //for (queue *i : all_queues)
    //    i->update();

    LOG("\n\n------ Running queue update:\n");
    q->update_hold();
    q->update();
    LOG("\n------ Queue update finished \n\n");
}


int main()
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
    seq_queue *qp = &q;
    uint32_t reg1_out;
    uint32_t reg2_out;
    logic_t reg1(qp, 5u, "reg1");
    logic_t reg2(qp, 22u, "reg2");

    uint32_t conn_test;
    reg1.connect(&conn_test);
    LOG("conn_test: " << conn_test);
    uint32_t conn_test2;
    reg1.connect(&conn_test2);
    LOG("conn_test2: " << conn_test2);

    //q.add(&reg1);
    //q.add(&reg2);

    reg1 = 17;
    reg2 = 44;

    q.update();
    LOG("conn_test after update: " << conn_test);
    LOG("conn_test2 after update: " << conn_test2);

    //intf_ex_stage{mux_sel_09};

    res = cl::mux4(1u, reg1.out(), reg2.out(), CL_UNUSED, 0u);
    LOG("res: " << res);

    logic_t ff1(qp, 5, "ff1");
    logic_t ff2(qp, 22, "ff2");

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


    //initw();


#else
    // cpu
    //                  rst + r + i + l + s + b + j + u + inv;
    uint32_t clk_count = 2 + 10 + 9 + 5 + 3 + 6 + 2 + 2 + 1;
    //                      dd  dummy
    clk_count = clk_count + 3  + 3;
    //clk_count = 6;

    seq_queue q;
    std::array<uint32_t, IMEM_SIZE> imem{};
    uint32_t imem_dout;
    std::array<uint32_t, DMEM_SIZE> dmem{};
    uint32_t dmem_dout;
    core core(&q, &imem_dout, &dmem_dout);

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
    imem[27] = 0xfab50ee3;  imemc[27] = "beq   x10,x11,4000005c <loop>";
    imem[28] = 0xfab51ce3;  imemc[28] = "bne   x10,x11,4000005c <loop>";
    imem[29] = 0xfab54ae3;  imemc[29] = "blt   x10,x11,4000005c <loop>";
    imem[30] = 0xfab558e3;  imemc[30] = "bge   x10,x11,4000005c <loop>";
    imem[31] = 0xfab566e3;  imemc[31] = "bltu  x10,x11,4000005c <loop>";
    imem[32] = 0xfab574e3;  imemc[32] = "bgeu  x10,x11,4000005c <loop>";
    imem[33] = 0x00458567;  imemc[33] = "jalr    x10,4(x11)";
    imem[34] = 0xfa1ff06f;  imemc[34] = "jal     x0,4000005c <loop>";
    imem[35] = 0x01000537;  imemc[35] = "lui     x10,0x1000";
    imem[36] = 0x01000517;  imemc[36] = "auipc   x10,0x1000";
    imem[37] = 0x01000500;  imemc[37] = "invalid instr";
    imem[38] = 0x00628433;  imemc[38] = "add     x8,x5,x6";
    imem[39] = 0x00f40593;  imemc[39] = "addi    x11,x8,15";
    imem[40] = 0x06340613;  imemc[40] = "addi    x12,x8,99";
    imem[41] = 0x06340613;  imemc[41] = "addi    x12,x8,99";
    imem[42] = 0x06340613;  imemc[42] = "addi    x12,x8,99";
    imem[43] = 0x06340613;  imemc[43] = "addi    x12,x8,99";

    core.reset(1);
    core.update_fe();
    imem_dout = imem[core.if_intf.imem_addr];
    core.update();
    
    LOG("---------- inst in IF stage: " << FHEX(imem_dout) <<
        "; ASM: " << imemc[core.if_intf.imem_addr]);
    
    queue_update_all(&q);
    clk_count--;
    core.reset(0);

    while (clk_count) {
        core.update_fe();
        imem_dout = imem[core.if_intf.imem_addr];
        core.update();
        
        LOG("---------- inst in IF stage: " << FHEX(imem_dout) <<
            "; ASM: " << imemc[core.if_intf.imem_addr]);
        
        queue_update_all(&q);
        clk_count--;
        //if (clk_count==35)core.reset(0);
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
