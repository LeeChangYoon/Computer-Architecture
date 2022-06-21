#include "cpu.h"


/* initialize the instruction memory by loading the program on it */
void IM_Init() {
	FILE* fp_in;
	uint32_t bin;
	uint32_t counter = 0x00000;

	fp_in = fopen(PROGRAM, "rb");
	if (fp_in == NULL) {
		printf("Error: program not found.");
		exit(0);
	}

	/* store the instructions into the memory in big - edian format */
	// printf("********************mips********************\n");
	// LINE;
	memset(&InstMemory, 0, sizeof(InstMemory));
	while (fread(&bin, 1, sizeof(int), fp_in) == 4) {
		// printf("%x:\t", counter);
		for (int i = 3; i >= 0; i--) {
			InstMemory[counter + i] = bin & 0xff;
			// printf("%02x", InstMemory[counter + i]);
			bin = bin >> 8;
		}
		// printf("\n");
		counter += 4;
	}
	// LINE;
	// printf("\n");
	fclose(fp_in);
}


/* fetch the instruction from the instruction memory */
uint32_t IM_ReadMemory(uint32_t ReadAddress) {
	return
		(InstMemory[ReadAddress + 3] << 24) |
		(InstMemory[ReadAddress + 2] << 16) |
		(InstMemory[ReadAddress + 1] << 8) |
		(InstMemory[ReadAddress + 0] << 0);
}