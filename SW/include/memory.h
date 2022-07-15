#pragma once

#include "defines.h"

#include <array>

class imem
{
private:
    std::array<uint32_t, IMEM_SIZE> memory{};
    std::array<std::string, IMEM_SIZE> memory_asm{};

public:
    imem();
    uint32_t read(uint32_t address);
#if ASM_IMEM
    std::string read_asm(uint32_t address);
#endif
private:
    void burn_mem();
};

class dmem
{
private:
    std::array<uint32_t, IMEM_SIZE> memory{};
    //uint32_t dmem_dout;
    const uint32_t mask[4][2] = {
            {0x0, 0x0000'00FF},
            {0x0, 0x0000'FF00},
            {0x0, 0x00FF'0000},
            {0x0, 0xFF00'0000} };
public:
    dmem();
    uint32_t access(uint32_t en, uint32_t we, uint32_t addr, uint32_t din);
private:
    uint32_t read();
    void write();
};