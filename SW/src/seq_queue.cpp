#include "../include/seq_queue.h"

void seq_queue::add(logic_t *ptr)
{
    queue.push_back(ptr);
}

void seq_queue::update_hold()
{
    for (logic_t *i : queue)
        i->update_hold();
}

void seq_queue::update()
{
    for (logic_t *i : queue)
        i->update();
}
seq_queue::~seq_queue()
{
    for (logic_t *i : queue)
        delete i;
}