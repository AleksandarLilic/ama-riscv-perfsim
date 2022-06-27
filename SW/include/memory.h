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
    uint32_t dmem_dout;

public:
    dmem();
    void access();
private:
    uint32_t read();
    void write();
};