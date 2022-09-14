#include "defines.h"
#include "seq_queue.h"

#include "perf_cpu.h"

#include "cpu.h"

//#include <algorithm>
//#include <iterator>
#include <fstream>

//#define TEST
#ifndef TEST

uint32_t global_inst_to_ctrl = 0;

#if RISCV_SANITY_TESTS
bool global_test_failed = 0;
uint32_t global_inst_count = 0;
std::vector<uint32_t> global_issued_instructions;
std::vector<uint32_t> global_committed_instructions;
#endif;

#if RISCV_ISA_REGR
uint32_t *global_tohost_ptr;
std::string global_test_name;
uint32_t *global_wb_inst_ptr;
#define CLK_TIMEOUT 1000 // per test
#endif

bool rst_done = 0;

void queue_update_all(seq_queue *q)
{
    LOG("\n ----- Running queue update:\n");
    q->update_hold();
    q->update();
    if (rst_done) perf_cpu::update_clk();
    LOG("\n ----- Queue update finished \n");
}

#if RISCV_SANITY_TESTS
void check_committed_instructions()
{
    LOG_M("\n ----- Instruction match ----- ");
    uint32_t i = 0;
    for (uint32_t inst : global_committed_instructions) {
        if (inst != global_issued_instructions[i]) {
            LOGE(" FAIL ");
            LOGE("Issued instruction different from committed instruction at count: " << i);
            LOGE("Issued instruction:   " << FHEX(global_issued_instructions[i]));
            LOGE("Committed instruction: " << FHEX(inst));
            return;
        }
        i++;
    }
    LOG_M(" PASS ");
}
#endif

void stim_update(uint64_t stim, uint64_t lvl, std::ofstream* log)
{
    stim = uint64_t(lvl);
    *log << stim << std::endl;
}

int main()
{
    // perf counters    
    uint32_t regr_perf_array[PERF_ARRAY_SIZE]{};

    uint32_t regr_test_status = 0;
    std::cout.precision(4);

    // log to file
    bool open_test_log = 0;
    open_test_log = freopen("test_log.txt", "w", stdout);    
    if (!open_test_log) {
        LOGE("Failed to open test.txt log. Exiting...");
        std::cin.get();
        return 1;
    }

    // wb inst write
    std::ofstream cycle_log;
    cycle_log.open("cycle_log.txt");

    // stimuli
    std::ofstream stim_clk;
    stim_clk.open("stim_clk.txt");
    uint64_t clk = 1;

    std::ofstream stim_rst;
    stim_rst.open("stim_rst.txt");

    // reset vector table
    std::ofstream vector_table;
    vector_table.open("vector_table.txt");
    vector_table << "";

    // simulation parameters
    uint32_t rst_cycles = 4;

    // needs at least 5 for pipeline, +1 clk for each stall (jump and branch); can be more
    const uint32_t clk_cycles_to_empty_pipeline = 5 + 39 + 10;
    uint32_t clk_cycles = 40 + clk_cycles_to_empty_pipeline;
#if RISCV_SANITY_TESTS
    clk_cycles = global_inst_count + clk_cycles_to_empty_pipeline;
#endif
    
    uint32_t regr_cnt = 0;
    uint32_t regr_tests = RISCV_ISA_REGR_NUM;

    if (SINGLE_TEST == 1)
        regr_tests = 1;

    uint32_t regr_clk_counter = 0;
    
    LOG_M(" ----- Regression Start -----");
    while(regr_cnt < regr_tests){

    perf_cpu::reset_clk();
    LOG_M("\n ----- Create CPU instance -----\n");
    seq_queue q;
    cpu *cpu0 = new cpu(&q);

    if (SINGLE_TEST == 1)
        global_test_name = SINGLE_TEST_NAME;
    else
        global_test_name = riscv_regr_tests[regr_cnt];

    LOG_M("\n\n ----- Test name: " << global_test_name << " ----- " << "\n");
    regr_cnt++;
    uint32_t clk_counter = 0;
    uint32_t rst_counter = 0;
    cpu0->burn_mem();

    LOG_M(" ----- Simulation Start -----");

    // initial toggle, i.e. the non functional one
    stim_update(clk, 1, &stim_clk);
    stim_update(clk, 0, &stim_clk);

    // Reset
    cpu0->reset(reset_t::set);
    do {
        stim_rst << 1 << std::endl;
        LOG_M("CPU in reset");
        cpu0->update();
        stim_update(clk, 1, &stim_clk);
        queue_update_all(&q);
        rst_counter++;
        clk_counter++;
        LOG_M("\n\n ---------- Cycle count in reset: " << (rst_counter) << " ---------- ");
        cycle_log << "clk: " << regr_clk_counter + clk_counter << "; Inst WB: " << FHEXI(*global_wb_inst_ptr) << std::endl;
        stim_update(clk, 0, &stim_clk);
    } while (rst_cycles > rst_counter);

    cpu0->reset(reset_t::clear);
    LOG_M("CPU reset done");
    rst_done = 1;

    // Run
#if RISCV_SANITY_TESTS
    while ((clk_cycles > clk_counter) && !global_test_failed) {
        cpu0->update();
        LOG_M("\n\n ---------- Cycle count: " << (clk_counter + rst_counter) << " ---------- ");
        queue_update_all(&q);
        clk_counter++;
    }

    LOG_M("\n ----- Simulation Status -----");
    if (global_test_failed)
        LOGE(" FAIL ");
    else
        LOG_M(" PASS ");
    
    check_committed_instructions();

#elif RISCV_ISA_REGR
    do {
        stim_rst << 0 << std::endl;
        stim_update(clk, 1, &stim_clk);
        cpu0->update();
        queue_update_all(&q);
        clk_counter++;
        cycle_log << "clk: " << regr_clk_counter + clk_counter << "; Inst WB: " << FHEXI(*global_wb_inst_ptr) << std::endl;
        LOG_M("\n\n ---------- Cycle count: " << (clk_counter) << " ---------- ");
        stim_update(clk, 0, &stim_clk);
    } while ((*global_tohost_ptr & 0x1) != 1u && (clk_counter < CLK_TIMEOUT));

    bool test_status = 0;
    uint32_t failed_test_id = 0;
    
    if (*global_tohost_ptr == 1) {
        test_status = true;
    }
    else {
        test_status = false;
        failed_test_id = *global_tohost_ptr >> 1;
    }

    uint32_t clk_pipe = 6;
    while (clk_pipe) {
        stim_rst << 0 << std::endl;
        stim_update(clk, 1, &stim_clk);
        LOG_M("passing_pipe");
        cpu0->update();
        queue_update_all(&q);
        clk_counter++;
        clk_pipe--;
        cycle_log << "clk: " << regr_clk_counter + clk_counter << "; Inst WB: " << FHEXI(*global_wb_inst_ptr) << std::endl;
        LOG_M("\n\n ---------- Cycle countP: " << (clk_counter) << " ---------- ");
        stim_update(clk, 0, &stim_clk);
    }

#else
    while (clk_cycles > clk_counter) {
        cpu0->update();
        queue_update_all(&q);
        clk_counter++;
    }
#endif

    LOG_M("\n ----- Simulation Stats -----\n");
#if RISCV_SANITY_TESTS
    LOG_M("Clock cycles to execute: " << clk_cycles);
#endif
#if RISCV_ISA_REGR
    if (test_status)
        LOG_M("Test passed; Test suite: " << global_test_name);
    else
        LOGE("Test failed. Test ID: " << failed_test_id << "; Test suite: " << global_test_name);
    
    LOG_M("");
#endif

    regr_test_status += (!test_status);

    uint32_t perf_array[PERF_ARRAY_SIZE];
    perf_cpu::collect_data(perf_array);
    perf_cpu::status_log(perf_array);

    //std::add(std::begin(perf_array), std::end(perf_array), std::begin(regr_perf_array));

    for (uint32_t i = 0; i < std::size(perf_array); i++)
        regr_perf_array[i] += perf_array[i];


    LOG_M("\nClock cycles executed: " << clk_counter);
    LOG_M("\n ----- Simulation End -----\n");

    delete cpu0;

    regr_clk_counter += clk_counter;
    
    } // while (regr_cnt < RISCV_ISA_REGR_NUM)
    
    LOG_M("\n ----- Regression results -----\n");

    if (regr_test_status != 0)
        LOGE("Regression Failed. Number of failed tests: " << regr_test_status << 
            ". Check log for details about failed tests\n");

    perf_cpu::status_log(regr_perf_array);

    LOG_M("\n ----- Regression End -----");
    
    cycle_log.close();
    fclose(stdout);
    //std::cin.get();
}
#else
int main()
{
    uint32_t din = 0xFF;
    uint32_t we = 0b0100;

    uint32_t mask[4][2] = {
            {0x0, 0x0000'00FF},
            {0x0, 0x0000'FF00},
            {0x0, 0x00FF'0000},
            {0x0, 0xFF00'0000} };

    //LOG("mask[0][0]: " << FHEX(mask[0][0]));
    //LOG("mask[1][0]: " << FHEX(mask[1][0]));
    //LOG("mask[2][0]: " << FHEX(mask[2][0]));
    //LOG("mask[3][0]: " << FHEX(mask[3][0]));
    //LOG("mask[0][1]: " << FHEX(mask[0][1]));
    //LOG("mask[1][1]: " << FHEX(mask[1][1]));
    //LOG("mask[2][1]: " << FHEX(mask[2][1]));
    //LOG("mask[3][1]: " << FHEX(mask[3][1]));

    //LOG("we 3: " << (we & 0x4));
    //uint32_t we3 = we & 0x4;
    //LOG("we 3: " << (we & 0x4));
    //LOG("mask[0][(we & 0x1)]: " << FHEX(mask[0][(we & 0x4)]));
    //LOG("mask[0][(we & 0x1)]: " << FHEX(mask[0][we3]));

    //uint32_t din0 = din & (mask[0][(we & 0x1)]);
    //LOG("din & mask[0][(we & 0x1)]: " << din0);

    //uint32_t din1 = din & (mask[1][(we & 0x2)]);
    //LOG("din & mask[1][(we & 0x2)]: " << din1);

    ////uint32_t din2 = din & (mask[2][(we & 0x4)]);
    //uint32_t din2 = din & (mask[2][(we3)]);
    //LOG("din & mask[2][(we & 0x4)]: " << din2);

    //uint32_t din3 = din & (mask[3][(we & 0x8)]);
    //LOG("din & mask[3][(we & 0x8)]: " << din3);

    uint32_t in = din & mask[0][(we & 0x1)] |
        din & mask[1][(we & 0x2) >> 1] |
        din & mask[2][(we & 0x4) >> 2] |
        din & mask[3][(we & 0x8) >> 3];
    
    LOG("in: " << in);
    std::cin.get();
    return 1;    
/*
    uint32_t funct3 = 0b100;


    uint32_t load_width = funct3 & 0b11;
    //uint32_t offset = mem_intf->alu_mem & 0b11;
    uint32_t offset = 2;
    uint32_t load_sign_bit = (funct3 & 0b100) >> 2;

    uint32_t unaligned_access = (
        (load_width == 0b01 && offset == 3) ||  // half out of bounds
        (load_width == 0b11 && offset != 0));   // word out of bounds
    
    if (unaligned_access) {
        LOGE("DMEM unaligned access not supported");
        std::cin.get();
        return 1;
    }

    uint32_t out = 0;
    //uint32_t dmem_out = 0x3A2A1A0A;
    uint32_t dmem_out = 0x80A50000;

    uint32_t load_width_bytes = load_width << 3;

    uint32_t mask = 0xFF;
    uint32_t get_data_sign = 0x80 << load_width_bytes;
    uint32_t sign_mask = 0x0;
    LOG("mask mask_data_sign: " << FHEX(get_data_sign));
    mask = ~(~mask << load_width_bytes);  // fill with 1s from right
    LOG("mask before offset: " << FHEX(mask));
    sign_mask = ~mask;
    LOG("sign mask: " << FHEX(sign_mask));
    mask = mask | ((mask << 8) & 0xFF00'0000);  // add byte[3] if word
    LOG("mask after word align: " << FHEX(mask));

    uint32_t offset_bytes = offset << 3;

    mask = mask << offset_bytes;

    out = (dmem_out & mask) >> offset_bytes;


    LOG("mask: " << FHEX(mask));
    LOG("load_width_bytes: " << load_width_bytes);
    LOG("dmem_out: " << FHEX(dmem_out));
    LOG("out: " << FHEX(out) << ", " << int32_t(out));

    uint32_t data_neg = out & get_data_sign;
    LOG("data_neg: " << FHEX(data_neg));

    if (load_sign_bit && data_neg)
        out |= sign_mask;
    LOG("out: " << FHEX(out) << ", " << int32_t(out));

    std::cin.get();
*/
}

#endif
