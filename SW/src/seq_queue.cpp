#include "../include/seq_queue.h"

void seq_queue::add(logic_t* ptr)
{
    queue.push_back(ptr);
}
void seq_queue::update() 
{
    for (auto i : queue)
        i->m_clk_update();
}