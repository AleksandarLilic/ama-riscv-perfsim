#pragma once

#include "defines.h"

namespace cl
{
    uint32_t mux2(uint32_t sel, uint32_t in0, uint32_t in1);
    uint32_t mux4(uint32_t sel, uint32_t in0, uint32_t in1, uint32_t in2, uint32_t in3);
    uint32_t mux8(uint32_t sel, uint32_t in0, uint32_t in1, uint32_t in2, uint32_t in3,
        uint32_t in4, uint32_t in5, uint32_t in6, uint32_t in7);
}