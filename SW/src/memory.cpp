#include "../include/memory.h"

imem::imem()
{
    burn_mem();
}

uint32_t imem::read(uint32_t address)
{
    // TODO: bounds check? Or just wrap around like in RTL?
    return memory[address];
}
#if ASM_IMEM
std::string imem::read_asm(uint32_t address)
{
    return memory_asm[address];
}
#endif

void imem::burn_mem()
{
    LOG("\nIMEM burn");
    std::ifstream hex_file;
    hex_file.open(ASM_TEST_PATH, std::ios::in);
    if (hex_file.is_open()) {
        LOG("HEX opened");
    }

    uint32_t i = 0;
    while (hex_file) {
        hex_file >> std::hex >> memory[i];
        LOG("    IMEM[" << i << "]: " << FHEX(memory[i]));
#if RISCV_SANITY_TESTS
        global_burned_instructions.push_back(memory[i]);
#endif
        i++;
    }
    LOG("IMEM burn done \n");

#if RISCV_SANITY_TESTS
    global_inst_count = i;
#endif;
}

dmem::dmem()
{
    memory[0] = 0xF0;
    memory[1] = 0xF1;
    memory[2] = 0xF2;
    memory[3] = 0xF3F3'F3F3;
    memory[4] = 0xF4;
    memory[5] = 0xF5;
    memory[6] = 0xF6;
    memory[7] = 0xF7;
    memory[8] = 0xF8;
    memory[9] = 0xF9;
    memory[10] = 0xF10;
    memory[11] = 0xF11;
    memory[12] = 0xF12;
    memory[13] = 0xF13;
    memory[14] = 0xF14;
    memory[15] = 0xF15;
    memory[16] = 0xF16;
    memory[17] = 0xF17;
    memory[18] = 0xF18;
    memory[19] = 0xF19;
    memory[20] = 0xF20;
    memory[21] = 0xF21;
    memory[22] = 0xF22;
    memory[23] = 0xF23;
    memory[24] = 0xF24;
    memory[25] = 0xF25;
    memory[26] = 0xF26;
    memory[27] = 0xF27;
    memory[28] = 0xF28;
    memory[29] = 0xF29;
}

uint32_t dmem::access(uint32_t en, uint32_t we, uint32_t addr, uint32_t din) {
    static uint32_t last_read;
    if (en) {
        // Byte Enable Write
        uint32_t in = din & mask[0][we & 0x1] |
            din & mask[1][(we & 0x2) >> 1] |
            din & mask[2][(we & 0x4) >> 2] |
            din & mask[3][(we & 0x8) >> 3];
        last_read = memory[addr];
    }
    LOG("    DMEM access return: " << last_read << ", " << FHEX(last_read));
    return last_read;
}
