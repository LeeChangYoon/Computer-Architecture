#include "cpu.h"

/* initialize the instruction memory by loading the program on it */
void Memory_Init() {
	FILE* fp_in;
	uint32_t bin;
	uint32_t counter = 0x00000;

	fp_in = fopen(PROGRAM, "rb");
	if (fp_in == NULL) {
		printf("Error: program not found.");
		exit(0);
	}

	/* store the instructions into the memory in big - endian format */
	// printf("********************mips********************\n");
	// LINE;
	memset(&Memory, 0, sizeof(Memory));
	while (fread(&bin, 1, sizeof(int), fp_in) == 4) {
		// printf("%x:\t", counter);
		for (int i = 3; i >= 0; i--) {
			Memory[counter + i] = bin & 0xff;
			// printf("%02x", Memory[counter + i]);
			bin = bin >> 8;
		}
		// printf("\n");
		counter += 4;
	}
	// LINE;
	// printf("\n");
	fclose(fp_in);
}

/* data memory access */
void Memory_Access(uint32_t Address, int size, uint32_t WriteData, bool MemRead, bool MemWrite) {
	/* memory read operations*/
	if ((MemRead == true) && (MemWrite == false)) {
		memory_access++;
		uint32_t result = 0;
		for (int i = 0; i < (size / 8); i++) {
			result = result << 8;
			result = result | Memory[Address + i];
		}
		MEMWB[0].ReadData = result;
	}

	/* memory write operations */
	if ((MemRead == false) && (MemWrite == true)) {
		memory_access++;
		for (int i = (size / 8) - 1; i >= 0; i--) {
			Memory[Address + i] = WriteData & 0xff;
			WriteData = WriteData >> 8;
		}
	}
}

/* copy the memory from the main memory to the cache */
void Memory_to_Cache(uint32_t* Cache, uint32_t Address) {
	Address >>= 6;
	Address <<= 6;
	uint32_t result = 0;
	for (int i = 0; i < MAXSIZE; i++) {
		for (int j = 0; j < (32 / 8); j++) {
			result <<= 8;
			result |= Memory[Address + (i * 4) + j];
		}
		Cache[i] = result;
	}
}

/* copy the memory from the cache to the main memory */
void Cache_to_Memory(uint32_t* Cache, uint32_t Address) {
	Address >>= 6;
	Address <<= 6;
	memory_access++;
	for (int i = 0; i < MAXSIZE; i++) {
		uint32_t WriteData = Cache[i];
		for (int j = (32 / 8) - 1; j >= 0; j--) {
			Memory[Address + (i * 4) + j] = WriteData & 0xff;
			WriteData = WriteData >> 8;
		}
	}
}