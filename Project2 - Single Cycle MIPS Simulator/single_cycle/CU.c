#include "mips.h"


// initialize variables in control unit
void CU_Init() {
    RegDst	= 0;
    SignEx	= 0;
    ALUSrc	= 0;
    MemtoReg	= 0;
    RegWrite	= 0;
    MemRead	= 0;
    MemWrite	= 0;
    Branch	= 0;
    Jump	= 0;
    JumpReg	= 0;
    JumpLink	= 0;
    Rtype	= 0;
    Equal	= 0;
    RA		= 0;
    ALUOp	= 0b00;
}


// control unit
void CU_Operation(uint8_t opcode,  uint32_t funct) {
    RegDst	= (opcode == 0x00);
    SignEx	= (opcode != ANDI) && (opcode != ORI);
    Shift	= (opcode == 0x00) && ((funct == SLL) || (funct == SRL));
    ALUSrc	= (opcode != 0x00) && (opcode != BEQ) && (opcode != BNE);
    MemtoReg	= (opcode == LW);
    RegWrite	= (opcode != SW) && (opcode != BEQ) && (opcode != BNE) && (opcode != J) && !((opcode == 0x00) && (funct == JR));
    MemRead	= (opcode == LW);
    MemWrite	= (opcode == SW);
    Branch	= (opcode == BEQ) || (opcode == BNE);
    Jump	= (opcode == J) || (opcode == JAL);
    JumpReg	= ((opcode == 0x00) && (funct == JR)) || ((opcode == 0x00) && (funct == JALR));
    JumpLink	= (opcode == JAL) || ((opcode == 0x00) && (funct == JALR));
    Rtype	= (opcode == 0x00);
    Equal	= (opcode == BEQ);
    RA		= (opcode == JAL);

    // load type
    if ((opcode == LW) || (opcode == LL) || (opcode == LHU) || (opcode == LBU))
	ALUOp = 0b00;
    // store type
    else if ((opcode == SW) || (opcode == SH) || (opcode == SC) || (opcode == SB))
	ALUOp = 0b00;
    // branch type
    else if ((opcode == BEQ) || (opcode == BNE))
	ALUOp = 0b01;
    // R type
    else if (opcode == 0x00)
	ALUOp = 0b10;
    // I type
    else
	ALUOp = 0b11;
}
