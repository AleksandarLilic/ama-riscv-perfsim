#include "../include/seq_queue.h"

void seq_queue::add(logic_t *ptr)
{
    queue.push_back(ptr);
}

void seq_queue::update()        // separate into 2 calls for multiple queues in the design, make q of q
{
    for (logic_t *i : queue)
        i->clk_update_hold();
    for (logic_t* i : queue)
        i->clk_update();
}

void seq_queue::reset()
{
    for (logic_t *i : queue)
        i->rst();
}

