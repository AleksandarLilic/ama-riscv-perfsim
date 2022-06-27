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
    memory[0] = 0x00c58533;   memory_asm[0] = "add   x10,x11,x12";
    memory[1] = 0x40c58533;   memory_asm[1] = "sub   x10,x11,x12";
    memory[2] = 0x00c59533;   memory_asm[2] = "sll   x10,x11,x12";
    memory[3] = 0x00c5d533;   memory_asm[3] = "srl   x10,x11,x12";
    memory[4] = 0x40c5d533;   memory_asm[4] = "sra   x10,x11,x12";
    memory[5] = 0x00c5a533;   memory_asm[5] = "slt   x10,x11,x12";
    memory[6] = 0x00c5b533;   memory_asm[6] = "sltu  x10,x11,x12";
    memory[7] = 0x00c5c533;   memory_asm[7] = "xor   x10,x11,x12";
    memory[8] = 0x00c5e533;   memory_asm[8] = "or    x10,x11,x12";
    memory[9] = 0x00c5f533;   memory_asm[9] = "and   x10,x11,x12";
    memory[10] = 0x02558513;  memory_asm[10] = "addi  x10,x11,37";
    memory[11] = 0x0255a513;  memory_asm[11] = "slti  x10,x11,37";
    memory[12] = 0x0255b513;  memory_asm[12] = "sltiu x10,x11,37";
    memory[13] = 0x0255c513;  memory_asm[13] = "xori  x10,x11,37";
    memory[14] = 0x0255e513;  memory_asm[14] = "ori   x10,x11,37";
    memory[15] = 0x0255f513;  memory_asm[15] = "andi  x10,x11,37";
    memory[16] = 0x00359513;  memory_asm[16] = "slli  x10,x11,0x3";
    memory[17] = 0x0035d513;  memory_asm[17] = "srli  x10,x11,0x3";
    memory[18] = 0x4035d513;  memory_asm[18] = "srai  x10,x11,0x3";
    memory[19] = 0x00458503;  memory_asm[19] = "lb    x10,4(x11)";
    memory[20] = 0x00459503;  memory_asm[20] = "lh    x10,4(x11)";
    memory[21] = 0x0045a503;  memory_asm[21] = "lw    x10,4(x11)";
    memory[22] = 0x0045c503;  memory_asm[22] = "lbu   x10,4(x11)";
    memory[23] = 0x0045d503;  memory_asm[23] = "lhu   x10,4(x11)";
    memory[24] = 0x00a58223;  memory_asm[24] = "sb    x10,4(x11)";
    memory[25] = 0x00a59223;  memory_asm[25] = "sh    x10,4(x11)";
    memory[26] = 0x00a5a223;  memory_asm[26] = "sw    x10,4(x11)";
    memory[27] = 0xfab50ee3;  memory_asm[27] = "beq   x10,x11,4000005c <loop>";
    memory[28] = 0xfab51ce3;  memory_asm[28] = "bne   x10,x11,4000005c <loop>";
    memory[29] = 0xfab54ae3;  memory_asm[29] = "blt   x10,x11,4000005c <loop>";
    memory[30] = 0xfab558e3;  memory_asm[30] = "bge   x10,x11,4000005c <loop>";
    memory[31] = 0xfab566e3;  memory_asm[31] = "bltu  x10,x11,4000005c <loop>";
    memory[32] = 0xfab574e3;  memory_asm[32] = "bgeu  x10,x11,4000005c <loop>";
    memory[33] = 0x00458567;  memory_asm[33] = "jalr    x10,4(x11)";
    memory[34] = 0xfa1ff06f;  memory_asm[34] = "jal     x0,4000005c <loop>";
    memory[35] = 0x01000537;  memory_asm[35] = "lui     x10,0x1000";
    memory[36] = 0x01000517;  memory_asm[36] = "auipc   x10,0x1000";
    memory[37] = 0x01000500;  memory_asm[37] = "invalid instr";
    memory[38] = 0x00628433;  memory_asm[38] = "add     x8,x5,x6";
    memory[39] = 0x00f40593;  memory_asm[39] = "addi    x11,x8,15";
    memory[40] = 0x06340613;  memory_asm[40] = "addi    x12,x8,99";
    memory[41] = 0x06340613;  memory_asm[41] = "addi    x12,x8,99";
    memory[42] = 0x06340613;  memory_asm[42] = "addi    x12,x8,99";
    memory[43] = 0x06340613;  memory_asm[43] = "addi    x12,x8,99";
    memory[44] = 0x00000013;  memory_asm[44] = "NOP";
    memory[45] = 0x00000013;  memory_asm[45] = "NOP";
    memory[46] = 0x00000013;  memory_asm[46] = "NOP";
    memory[47] = 0x00000013;  memory_asm[47] = "NOP";
    memory[48] = 0x00000013;  memory_asm[48] = "NOP";
}

dmem::dmem()
{
    LOGW("dmem created -> placeholder");
}