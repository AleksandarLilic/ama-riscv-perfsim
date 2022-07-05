#include "../include/cpu.h"

cpu::cpu(seq_queue *q) :
    //core(q, &imem_dout, &dmem_dout),
    core(q, &core_intf)/*,
    imem(IMEM_SIZE, NOT_RESET)*/
{
    this->q = q;
//    imem_address = &core.if_intf.imem_addr;
//    dmem_address = &core.ex_intf.alu_out;
}

void cpu::update()
{
    core.status_log();
    core.reset(reset_status);
    core.update_system();
    // WB
    core.update_wb();
    // MEM
    core.update_mem();
    // EX
    core.update_ex();
    core_intf.dmem_dout = dmem.access(*core_intf.dmem_en, *core_intf.dmem_we, 
        *core_intf.dmem_addr, *core_intf.dmem_din);
    // ID
    core.update_id();
    // IF
    core.update_if();
    core_intf.imem_dout = imem.read(*core_intf.imem_addr);

#if ASM_IMEM
    static std::string imemc_s[5]{};
    imemc_s[4] = imemc_s[3];    // wb
    imemc_s[3] = imemc_s[2];    // mem
    imemc_s[2] = imemc_s[1];    // ex
    if (global_inst_to_ctrl == 0x13) // id
        imemc_s[1] = "NOP";
    else
        imemc_s[1] = imemc_s[0];
    imemc_s[0] = imem.read_asm(*core_intf.imem_addr); // if
    LOG("    Instruction in IF stage: " << FHEX(core_intf.imem_dout));
    
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
