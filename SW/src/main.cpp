#include "defines.h"
#include "seq_queue.h"

#include "cpu.h"
//#define TEST
#ifndef TEST

uint32_t global_inst_to_ctrl = 0;
#if RISCV_SANITY_TESTS
bool global_test_failed = 0;
#endif;

void queue_update_all(seq_queue *q)
{
    LOG("\n\n------ Running queue update:\n");
    q->update_hold();
    q->update();
    LOG("\n------ Queue update finished \n\n");
}

int main()
{
    uint32_t rst_count = 1;
    //                       rst       + r + i + l + s + b + j + u + inv;
    uint32_t clk_count = (rst_count+1) + 10 + 9 + 5 + 3 + 6 + 2 + 2 + 1;
    //                      dd  dummy  NOP
    clk_count = clk_count + 3  + 3    + 5 ;
    //clk_count = 16;
    clk_count = 32;
    //clk_count = 40;

    seq_queue q;
    cpu *cpu0 = new cpu(&q);

    LOG(" ----- Simulation Start -----");

    // Reset
    cpu0->reset(reset_t::set);
    while (rst_count) {
        cpu0->update();
        queue_update_all(&q);
        rst_count--;
    }
    cpu0->reset(reset_t::clear);

    // Run
    while (clk_count && !global_test_failed) {
        cpu0->update();
        queue_update_all(&q);
        clk_count--;
    }

    if (global_test_failed)
        LOG("\n ----- Simulation ending due to failiure ----- \n");

    LOG(" ----- Simulation End -----");
    delete cpu0;
    std::cin.get();
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