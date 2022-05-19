#include "../include/cl.h"

constexpr auto W_MSG = "Potential overflow on the select signal";

uint32_t cl::mux2(uint32_t sel, uint32_t in0, uint32_t in1)
{
    uint32_t mask = 0x01;
    uint32_t res = (sel & mask) ? in1 : in0;
    if (sel & ~mask)
        LOGW(W_MSG << " for mux2; sel = " << sel << "; in0 = " << in0 << "; in1 = " << in1 << 
            "; selected input: " << (sel & mask) << "; out: " << res << "; ");
    return res ;
}

uint32_t cl::mux4(uint32_t sel, uint32_t in0, uint32_t in1, uint32_t in2, uint32_t in3)
{
    uint32_t mask = 0x03;
    uint32_t res = in0;
    switch (sel & mask) {
        //case 0x00: res = in0; break;
        case 0x01: res = in1; break;
        case 0x02: res = in2; break;
        case 0x03: res = in3; break;
    }
    if (sel & ~mask)
        LOGW(W_MSG << " for mux4; sel = " << sel << "; in0 = " << in0 << "; in1 = " << in1 <<
            "; in2 = " << in2 << "; in3 = " << in3 << 
            "; selected input: " << (sel & mask) << "; out: " << res << "; ");
    return res;
}

uint32_t cl::mux8(uint32_t sel, uint32_t in0, uint32_t in1, uint32_t in2, uint32_t in3,
    uint32_t in4, uint32_t in5, uint32_t in6, uint32_t in7)
{
    uint32_t mask = 0x07;
    uint32_t res = in0;
    switch (sel & mask) {
        //case 0x00: res = in0; break;
        case 0x01: res = in1; break;
        case 0x02: res = in2; break;
        case 0x03: res = in3; break;
        case 0x04: res = in4; break;
        case 0x05: res = in5; break;
        case 0x06: res = in6; break;
        case 0x07: res = in7; break;
    }
    if (sel & ~mask)
        LOGW(W_MSG << " for mux8; sel = " << sel << 
            "; in0 = " << in0 << "; in1 = " << in1 << "; in2 = " << in2 << "; in3 = " << in3 << "; in4 = " << in4 << "; in5 = " << in5 <<
            "; in6 = " << in6 << "; in7 = " << in7 <<
            "; selected input: " << (sel & mask) << "; out: " << res << "; ");
    return res;
}
