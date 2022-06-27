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
    LOG("    Instruction in IF stage: " << 
        FHEX(imem_dout) << "; ASM: " << imem.read_asm(core.if_intf.imem_addr));
#else
    LOG("    Instruction in IF stage: " <<
        FHEX(imem_dout));
#endif
}
