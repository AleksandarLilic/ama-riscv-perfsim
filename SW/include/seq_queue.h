#pragma once

#include <vector>
#include "defines.h"
#include "logic_t.h"

class seq_queue
{
private:
    std::vector<logic_t*> queue;
public:
    void update_hold();
    void update();
    void reset();
private:
    void add(logic_t *ptr);
    //friend void logic_t::enqueue(seq_queue *q);
    friend logic_t;
};

