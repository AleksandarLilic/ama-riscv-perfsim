#pragma once

#include <vector>
#include "defines.h"
#include "logic_t.h"

class seq_queue
{
private:
    std::vector<logic_t*> queue;
public:
    void add(logic_t* ptr);
    void update();
    void reset();
};