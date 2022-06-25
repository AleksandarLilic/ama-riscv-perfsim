#include "../include/cpu.h"

cpu::cpu(seq_queue *q) :
    core(q, &imem_dout, &dmem_dout)
{
    this->q = q;

    // temp. move to mem.h
    imem[0] = 0x00c58533;  imemc[0] = "add   x10,x11,x12";
    imem[1] = 0x40c58533;  imemc[1] = "sub   x10,x11,x12";
    imem[2] = 0x00c59533;  imemc[2] = "sll   x10,x11,x12";
    imem[3] = 0x00c5d533;  imemc[3] = "srl   x10,x11,x12";
    imem[4] = 0x40c5d533;  imemc[4] = "sra   x10,x11,x12";
    imem[5] = 0x00c5a533;  imemc[5] = "slt   x10,x11,x12";
    imem[6] = 0x00c5b533;  imemc[6] = "sltu  x10,x11,x12";
    imem[7] = 0x00c5c533;  imemc[7] = "xor   x10,x11,x12";
    imem[8] = 0x00c5e533;  imemc[8] = "or    x10,x11,x12";
    imem[9] = 0x00c5f533;  imemc[9] = "and   x10,x11,x12";
    imem[10] = 0x02558513;  imemc[10] = "addi  x10,x11,37";
    imem[11] = 0x0255a513;  imemc[11] = "slti  x10,x11,37";
    imem[12] = 0x0255b513;  imemc[12] = "sltiu x10,x11,37";
    imem[13] = 0x0255c513;  imemc[13] = "xori  x10,x11,37";
    imem[14] = 0x0255e513;  imemc[14] = "ori   x10,x11,37";
    imem[15] = 0x0255f513;  imemc[15] = "andi  x10,x11,37";
    imem[16] = 0x00359513;  imemc[16] = "slli  x10,x11,0x3";
    imem[17] = 0x0035d513;  imemc[17] = "srli  x10,x11,0x3";
    imem[18] = 0x4035d513;  imemc[18] = "srai  x10,x11,0x3";
    imem[19] = 0x00458503;  imemc[19] = "lb    x10,4(x11)";
    imem[20] = 0x00459503;  imemc[20] = "lh    x10,4(x11)";
    imem[21] = 0x0045a503;  imemc[21] = "lw    x10,4(x11)";
    imem[22] = 0x0045c503;  imemc[22] = "lbu   x10,4(x11)";
    imem[23] = 0x0045d503;  imemc[23] = "lhu   x10,4(x11)";
    imem[24] = 0x00a58223;  imemc[24] = "sb    x10,4(x11)";
    imem[25] = 0x00a59223;  imemc[25] = "sh    x10,4(x11)";
    imem[26] = 0x00a5a223;  imemc[26] = "sw    x10,4(x11)";
    imem[27] = 0xfab50ee3;  imemc[27] = "beq   x10,x11,4000005c <loop>";
    imem[28] = 0xfab51ce3;  imemc[28] = "bne   x10,x11,4000005c <loop>";
    imem[29] = 0xfab54ae3;  imemc[29] = "blt   x10,x11,4000005c <loop>";
    imem[30] = 0xfab558e3;  imemc[30] = "bge   x10,x11,4000005c <loop>";
    imem[31] = 0xfab566e3;  imemc[31] = "bltu  x10,x11,4000005c <loop>";
    imem[32] = 0xfab574e3;  imemc[32] = "bgeu  x10,x11,4000005c <loop>";
    imem[33] = 0x00458567;  imemc[33] = "jalr    x10,4(x11)";
    imem[34] = 0xfa1ff06f;  imemc[34] = "jal     x0,4000005c <loop>";
    imem[35] = 0x01000537;  imemc[35] = "lui     x10,0x1000";
    imem[36] = 0x01000517;  imemc[36] = "auipc   x10,0x1000";
    imem[37] = 0x01000500;  imemc[37] = "invalid instr";
    imem[38] = 0x00628433;  imemc[38] = "add     x8,x5,x6";
    imem[39] = 0x00f40593;  imemc[39] = "addi    x11,x8,15";
    imem[40] = 0x06340613;  imemc[40] = "addi    x12,x8,99";
    imem[41] = 0x06340613;  imemc[41] = "addi    x12,x8,99";
    imem[42] = 0x06340613;  imemc[42] = "addi    x12,x8,99";
    imem[43] = 0x06340613;  imemc[43] = "addi    x12,x8,99";
    imem[44] = 0x00000013;  imemc[44] = "NOP";
    imem[45] = 0x00000013;  imemc[45] = "NOP";
    imem[46] = 0x00000013;  imemc[46] = "NOP";
    imem[47] = 0x00000013;  imemc[47] = "NOP";
    imem[48] = 0x00000013;  imemc[48] = "NOP";
}

void cpu::update()
{
    core.status_log();
    core.reset(reset_status);
    core.update_system();
    core.update_wb();
    core.update_mem();
    //  dmem_dout = dmem[dmem_addr];
    core.update_ex();
    core.update_id();
    core.update_if();
    imem_dout = imem[core.if_intf.imem_addr];
    LOG("---------- inst in IF stage: " << 
        FHEX(imem_dout) << "; ASM: " << imemc[core.if_intf.imem_addr]);
}
