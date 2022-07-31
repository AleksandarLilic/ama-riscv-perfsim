#include "../include/memory.h"

imem::imem()
{
    burn_mem();
}

uint32_t imem::read(uint32_t address)
{
    // TODO: bounds check? Or just wrap around like in RTL?
    return memory[address & MEM_M];
}
#if ASM_IMEM
std::string imem::read_asm(uint32_t address)
{
    return memory_asm[address & MEM_M];
}
#endif

void imem::burn_mem()
{
    LOG("\nIMEM burn");
    std::ifstream hex_file;
#if RISCV_SANITY_TESTS
    hex_file.open(ASM_RISCV_SANITY_TEST, std::ios::in);
#elif RISCV_ISA_REGR
    std::string test_hex = ASM_RISCV_ISA_TESTS + global_test_name + ".hex";
    hex_file.open(test_hex, std::ios::in);
    LOG("Path to search for the test: " << test_hex);
#else
    LOGE("No test defined. Nothing to be loaded to the memory")
#endif
    if (hex_file.is_open())
        LOG("HEX opened");
    else
        LOGE("HEX file cannot be found");

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
#if RISCV_SANITY_TESTS
    hex_file.open(ASM_RISCV_SANITY_TEST, std::ios::in);
#elif RISCV_ISA_REGR
    std::string test_hex = ASM_RISCV_ISA_TESTS + global_test_name + ".hex";
    hex_file.open(test_hex, std::ios::in);
    LOG("Path to search for the test: " << test_hex);
#else
    LOGE("No test defined. Nothing to be loaded to the memory")
#endif
    if (hex_file.is_open())
        LOG("HEX opened");
    else
        LOGE("HEX file cannot be found");

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
    uint32_t enm = 0; // enable mask
    if (en) {
        LOG("    DMEM address: " << FHEX(addr));
        if (we) {
            // Byte Enable Write
            enm = mask[0][we & 0x1] | mask[1][(we & 0x2) >> 1] |
                mask[2][(we & 0x4) >> 2] | mask[3][(we & 0x8) >> 3];
            uint32_t in = din & enm;
            memory[addr & MEM_M] = (memory[addr & MEM_M] & ~enm) | in;
            LOG("    DMEM stored: " << FHEX(in) << " at address: " << FHEX(addr));
        }
        last_read = memory[addr & MEM_M];
    }
    LOG("    DMEM access return: " << last_read << ", " << FHEX(last_read));
    return last_read;
}
