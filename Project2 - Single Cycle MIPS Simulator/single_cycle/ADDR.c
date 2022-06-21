#include "mips.h"


// sign extend
uint32_t SignExtend(uint16_t immediate) {
    if (!(immediate & 0x8000)) return 0x0000ffff & immediate;
    else return 0xffff0000 | immediate;
}

// zero extend
uint32_t ZeroExtend(uint16_t immediate) {
    return 0x0000ffff & immediate;
}

// branch addr
uint32_t BranchAddr(uint32_t PC, uint32_t immediate) {
    return PC + 4 + (immediate << 2);
}

// jump addr
uint32_t JumpAddr(uint32_t PC, uint32_t address) {
    return ((PC + 4) & 0xf0000000) | (address << 2);
}

// PC addr
uint32_t PCAddr(uint32_t PC) {
    if ((inst->opcode == 0x00) && (inst->funct == JALR)) return PC + 4;
    return PC + MUX(4, 8, JumpLink);
}
