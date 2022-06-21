#include "cpu.h"

/* Initialize the 2-level predictor */
void LVL2_Init() {
	memset(&LHT, 0, sizeof(LHT));
	memset(&BHT, -1, sizeof(BHT));
	memset(&PHT, 0b01, sizeof(PHT));
}

/* 2-level predictor  branch prediction */
void LVL2_Prediction() {
	int bht_idx = BHTENTRY - 1;
	bht_idx <<= 2;
	bht_idx &= PC;
	bht_idx >>= 2;
	IFID[0].BHTIdx = bht_idx;

	int lht_idx = LHTENTRY - 1;
	lht_idx <<= 2;
	lht_idx &= PC;
	lht_idx >>= 2;
	IFID[0].LHTIdx = lht_idx;

	int pht_idx = PHTENTRY - 1;
	pht_idx <<= 2;
	pht_idx &= LHT[lht_idx];
	pht_idx >>= 2;
	IFID[0].PHTIdx = pht_idx;

	/* check 2-bit for branch prediction
		0b00: strongly not taken
		0b01: weakly not taken
		0b10: weakly taken
		0b11: strongly taken */
	if (PC == BHT[bht_idx].branch_inst_addr) {
		IFID[0].BHTFound = true;
		switch (PHT[pht_idx])
		{
		case 0b00:
		case 0b01:
			IFID[0].PreTaken = false;
			if (PCWrite) PC = PCAddr(PC);
			IFID[0].NPC = PC;
			break;

		case 0b10:
		case 0b11:
			IFID[0].PreTaken = true;
			IFID[0].NPC = PCAddr(PC);
			if (PCWrite) PC = BHT[bht_idx].branch_target_addr;
			break;
		}
	}
	else {
		IFID[0].BHTFound = false;
		IFID[0].PreTaken = false;
		if (PCWrite) PC = PCAddr(PC);
		IFID[0].NPC = PC;
	}
}

/* 2-level branch predictor execution */
void LVL2_Execution() {
	bool BrTaken = MUX2(EXMEM[0].bcond && EXMEM[0].Control.Branch, !EXMEM[0].bcond && EXMEM[0].Control.Branch, EXMEM[0].Control.Equal);

	/* change 2-bit for branch prediction
		0b00: strongly not taken
		0b01: weakly not taken
		0b10: weakly taken
		0b11: strongly taken */
	if (IDEX[1].Control.BHTFound) { // found in BHT during IF stage
		if (IDEX[1].Control.PreTaken) { // predicted to be taken
			if (BrTaken) {
				/* hit */
				branch_hit++;
				// printf("Branch Prediction Hit\n");
				if (PHT[IDEX[1].PHTIdx] == 0b10) // weakly taken -> strongly taken
					PHT[IDEX[1].PHTIdx] = 0b11;
			}
			else {
				/* miss */
				branch_miss++;
				// printf("Branch Prediction Miss\n");
				if (PHT[IDEX[1].PHTIdx] >= 0b10)
					PHT[IDEX[1].PHTIdx] -= 1;
				if (PCWrite) PC = IDEX[1].NPC;

				/* change the valid bit of IFID and IDEX latches */
				IFID[0].valid = false;
				IFID[1].valid = false;
				IDEX[0].valid = false;
			}
		}
		else { // predicted not to be taken
			if (BrTaken) {
				/* miss */
				branch_miss++;
				// printf("Branch Prediction Miss\n");
				if (PHT[IDEX[1].PHTIdx] <= 0b01) // strongly not taken -> weakly not taken or weakly not taken- > weakly taken
					PHT[IDEX[1].PHTIdx] += 1;
				if (PCWrite) PC = BHT[IDEX[1].BHTIdx].branch_target_addr;

				/* change the valid bit of IFID and IDEX latches */
				IFID[0].valid = false;
				IFID[1].valid = false;
				IDEX[0].valid = false;
			}
			else {
				/* hit */
				branch_hit++;
				// printf("Branch Prediction Hit\n");
				if (PHT[IDEX[1].PHTIdx] == 0b01) // weakly not taken -> strongly not taken
					PHT[IDEX[1].PHTIdx] = 0b00;
			}
		}
	}
	else { // not found in BHT during IF stage
		if (BrTaken) { // BHT should be updated in this condition
			/* miss */
			branch_miss++;
			// printf("Branch Prediction Miss\n");
			BHT[IDEX[1].BHTIdx].branch_inst_addr = IDEX[1].NPC - 4; // update instruction address
			if (PCWrite) PC = MUX2(PC, BranchAddr(IDEX[1].NPC, IDEX[1].imm), BrTaken);
			BHT[IDEX[1].BHTIdx].branch_target_addr = PC; // update target address
			PHT[IDEX[1].PHTIdx] = 0b11; // update prediction bit into strongly taken

			/* change the valid bit of IFID and IDEX latches */
			IFID[0].valid = false;
			IFID[1].valid = false;
			IDEX[0].valid = false;
		}
		else {
			/* hit */
		}
	}

	/* local history register update */
	if (IDEX[1].opcode == BEQ || IDEX[1].opcode == BNE) {
		branch_taken++;
		LHT[IDEX[1].LHTIdx] <<= 1;
		LHT[IDEX[1].LHTIdx] |= BrTaken;
	}
}