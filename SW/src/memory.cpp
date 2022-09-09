#include "../include/memory.h"

imem::imem()
{
    //burn_mem();
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
#if HEX_READ
    LOG("Burning with HEX file");
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
#else // !HEX_READ
    LOG("Burning with BIN file");
    std::ifstream bin_file;
    std::string test_bin = ASM_RISCV_ISA_TESTS + global_test_name + ".bin";

    bin_file.open(test_bin, std::ios::in | std::ios::binary);
    LOG("Path to search for the test: " << test_bin);
    
    bin_file.seekg(0, std::ios::end);
    uint32_t size = bin_file.tellg();
    bin_file.seekg(0, std::ios::beg);

    if (bin_file.is_open())
        LOG("BIN opened");
    else
        LOGE("BIN file cannot be found");

    const uint32_t stride = 4;
    char bin_line[stride];    
    uint32_t mem_addr = 0;

    while (mem_addr < (size/stride)) {
        bin_file.read(bin_line, stride);
        uint32_t one_word = 0;

        // flip endianess
        for (uint32_t j = 0; j < stride; j++)
            one_word |= (bin_line[j] & 0xFF) << (j * 8);
        
        memory[mem_addr] = one_word;
#if LOG_DBG
        LOG("    IMEM[" << mem_addr << "]: " << FHEX(memory[mem_addr]));
#endif // LOG_DBG
#if RISCV_SANITY_TESTS
        if ((memory[mem_addr]) == 0 && (global_inst_count == 0))
            global_inst_count = i;
#endif // RISCV_SANITY_TESTS
        mem_addr++;
    }
#endif // !HEX_READ
    LOG("IMEM burn done \n");

}

dmem::dmem()
{
    //burn_mem();
}

void dmem::burn_mem()
{
    LOG("\nDMEM burn");
#if HEX_READ
    LOG("Burning with HEX file");
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
#if RISCV_SANITY_TESTS
        if ((memory[i]) == 0 && (global_inst_count == 0))
            global_inst_count = i;
#endif;
        i++;
    }
#else // !HEX_READ
    LOG("Burning with BIN file");
    std::ifstream bin_file;
    std::string test_bin = ASM_RISCV_ISA_TESTS + global_test_name + ".bin";

    bin_file.open(test_bin, std::ios::in | std::ios::binary);
    LOG("Path to search for the test: " << test_bin);

    bin_file.seekg(0, std::ios::end);
    uint32_t size = bin_file.tellg();
    bin_file.seekg(0, std::ios::beg);

    if (bin_file.is_open())
        LOG("BIN opened");
    else
        LOGE("BIN file cannot be found");

    const uint32_t stride = 4;
    char bin_line[stride];
    uint32_t mem_addr = 0;

    while (mem_addr < (size / stride)) {
        bin_file.read(bin_line, stride);
        uint32_t one_word = 0;

        // flip endianess
        for (uint32_t j = 0; j < stride; j++)
            one_word |= (bin_line[j] & 0xFF) << (j * 8);

        memory[mem_addr] = one_word;
#if LOG_DBG
        LOG("    DMEM[" << mem_addr << "]: " << FHEX(memory[mem_addr]));
#endif // LOG_DBG
#if RISCV_SANITY_TESTS
        if ((memory[mem_addr]) == 0 && (global_inst_count == 0))
            global_inst_count = i;
#endif // RISCV_SANITY_TESTS
        mem_addr++;
    }
#endif // !HEX_READ
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
