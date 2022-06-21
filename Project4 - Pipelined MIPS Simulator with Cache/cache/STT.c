#include "cpu.h"

/* static branch predictor */
/* always not taken branch predictor */
/* always not taken branch predictor execution */
void ANT_Execution() {
	bool BrTaken = MUX2(EXMEM[0].bcond && EXMEM[0].Control.Branch, !EXMEM[0].bcond && EXMEM[0].Control.Branch, EXMEM[0].Control.Equal);
	if (IDEX[1].opcode == BEQ || IDEX[1].opcode == BNE) {
		branch_taken++;
		branch_hit = branch_taken - branch_miss;
	}

	if (BrTaken) {
		/* miss */
		branch_miss++;
		// printf("Branch Prediction Miss\n");
		if (PCWrite) PC = MUX2(PC, BranchAddr(IDEX[1].NPC, IDEX[1].imm), BrTaken);

		/* change the valid bit of IFID and IDEX latches */
		IFID[0].valid = false;
		IFID[1].valid = false;
		IDEX[0].valid = false;
	}
	else {
		/* hit */
		// printf("Branch Prediction Hit\n");
	}
}

/* always taken branch predictor */
/* initialize always taken branch predictor */
void ALT_Init() {
	memset(&BHT, -1, sizeof(BHT));
}

/* always taken predictor branch prediction */
void ALT_Prediction() {
	/* set index for branch history table by using the last two bits from the PC value */
	int bht_idx = BHTENTRY - 1;
	bht_idx <<= 2;
	bht_idx &= PC;
	bht_idx >>= 2;
	IFID[0].BHTIdx = bht_idx;

	if (PC == BHT[bht_idx].branch_inst_addr) {
		IFID[0].BHTFound = true;
		IFID[0].NPC = PCAddr(PC);
		if (PCWrite) PC = BHT[bht_idx].branch_target_addr;
	}
	else {
		IFID[0].BHTFound = false;
		if (PCWrite) PC = PCAddr(PC);
		IFID[0].NPC = PC;
	}
}

/* always taken branch predictor execution */
void ALT_Execution() {
	if (IDEX[1].opcode == BEQ || IDEX[1].opcode == BNE) branch_taken++;
	bool BrTaken = MUX2(EXMEM[0].bcond && EXMEM[0].Control.Branch, !EXMEM[0].bcond && EXMEM[0].Control.Branch, EXMEM[0].Control.Equal);

	if (IDEX[1].Control.BHTFound) {
		if (BrTaken) {
			/* hit */
			branch_hit++;
			// printf("Branch Prediction Hit\n");
		}
		else {
			/* miss */
			branch_miss++;
			// printf("Branch Prediction Miss\n");
			if (PCWrite) PC = IDEX[1].NPC;

			/* change the valid bit of IFID and IDEX latches */
			IFID[0].valid = false;
			IFID[1].valid = false;
			IDEX[0].valid = false;
		}
	}
	else {
		if (BrTaken) {
			/* miss */
			branch_hit++;
			// printf("Branch Prediction MIss\n");
			BHT[IDEX[1].BHTIdx].branch_inst_addr = IDEX[1].NPC - 4; // update instruction address
			if (PCWrite) PC = MUX2(PC, BranchAddr(IDEX[1].NPC, IDEX[1].imm), BrTaken);
			BHT[IDEX[1].BHTIdx].branch_target_addr = PC; // update target address

			/* change the valid bit of IFID and IDEX latches */
			IFID[0].valid = false;
			IFID[1].valid = false;
			IDEX[0].valid = false;
		}
		else {
			/* hit */
		}
	}
}