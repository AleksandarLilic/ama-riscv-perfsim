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
#if LOG_DBG
        LOG("    IMEM[" << i << "]: " << FHEX(memory[i]));
#endif
#if RISCV_SANITY_TESTS
        if ((memory[i]) == 0 && (global_inst_count == 0))
        global_inst_count = i;
#endif;
        i++;
    }
    LOG("IMEM burn done \n");
}

dmem::dmem()
{
    burn_mem();
}

void dmem::burn_mem()
{
    LOG("\nDMEM burn");
    std::ifstream hex_file;
    hex_file.open(ASM_TEST_PATH, std::ios::in);
    if (hex_file.is_open()) {
        LOG("HEX opened");
    }

    uint32_t i = 0;
    while (hex_file) {
        hex_file >> std::hex >> memory[i];
#if LOG_DBG
        LOG("    DMEM[" << i << "]: " << FHEX(memory[i]));
#endif
        i++;
    }
    LOG("DMEM burn done \n");
}

uint32_t dmem::access(uint32_t en, uint32_t we, uint32_t addr, uint32_t din) {
#if LOG_DBG
    LOG("    DMEM access parameters: ");
    LOG("        en: " << en << "; we: " << FHEX(we) << "; address: " << FHEX(addr) << 
        "; din: " << FHEX(din) << ", (" << din << ")" );
#endif
    static uint32_t last_read;
    if (en) {
        if (we) {
            // Byte Enable Write
            uint32_t in = din & mask[0][we & 0x1] |
                din & mask[1][(we & 0x2) >> 1] |
                din & mask[2][(we & 0x4) >> 2] |
                din & mask[3][(we & 0x8) >> 3];
            memory[addr] = in;
            LOG("    DMEM stored: " << FHEX(in) << " at address: " << FHEX(addr));
        }
        last_read = memory[addr];
    }
    LOG("    DMEM access return: " << last_read << ", " << FHEX(last_read));
    return last_read;
}
