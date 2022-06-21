#include "mips.h"


int main(int argc, char* argv[]) {
    // initialize
    PROGRAM = argv[1];
    init_all();
    printf("****************single cycle****************\n");

    while (PC < 0xffffffff) {
	// instruction fetch (IF)
	inst_count++;
	IR = IM_ReadMemory(PC);
	printf("----------------------------------------------------------------------\n");
	printf("Cycle: %d\n", inst_count);
	printf("[Fetch]   %x: %08x\n", PC, IR);

	// instruction decode and register operand fetch (ID / RF)
	Inst_Decode();
	Print_Decode();
	RF_Read(inst->rs, inst->rt);
	CU_Operation(inst->opcode, inst->funct);

	// execute / evaluate memory address (EX / AG)
	inst->immediate = MUX(ZeroExtend(inst->immediate), SignExtend(inst->immediate), SignEx);
	ALUControl = ALU_Control(ALUOp, MUX(inst->opcode, inst->funct, Rtype));
	ALUResult = ALU_Operation(ALUControl, MUX(ReadData1, ReadData2, Shift), MUX(MUX(ReadData2, inst->immediate, ALUSrc), inst->shamt, Shift));
	
	// memory operand fetch (MEM)
	DM_MemoryAccess(ALUResult, 32, ReadData2, MemRead, MemWrite);

	// store / writeback result (WB)
	RF_Write(MUX(MUX(inst->rt, inst->rd, RegDst), ra, RA), MUX(MUX(ALUResult, ReadData, MemtoReg), PCAddr(PC), JumpLink), RegWrite);

	// PC update
	Branch = MUX(ALUResult && Branch, !ALUResult && Branch, Equal);
	PC = MUX(MUX(MUX(PCAddr(PC), BranchAddr(PC, inst->immediate), Branch), JumpAddr(PC, inst->address), Jump), ReadData1, JumpReg);
	
	// show user visible architecture state
	Print_Execute();
    }
    terminate();
    return 0;
}


// init CPU
void init_all() {
    RF_Init();
    IM_Init();
    CU_Init();
    DM_Init();
    inst_count = 0;
    IR = 0x00000000;
    PC = 0x00000000;
    inst = (instruction*)malloc(sizeof(instruction));
}


// terminate CPU
void terminate() {
    printf("\n******************result*********************\n");
    printf("(1) Final return value: %d\n", Register[2]);
    printf("(2) Number of executed instruction: %d\n", inst_count);
    printf("(3) Number of executed R type instruction: %d\n", R_count);
    printf("(4) Number of executed I type instruction: %d\n", I_count);
    printf("(5) Number of executed J type instruction: %d\n", J_count);
    printf("(6) Number of executed memory access instruction: %d\n", M_count);
    printf("(7) Number of executed taken branches instruction: %d\n", B_count);
    printf("*********************************************\n");

    free(inst);
}


// instruction decode
void Inst_Decode() {
    inst->opcode    = (IR >> 26) & 0x3f;
    inst->rs	    = (IR >> 21) & 0x1f;
    inst->rt	    = (IR >> 16) & 0x1f;
    inst->rd	    = (IR >> 11) & 0x1f;
    inst->shamt	    = (IR >>  6) & 0x1f;
    inst->funct	    = (IR >>  0) & 0x3f;
    inst->immediate = (IR >>  0) & 0xffff;
    inst->address   = (IR >>  0) & 0x3ffffff;
}


// multiplexer (MUX)
uint32_t MUX(uint32_t IN1, uint32_t IN2, bool S) {
    return S ? IN2 : IN1;
}


// print decode 
void Print_Decode() {
    enum inst_type type;
    switch (inst->opcode)
    {
    case 0x00:	type = R_type; break;
    case 0x02:
    case 0x03:	type = J_type; break;
    default:	type = I_type;  break;
    }

    switch (type)
    {
    case J_type:
	J_count++;
	printf("[Decode]  opcode(%02x) address: %x\n", inst->opcode, inst->address);
	break;
    case I_type:
	I_count++;
	printf("[Decode]  opcode(%02x) rs: %x rt: %x immediate: %x\n", inst->opcode, inst->rs, inst->rt, inst->immediate);
	break;
    case R_type:
	if(IR != 0x0) R_count++;
	printf("[Decode]  opcode(%02x) rs: %x rt: %x rd: %x shamt: %x funct(%x)\n", inst->opcode, inst->rs, inst->rd, inst->rt, inst->shamt, inst->funct);
	break;
    }

}


// print execute
void Print_Execute() {
    switch (inst->opcode)
    {
    // I type
    case ADDI: 
    case ADDIU: printf("[Execute] R[%d]: %08x = R[%d] + %08x\n", inst->rt, Register[inst->rt], inst->rs, inst->immediate); break;
    case ANDI:	printf("[Execute] R[%d]: %08x = R[%d] & %08x\n", inst->rt, Register[inst->rt], inst->rs, inst->immediate); break;
    case BEQ: 
	B_count++;
	if (Branch == true) printf("[Execute] Jump to  %08x = R[%d] == R[%d]\n", PC, inst->rs, inst->rt);
	else printf("[Execute] Branch equal X = R[%d] != R[%d]\n", inst->rs, inst->rt); 
	break;
    case BNE:
	B_count++;
	if (Branch == true) printf("[Execute] Branch not equal X = R[%d] == R[%d]\n", inst->rs, inst->rt);
	else printf("[Execute] Jump to  %08x = R[%d] != R[%d]\n", PC, inst->rs, inst->rt);
	break;
    case LUI:	printf("[Execute] R[%d]: %08x = R[%d] << 16\n", inst->rt, Register[inst->rt], inst->immediate); break;
    case LW:	M_count++;  printf("[Execute] R[%d]: %08x = M[R[%d] + %08x]\n", inst->rt, Register[inst->rt], inst->rs, inst->immediate); break;
    case ORI:	printf("[Execute] R[%d]: %08x = R[%d] | %08x\n", inst->rt, Register[inst->rt], inst->rs, inst->immediate); break;
    case SLTI:
    case SLTIU:	printf("[Execute] R[%d]: %d = (R[%d] < %08x)? 1 : 0\n", inst->rt, Register[inst->rt], inst->rs, inst->immediate); break;
    case SW:	M_count++; printf("[Execute] M[R[%d] + %08x]: %08x = R[%d]\n", inst->rs, inst->immediate, Register[inst->rt], inst->rt); break;

    // J type
    case J:	printf("[Execute] Jump to %08x\n", PC); break;
    case JAL:	printf("[Execute] R[31]: %08x; PC: %08x\n", Register[31], PC); break;

    // R type
    case 0x00:
	switch (inst->funct)
	{
	case ADD: 
	case ADDU:  printf("[Execute] R[%d]: %08x = R[%d] + R[%d]\n", inst->rd, Register[inst->rd], inst->rs, inst->rt); break;
	case AND:   printf("[Execute] R[%d]: %08x = R[%d] & R[%d]\n", inst->rd, Register[inst->rd], inst->rs, inst->rt); break;
	case JR:    printf("[Execute] PC: %08x = R[%d]\n", PC, inst->rs); break;
	case JALR:  printf("[Execute] R[31]: %08x; PC: %08x\n", Register[inst->rd], PC); break;
	case NOR:   printf("[Execute] R[%d]: %08x = ~(R[%d] | R[%d])\n", inst->rd, Register[inst->rd], inst->rs, inst->rt); break;
	case OR:    printf("[Execute] R[%d]: %08x = R[%d] | R[%d]\n", inst->rd, Register[inst->rd], inst->rs, inst->rt); break;
	case SLT:
	case SLTU:  printf("[Execute] R[%d]: %d = (R[%d] + R[%d]) ? 1 : 0\n", inst->rd, Register[inst->rd], inst->rs, inst->rt); break;
	case SLL:   printf("[Execute] R[%d]: %08x = R[%d] << R[%d]\n", inst->rd, Register[inst->rd], inst->rs, inst->rt); break;
	case SRL:   printf("[Execute] R[%d]: %08x = R[%d] >> R[%d]\n", inst->rd, Register[inst->rd], inst->rs, inst->rt); break;
	case SUB:	
	case SUBU:  printf("[Execute] R[%d]: %08x = R[%d] - R[%d]\n", inst->rd, Register[inst->rd], inst->rs, inst->rt); break;
	}
    }
}
