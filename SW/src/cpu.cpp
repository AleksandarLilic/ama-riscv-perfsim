#include "../include/cpu.h"

cpu::cpu(seq_queue *q) :
    core(q, &imem_dout, &dmem_dout)/*,
    imem(IMEM_SIZE, NOT_RESET)*/
{
    this->q = q;
}

void cpu::update()
{
    core.status_log();
    core.reset(reset_status);
    core.update_system();
    core.update_wb();
    core.update_mem();
    // dmem_dout = dmem.access(ex_intf, mem_intf);
    core.update_ex();
    core.update_id();
    core.update_if();
    imem_dout = imem.read(core.if_intf.imem_addr);

#if ASM_IMEM
    static std::string imemc_s[5]{};
    imemc_s[4] = imemc_s[3];    // wb
    imemc_s[3] = imemc_s[2];    // mem
    imemc_s[2] = imemc_s[1];    // ex
    imemc_s[1] = imemc_s[0];    // id
    imemc_s[0] = imem.read_asm(core.if_intf.imem_addr); // if
    LOG("    Instruction in IF stage: " << FHEX(imem_dout));
    
    LOG("\nInstruction pipeline:");
    LOG("    Instruction in IF stage:  " << imemc_s[0]);
    LOG("    Instruction in ID stage:  " << imemc_s[1]);
    LOG("    Instruction in EX stage:  " << imemc_s[2]);
    LOG("    Instruction in MEM stage: " << imemc_s[3]);
    LOG("    Instruction in WB stage:  " << imemc_s[4]);

#else
    LOG("    Instruction in IF stage: " <<
        FHEX(imem_dout));
#endif
}
