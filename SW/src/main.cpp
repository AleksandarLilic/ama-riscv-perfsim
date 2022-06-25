#include "defines.h"
#include "seq_queue.h"

#include "cpu.h"

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
    clk_count = 6;

    seq_queue q;
    cpu cpu(&q);

    LOG(" ----- Simulation Start -----");

    // Reset
    cpu.reset(reset_t::set);
    while (rst_count) {
        cpu.update();
        queue_update_all(&q);
        rst_count--;
    }
    cpu.reset(reset_t::clear);

    // Run
    while (clk_count) {
        cpu.update();
        queue_update_all(&q);
        clk_count--;
    }

    LOG(" ----- Simulation End -----");
    std::cin.get();
}
