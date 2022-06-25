#pragma once

#include "defines.h"
#include "logic_t.h"
#include <vector>

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
    friend logic_t;
};
