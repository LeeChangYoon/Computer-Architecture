#include "cpu.h"

/* initialize 2-bit last time predictor */
void LTP2_Init() {
	memset(&BHT, -1, sizeof(BHT));
	for (int i = 0; i < BHTENTRY; i++) BHT[i].prediction_bit = 0b01;
}

/* 2-bit last time branch predictor prediction */
void LTP2_Prediction() {
	/* set index for branch history table by using the last two bits from the PC value */
	int bht_idx = BHTENTRY - 1;
	bht_idx <<= 2;
	bht_idx &= PC;
	bht_idx >>= 2;
	IFID[0].BHTIdx = bht_idx;

	/* check 2-bit for branch prediction
		0b00: strongly not taken
		0b01: weakly not taken
		0b10: weakly taken
		0b11: strongly taken */
	if (PC == BHT[bht_idx].branch_inst_addr) {
		IFID[0].BHTFound = true;
		switch (BHT[bht_idx].prediction_bit)
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

/* 2-bit last time branch predictor execution*/
void LTP2_Execution() {
	if (IDEX[1].opcode == BEQ || IDEX[1].opcode == BNE) branch_taken++;
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
				if (BHT[IDEX[1].BHTIdx].prediction_bit == 0b10) // weakly taken -> strongly taken
					BHT[IDEX[1].BHTIdx].prediction_bit = 0b11;
			}
			else {
				/* miss */
				branch_miss++;
				// printf("Branch Prediction Miss\n");
				if (BHT[IDEX[1].BHTIdx].prediction_bit >= 0b10)
					BHT[IDEX[1].BHTIdx].prediction_bit -= 1;
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
				if (BHT[IDEX[1].BHTIdx].prediction_bit <= 0b01) // strongly not taken -> weakly not taken or weakly not taken- > weakly taken
					BHT[IDEX[1].BHTIdx].prediction_bit += 1;
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
				if (BHT[IDEX[1].BHTIdx].prediction_bit == 0b01) // weakly not taken -> strongly not taken
					BHT[IDEX[1].BHTIdx].prediction_bit = 0b00;
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
			BHT[IDEX[1].BHTIdx].prediction_bit = 0b11; // update prediction bit into strongly taken

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