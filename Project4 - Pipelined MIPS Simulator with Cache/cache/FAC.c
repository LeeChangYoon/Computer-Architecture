#include "cpu.h"

/* initialize the fully associative cache */
void FAC_Init(FACLine* Cache) {
	memset(Cache, -1, sizeof(FACLine) * MAXLINE * MAXWAY);

	for (int i = 0; i < MAXLINE * MAXWAY; i++) {
		Cache[i].sca = 0;
		Cache[i].esca = 0b00;
		Cache[i].valid = false;
		Cache[i].dirty = false;
		FAC_Deque[i] = (MAXLINE * MAXWAY - i - 1);
	}
}

/* instruction fully associative cache operation */
uint32_t FAC_L1_Inst_Operation(uint32_t Address) {
	uint32_t res = 0;
	uint32_t data = FAC_Read(FAC_L1_Inst, Address);

	for (int i = 3; i >= 0; i--) {
		res |= (data & 0xff) << (8 * i);
		data >>= 8;
	}
	return res;
}

/* fully associative data cache operation */
void FAC_L1_Data_Operation(uint32_t Address, int size, uint32_t WriteData, bool MemRead, bool MemWrite) {
	if ((MemRead == true) && (MemWrite == false))
		MEMWB[0].ReadData = FAC_Read(FAC_L1_Data, Address);

	if ((MemRead == false) && (MemWrite == true))
		FAC_Write(FAC_L1_Data, Address, WriteData);
}

/* read data from the fully associative data cache */
uint32_t FAC_Read(FACLine* Cache, uint32_t Address) {
	uint32_t data = 0;
	bool isUpdated = false;

	uint32_t tag = Address >> 6;
	uint8_t offset = Address & 0x3f;

	for (int i = 0; i < MAXLINE * MAXWAY; i++) {
		if (!Cache[i].valid) {
			cache_miss += 1;
			isUpdated = true;
			Cache[i].tag = tag;
			Cache[i].valid = true;
			Cache[i].frequency = 0;
			Cache[i].timestamp = true;
			Memory_to_Cache(Cache[i].data, Address);
			data = Cache[i].data[offset >> 2];
			break;
		}

		if (Cache[i].tag == tag) {
			cache_hit += 1;
			isUpdated = true;
			Cache[i].frequency += 1;
			Cache[i].timestamp = cycle;
			data = Cache[i].data[offset >> 2];
			break;
		}
	}

	if (!isUpdated) {
		uint32_t Index = 0;
		cache_miss += 1;
		switch (REPLACEMENT)
		{
		case '0': Index = FAC_FIFO();  break;
		case '1': Index = FAC_Random(); break;
		case '2': Index = FAC_LRU(Cache); break;
		case '3': Index = FAC_LFU(Cache); break;
		case '4': Index = FAC_SCA(Cache); break;
		case '5': Index = FAC_ESCA(Cache); break;
		default:
			printf("Error: wrong replacement policy selection.\n");
			exit(0);
		}
		uint32_t address = Cache[Index].tag << 6;

		Cache[Index].tag = tag;
		Cache[Index].valid = true;
		Cache[Index].frequency = 0;
		Cache[Index].timestamp = cycle;

		if (WRITEBACK) {
			if (Cache[Index].dirty) Cache_to_Memory(Cache[Index].data, address);
			Cache[Index].dirty = false;
		}
		Memory_to_Cache(Cache[Index].data, Address);
		data = Cache[Index].data[offset >> 2];
	}
	return data;
}

/* write the data into the fully  associative data cache */
void FAC_Write(FACLine* Cache, uint32_t Address, uint32_t value) {
	uint32_t data = 0;
	bool isUpdated = false;

	uint32_t tag = Address >> 6;
	uint8_t offset = Address & 0x3f;

	for (int i = 0; i < MAXLINE * MAXWAY; i++) {
		if (!Cache[i].valid) {
			cache_miss += 1;
			isUpdated = true;
			Cache[i].tag = tag;
			Cache[i].valid = true;
			Cache[i].frequency = 0;
			Cache[i].timestamp = cycle;

			if (WRITEBACK) {
				Memory_to_Cache(Cache[i].data, Address);
				Cache[i].data[offset >> 2] = value;
				Cache[i].dirty = true;
			}
			else {
				Memory_Access(Address, 32, value, 0, 1);
				Memory_to_Cache(Cache[i].data, Address);
			}

			break;
		}

		if (Cache[i].tag == tag) {
			cache_hit += 1;
			isUpdated = true;
			Cache[i].frequency += 1;
			Cache[i].timestamp = cycle;
			Cache[i].data[offset >> 2] = value;

			if (WRITEBACK) {
				Cache[i].dirty = true;
			}
			else {
				Memory_Access(Address, 32, value, 0, 1);
			}
			break;
		}
	}

	if (!isUpdated) {
		uint32_t Index = 0;
		cache_miss += 1;
		switch (REPLACEMENT)
		{
		case '0': Index = FAC_FIFO();  break;
		case '1': Index = FAC_Random(); break;
		case '2': Index = FAC_LRU(Cache); break;
		case '3': Index = FAC_LFU(Cache); break;
		case '4': Index = FAC_SCA(Cache); break;
		case '5': Index = FAC_ESCA(Cache); break;
		default:
			printf("Error: wrong replacement policy selection.\n");
			exit(0);
		}
		uint32_t address = Cache[Index].tag << 6;

		Cache[Index].tag = tag;
		Cache[Index].valid = true;
		Cache[Index].frequency = 0;
		Cache[Index].timestamp = cycle;

		if (WRITEBACK) {
			if (Cache[Index].dirty) Cache_to_Memory(Cache[Index].data, address);
			Memory_to_Cache(Cache[Index].data, Address);
			Cache[Index].data[offset >> 2] = value;
			Cache[Index].dirty = true;
		}
		else {
			Memory_Access(Address, 32, value, 0, 1);
			Memory_to_Cache(Cache[Index].data, Address);
		}
	}
}

/* random algorithm*/
uint32_t FAC_Random() {
	return rand() % (MAXLINE * MAXWAY);
}

/* least recently used algorithm */
uint32_t FAC_LRU(FACLine* Cache) {
	uint32_t current;
	uint32_t LRUIdx = 0;
	int timestamp = Cache[0].timestamp;

	for (current = 0; current < MAXLINE * MAXWAY; current++) {
		cache_total++;
		if (timestamp > Cache[current].timestamp) {
			LRUIdx = current;
			timestamp = Cache[current].timestamp;
		}
	}
	return LRUIdx;
}

/* least frequently used algorithm */
uint32_t FAC_LFU(FACLine* Cache) {
	uint32_t current;
	uint32_t LFUIdx = 0;
	int frequency = Cache[0].frequency;

	for (current = 0; current < MAXLINE * MAXWAY; current++) {
		cache_total++;
		if (frequency > Cache[current].frequency) {
			LFUIdx = current;
			frequency = Cache[current].frequency;
		}
	}
	return LFUIdx;
}

/* first in first out algorithm */
uint32_t FAC_FIFO() {
	cache_total++;
	uint32_t FIFOIdx = FAC_Deque[MAXLINE * MAXWAY - 1];
	Deque_Cycle(FAC_Deque, MAXLINE * MAXWAY);
	return FIFOIdx;
}

/* second chance algorithm */
uint32_t FAC_SCA(FACLine* Cache) {
	uint32_t SCAIdx = FAC_Deque[MAXLINE * MAXWAY - 1];
	while (Cache[SCAIdx].sca) {
		cache_total++;
		Cache[SCAIdx].sca = 0;
		Deque_Cycle(FAC_Deque, MAXLINE * MAXWAY);
		SCAIdx = FAC_Deque[MAXLINE * MAXWAY - 1];
	}
	cache_total++;
	Cache[SCAIdx].sca = 1;
	Deque_Cycle(FAC_Deque, MAXLINE * MAXWAY);
	return SCAIdx;
}

/* enhanced second chance algorithm */
uint32_t FAC_ESCA(FACLine* Cache) {
	uint32_t ESCAIdx = FAC_Deque[MAXLINE * MAXWAY - 1];
	while (Cache[ESCAIdx].esca) {
		cache_total++;
		Cache[ESCAIdx].esca -= 1;
		Deque_Cycle(FAC_Deque, MAXLINE * MAXWAY);
		ESCAIdx = FAC_Deque[MAXLINE * MAXWAY - 1];
	}
	cache_total++;
	Cache[ESCAIdx].esca = 0b11;
	Deque_Cycle(FAC_Deque, MAXLINE * MAXWAY);
	return ESCAIdx;
}
