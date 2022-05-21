#include "../include/seq_queue.h"

void seq_queue::add(logic_t* ptr)
{
    queue.push_back(ptr);
}
void seq_queue::update() 
{
    for (logic_t* i : queue)
        i->m_clk_update();
}