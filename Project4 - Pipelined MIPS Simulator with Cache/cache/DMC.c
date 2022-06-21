#include "cpu.h"

/* initialize the direct mapped cache */
void DMC_Init(DMCLine* Cache) {
	memset(Cache, -1, sizeof(DMCLine) * MAXLINE * MAXWAY);

	for (int i = 0; i < MAXLINE * MAXWAY; i++) {
		Cache[i].valid = false;
		Cache[i].dirty = false;
	}
}

/* instruction  direct mapped cache operation */
uint32_t DMC_L1_Inst_Operation(uint32_t Address) {
	uint32_t res = 0;
	uint32_t data = DMC_Read(DMC_L1_Inst, Address);

	for (int i = 3; i >= 0; i--) {
		res |= (data & 0xff) << (8 * i);
		data >>= 8;
	}
	return res;
}

/* direct mapped data cache operation */
void DMC_L1_Data_Operation(uint32_t Address, int size, uint32_t WriteData, bool MemRead, bool MemWrite) {
	if ((MemRead == true) && (MemWrite == false))
		MEMWB[0].ReadData = DMC_Read(DMC_L1_Data, Address);

	if ((MemRead == false) && (MemWrite == true))
		DMC_Write(DMC_L1_Data, Address, WriteData);
}

/* read data from the direct mapped data cache */
uint32_t DMC_Read(DMCLine* Cache, uint32_t Address) {
	uint32_t data = 0;
	bool isUpdated = false;

	uint32_t tag = (Address >> 10);
	uint8_t offset = Address & 0x3f;
	uint8_t index = (Address >> 6) & 0xf;

	if (Cache[index].tag == tag) {
		cache_hit++;
		data = Cache[index].data[offset >> 2];
	}

	else {
		cache_miss++;
		uint32_t address = (Cache[index].tag << 10) + (index << 6);

		Cache[index].tag = tag;
		Cache[index].valid = true;

		if (WRITEBACK) {
			if (Cache[index].dirty) Cache_to_Memory(Cache[index].data, address);
			Cache[index].dirty = false;
		}

		Memory_to_Cache(Cache[index].data, Address);
		data = Cache[index].data[offset >> 2];
	}
	return data;
}

/* write the data into the direct mapped data cache */
void DMC_Write(DMCLine* Cache, uint32_t Address, uint32_t value) {
	uint32_t data = 0;
	bool isUpdated = false;

	uint32_t tag = (Address >> 10);
	uint8_t offset = Address & 0x3f;
	uint8_t index = (Address >> 6) & 0xf;

	if (Cache[index].tag == tag) {
		cache_hit++;
		Cache[index].dirty = true;
		Cache[index].data[offset >> 2] = value;

		if (WRITEBACK) {
			Cache[index].dirty = true;
		}
		else {
			Memory_Access(Address, 32, value, 0, 1);
		}
	}

	else {
		cache_miss++;
		uint32_t address = (Cache[index].tag << 10) + (index << 6);

		Cache[index].tag = tag;
		Cache[index].valid = true;

		if (WRITEBACK) {
			if (Cache[index].dirty) Cache_to_Memory(Cache[index].data, address);
			Memory_to_Cache(Cache[index].data, Address);
			Cache[index].data[offset >> 2] = value;
			Cache[index].dirty = true;
		}
		else {
			Memory_Access(Address, 32, value, 0, 1);
			Memory_to_Cache(Cache[index].data, Address);
		}
	}
}
