#include "defines.h"
#include "logic_t.h"
#include "cl.h"
#include "seq_queue.h"
#include "core.h"
#include "memory.h"
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


uint32_t pc_input = 0u;
uint32_t pc_output;
uint32_t stall_if;
uint32_t ff_0_output;
uint32_t csr_we;
uint32_t ff_1_output;
uint32_t csr_ui;
uint32_t ff_2_output;
uint32_t ff_3_input;
uint32_t ff_3_output;
uint32_t ff_4_input;
uint32_t ff_4_output;
uint32_t ff_5_input;
uint32_t ff_5_output;
uint32_t ff_6_input;
uint32_t ff_6_output;
uint32_t ff_7_input;
uint32_t ff_7_output;
uint32_t ff_8_input;
uint32_t ff_8_output;
uint32_t ff_9_input;
uint32_t ff_9_output;
uint32_t ff_10_input;
uint32_t ff_10_output;
uint32_t ff_11_input;
uint32_t ff_11_output;
uint32_t ff_12_input;
uint32_t ff_12_output;
uint32_t ff_13_input;
uint32_t ff_13_output;
uint32_t ff_14_input;
uint32_t ff_14_output;
uint32_t ff_15_input;
uint32_t ff_15_output;
uint32_t ff_16_input;
uint32_t ff_16_output;
uint32_t ff_17_input;
uint32_t ff_17_output;

typedef struct
{
    std::string id;
    uint32_t init;
    uint32_t * input;
    uint32_t * output;

}ff_init_cfg_t;

#define     NR_FF   19u
ff_init_cfg_t ff_init_cfg[NR_FF] =
{
    { "PC", 0, &pc_input, &pc_output},
    { "FF_0", 0, &stall_if, &ff_0_output },
    { "FF_1", 0, &csr_we, &ff_1_output },
    { "FF_2", 0, &csr_ui, &ff_2_output },
    { "FF_3", 0, &ff_3_input, &ff_3_output },
    { "FF_4", 0, &ff_4_input, &ff_3_input },
    { "FF_5", 0, &ff_5_input, &ff_5_output },
    { "FF_6", 0, &ff_6_input, &ff_6_output },
    { "FF_7", 0, &ff_7_input, &ff_7_output },
    { "FF_8", 0, &ff_8_input, &ff_8_output },
    { "FF_9", 0, &ff_5_output, &ff_9_output },
    { "FF_10", 0, &ff_6_output, &ff_10_output },
    { "FF_11", 0, &ff_11_input, &ff_11_output },
    { "FF_12", 0, &ff_12_input, &ff_12_output },
    { "FF_13", 0, &ff_13_input, &ff_13_output },
    { "FF_14", 0, &ff_14_input, &ff_14_output },
    { "FF_15", 0, &ff_15_input, &ff_15_output },
    { "FF_16", 0, &ff_16_input, &ff_16_output },
    { "FF_17", 0, &ff_17_input, &ff_17_output },
};

seq_queue q;
seq_queue* qp = &q;
uint32_t mux1_sel = 0u;
uint32_t mux1_out = 0u;
uint32_t imem[] = {0xFFFFFFFF, 0x0, 0xFFFFFFFF, 0x0};
void init()
{
    uint32_t i = 0u;
    logic_t* ff;
    for (i = 0u; i < NR_FF; i++)
    {
        ff = new logic_t(qp, ff_init_cfg[i].init, ff_init_cfg[i].id);
        ff->connect(ff_init_cfg[i].output);
        ff->connect_in(ff_init_cfg[i].input);
    }

}
int main()
{

    init();


    mux1_out = cl::mux2(mux1_sel, imem[pc_output], 0);
    ff_4_input = (mux1_out >> 15) & 0x1F;
    ff_5_input = (mux1_out >> 7) & 0x1F;

    LOG("mux1_out: " << mux1_out);
    pc_input += 1;

    q.update();

    pc_input +=1;
    mux1_out = cl::mux2(mux1_sel, imem[pc_output], 0);
    ff_4_input = (mux1_out >> 15) & 0x1F;
    ff_5_input = (mux1_out >> 7) & 0x1F;
    LOG("mux1_out: " << mux1_out);

    q.update();

    pc_input += 1;
    mux1_out = cl::mux2(mux1_sel, imem[pc_output], 0);
    ff_4_input = (mux1_out >> 15) & 0x1F;
    ff_5_input = (mux1_out >> 7) & 0x1F;
    LOG("mux1_out: " << mux1_out);

    q.update();

    pc_input += 1;
    mux1_out = cl::mux2(mux1_sel, imem[pc_output], 0);
    ff_4_input = (mux1_out >> 15) & 0x1F;
    ff_5_input = (mux1_out >> 7) & 0x1F;
    LOG("mux1_out: " << mux1_out);


    q.update();
    //LOG("conn_test2 after update: " << conn_test2);

    //intf_ex_stage{mux_sel_09};

    //res = cl::mux4(1u, reg1.out(), reg2.out(), CL_UNUSED, 0u);
   
}