#include "../include/inst_field.h"

uint32_t inst_field::funct7(uint32_t inst)
{
    return (inst & FUNCT7_M) >> 25;
}

uint32_t inst_field::opc7(uint32_t inst)
{
    return (inst & OPC7_M);
}
