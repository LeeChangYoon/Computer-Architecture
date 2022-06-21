#include "cpu.h"


/* main */
void main(int argc, char* argv[]) {
	PROGRAM = argv[1];
	PREDICTOR = argv[2][0];

	/* initialization */
	init();
	
	/* cycle start */
	do {
		// LINE;
		cycle++;

		/* execute the stages */
		MEM();
		WB(); // write back first so that write operation happens before read operation
		IF();
		ID();
		EX();

		/* fix the data hazards and the delay slot due to memory read operaiton*/
		HU_Operation();
		FW_Operation();

		/* print out the logs */
		// LOG_Print();

		/* update the latches */
		LAT_Update();
		// LINE;
		if (PC == 0x18ecc) printf("Cycle: %d\n", Register[v1]);
	} while (PC < 0xffffffff);

	/* termination */
	terminate();
}


/* IF (instruction fetch) stage*/
void IF() {
	if (PC != 0xffffffff) {
		total_instruction++;
		IFID[0].valid = true; // change the valid bit into 1
		IFID[0].IR = IM_ReadMemory(PC); // fetch the instruction
		switch (PREDICTOR)
		{
		case '1': { PC = PCAddr(PC); IFID[0].NPC = PC;  break; }
		case '2': ALT_Prediction();		break;
		case '3': LTP1_Prediction();		break;
		case '4': LTP2_Prediction();		break;
		case '5': GSH_Prediction();		break;
		case '6': LVL2_Prediction();		break;
		default: {
			printf("Error: wrong branch predictor selection.\n");
			exit(0);
		}
		}
	}	
}


/* ID (instruction decode) stage */
void ID() {
	if (IFID[1].valid) {
		/* instruction decode */
		inst->opcode = (IFID[1].IR >> 26) & 0x3f;
		inst->rs = (IFID[1].IR >> 21) & 0x1f;
		inst->rt = (IFID[1].IR >> 16) & 0x1f;
		inst->rd = (IFID[1].IR >> 11) & 0x1f;
		inst->shamt = (IFID[1].IR >> 6) & 0x1f;
		inst->funct = (IFID[1].IR >> 0) & 0x3f;
		inst->immediate = (IFID[1].IR >> 0) & 0xffff;
		inst->address = (IFID[1].IR >> 0) & 0x3ffffff;

		/* execute stage */
		RF_Read(inst->rs, inst->rt);
		CU_Operation(inst->opcode, inst->funct);

		IDEX[0].valid = true; // change the valid bit into 1
		if (!IDEXCtrlSrc) IDEX_EXMEM_CPY(); 
		else memset(&IDEX[0].Control, 0, sizeof(IDEX_SIG)); // if data hazard is detected, flush the control signals
		IDEX_LAT_WIRE();

		if (PCWrite && Jump) jump_instruction++;
		if (PCWrite) PC = MUX2(PC, JumpAddr(IFID[1].NPC, inst->address), Jump);
	}
}


/* EX (execution) stage */
void EX() {
	if (IDEX[1].valid) {
		/* execute stage */
		uint32_t ALUControl = ALU_Control(IDEX[1].Control.ALUOp, MUX2(IDEX[1].opcode, IDEX[1].funct, IDEX[1].Control.Rtype));
		EXMEM[0].ReadData1 = MUX3(IDEX[1].ReadData1, MUX2(MEMWB[1].ALUResult, MEMWB[1].ReadData, MEMWB[1].Control.MemToReg), EXMEM[1].ALUResult, FwdA);
		EXMEM[0].ReadData2 = MUX3(IDEX[1].ReadData2, MUX2(MEMWB[1].ALUResult, MEMWB[1].ReadData, MEMWB[1].Control.MemToReg), EXMEM[1].ALUResult, FwdB);
		EXMEM[0].ALUResult = ALU_Operation(ALUControl, MUX2(EXMEM[0].ReadData1, EXMEM[0].ReadData2, IDEX[1].Control.Shift), MUX2(MUX2(EXMEM[0].ReadData2, IDEX[1].imm, IDEX[1].Control.ALUSrc), IDEX[1].shamt, IDEX[1].Control.Shift));
	
		EXMEM[0].valid = true; // change the valid bit into 1
		EXMEM_MEMWB_CPY();
		EXMEM_LAT_WIRE();

		switch (PREDICTOR)
		{
		case '1': ANT_Execution();		break;
		case '2':	ALT_Execution();		break;
		case '3': LTP1_Execution();		break;
		case '4': LTP2_Execution();		break;
		case '5': GSH_Execution();		break;
		case '6': LVL2_Execution();		break;
		default: {
			printf("Error: wrong branch predictor selection.\n");
			exit(0);
		}
		}
		if (PCWrite && IDEX[1].Control.JumpReg) jump_instruction++;
		if (PCWrite) PC = MUX2(PC, EXMEM[0].ReadData1, IDEX[1].Control.JumpReg);
	}
}


/* MEM (memory access) stage */
void MEM() {
	if (EXMEM[1].valid) {
		DM_MemoryAccess(EXMEM[1].ALUResult, 32, EXMEM[1].ReadData2, EXMEM[1].Control.MemRead, EXMEM[1].Control.MemWrite);
		MEMWB[0].valid = true; // change the valid bit into 1
		MEMWB_LAT_WIRE();
	}
}


/* WB (write back) stage */
void WB() {
	if (MEMWB[1].valid)
		RF_Write(MEMWB[1].rd, MUX2(MUX2(MEMWB[1].ALUResult, MEMWB[1].ReadData, MEMWB[1].Control.MemToReg), MEMWB[1].NPC, MEMWB[1].Control.JumpLink), MEMWB[1].Control.RegWrite);
}


/* initialize all of the hardware components */
void init() {
	RF_Init();
	IM_Init();
	CU_Init();
	DM_Init();
	FW_Init();
	HU_Init();
	LAT_Init();
	LOG_Init();
	switch (PREDICTOR)
	{
	case '1':							break;
	case '2': ALT_Init();		break;
	case '3': LTP1_Init();		break;
	case '4': LTP2_Init();		break;
	case '5': GSH_Init();		break;
	case '6': LVL2_Init();		break;
	default: {
		printf("Error: wrong branch predictor selection.\n");
		exit(0);
	}
	}
	PC = 0x00000000;
	inst = (instruction*)malloc(sizeof(instruction));
}


/* termination */
void terminate() {
	printf("\n******************result*********************\n");
	printf("Final return value: %d\n", Register[2]);
	printf("Number of executed instruction: %d\n", total_instruction);
	printf("Number of executed memory access instruction: %d\n", memory_access);
	printf("Number of executed register operation instruction: %d\n", register_operation);
	printf("Number of executed taken branch instruction: %d\n", branch_taken);
	printf("Number of predicted branch: %d\n", branch_hit);
	printf("Number of not predicted branch: %d\n", branch_miss);
	printf("Number of jump instruction: %d\n", jump_instruction);
	printf("*********************************************\n");

	free(inst);
}
