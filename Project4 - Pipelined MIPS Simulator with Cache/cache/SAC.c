#include "cpu.h"

/* initialize the set associative cache */
void SAC_Init(SACLine Cache[][MAXWAY]) {
	memset(Cache, -1, sizeof(SACLine) * MAXWAY * MAXLINE);

	for (int i = 0; i < MAXLINE; i++)
		for (int j = 0; j < MAXWAY; j++) {
			Cache[i][j].sca = 0;
			Cache[i][j].esca = 0b00;
			Cache[i][j].valid = false;
			Cache[i][j].dirty = false;
			SAC_Deque[i][j] = (MAXWAY - j - 1);
		}
}

/* instruction set associative cache operation */
uint32_t SAC_L1_Inst_Operation(uint32_t Address) {
	uint32_t res = 0;
	uint32_t data = SAC_Read(SAC_L1_Inst, Address);

	for (int i = 3; i >= 0; i--) {
		res |= (data & 0xff) << (8 * i);
		data >>= 8;
	}
	return res;
}

/* set associative data cache operation */
void SAC_L1_Data_Operation(uint32_t Address, int size, uint32_t WriteData, bool MemRead, bool MemWrite) {
	if ((MemRead == true) && (MemWrite == false))
		MEMWB[0].ReadData = SAC_Read(SAC_L1_Data, Address);

	if ((MemRead == false) && (MemWrite == true))
		SAC_Write(SAC_L1_Data, Address, WriteData);
}

/* read data from the set associative data cache */
uint32_t SAC_Read(SACLine Cache[][MAXWAY], uint32_t Address) {
	int EmptyIdx = 0;
	uint32_t data = 0;
	bool isEmpty = false;
	bool isUpdated = false;

	uint32_t tag = (Address >> 8);
	uint8_t offset = Address & 0x3f;
	uint8_t index = (Address >> 6) & 0x3;

	for (int way = 0; way < MAXWAY; way++) {
		if (!Cache[index][way].valid && !isEmpty) {
			isEmpty = true;
			EmptyIdx = way;
			continue;
		}

		if (Cache[index][way].tag == tag) {
			cache_hit += 1;
			isUpdated = true;
			Cache[index][way].frequency += 1;
			Cache[index][way].timestamp = cycle;
			data = Cache[index][way].data[offset >> 2];
			break;
		}
	}

	if (!isUpdated && isEmpty) {
		cache_miss += 1;
		isUpdated = true;
		Cache[index][EmptyIdx].tag = tag;
		Cache[index][EmptyIdx].valid = true;
		Cache[index][EmptyIdx].frequency = 0;
		Cache[index][EmptyIdx].timestamp = cycle;
		Memory_to_Cache(Cache[index][EmptyIdx].data, Address);
		data = Cache[index][EmptyIdx].data[offset >> 2];
	}

	if (!isUpdated && !isEmpty) {
		int way = 0;
		cache_miss += 1;
		switch (REPLACEMENT)
		{
		case '0': way = SAC_FIFO(index); break;
		case '1': way = SAC_Random(); break;
		case '2': way = SAC_LRU(Cache, index); break;
		case '3': way = SAC_LFU(Cache, index); break;
		case '4': way = SAC_SCA(Cache, index); break;
		case '5': way = SAC_ESCA(Cache, index); break;
		default:
			printf("Error: wrong replacement policy selection.\n");
			exit(0);
		}
		uint32_t address = (Cache[index][way].tag << 8) + (index << 6);

		Cache[index][way].tag = tag;
		Cache[index][way].valid = true;
		Cache[index][way].frequency = 0;
		Cache[index][way].timestamp = cycle;

		if (WRITEBACK) {
			if (Cache[index][way].dirty) Cache_to_Memory(Cache[index][way].data, address);
			Cache[index][way].dirty = false;
		}
		Memory_to_Cache(Cache[index][way].data, Address);
		data = Cache[index][way].data[offset >> 2];
	}
	return data;
}

/* write the data into the set associative data cache */
void SAC_Write(SACLine Cache[][MAXWAY], uint32_t Address, uint32_t value) {
	int EmptyIdx = 0;
	bool isEmpty = false;
	bool isUpdated = false;

	uint32_t tag = (Address >> 8);
	uint8_t offset = Address & 0x3f;
	uint8_t index = (Address >> 6) & 0x3;

	for (int way = 0; way < MAXWAY; way++) {
		if (!Cache[index][way].valid && !isEmpty) {
			EmptyIdx = way;
			isEmpty = true;
			continue;
		}

		if (Cache[index][way].tag == tag) {
			cache_hit += 1;
			isUpdated = true;
			Cache[index][way].frequency += 1;
			Cache[index][way].timestamp = cycle;
			Cache[index][way].data[offset >> 2] = value;

			if (WRITEBACK) {
				Cache[index][way].dirty = true;
			}
			else {
				Memory_Access(Address, 32, value, 0, 1);
			}
			break;
		}
	}

	if (!isUpdated && isEmpty) {
		cache_miss += 1;
		isUpdated = true;
		Cache[index][EmptyIdx].tag = tag;
		Cache[index][EmptyIdx].valid = true;
		Cache[index][EmptyIdx].frequency = 0;
		Cache[index][EmptyIdx].timestamp = cycle;

		if (WRITEBACK) {
			Memory_to_Cache(Cache[index][EmptyIdx].data, Address);
			Cache[index][EmptyIdx].data[offset >> 2] = value;
			Cache[index][EmptyIdx].dirty = true;
		}
		else {
			Memory_Access(Address, 32, value, 0, 1);
			Memory_to_Cache(Cache[index][EmptyIdx].data, Address);
		}
	}

	if (!isUpdated && !isEmpty) {
		int way = 0;
		cache_miss += 1;
		switch (REPLACEMENT)
		{
		case '0': way = SAC_FIFO(index); break;
		case '1': way = SAC_Random(); break;
		case '2': way = SAC_LRU(Cache, index); break;
		case '3': way = SAC_LFU(Cache, index); break;
		case '4': way = SAC_SCA(Cache, index); break;
		case '5': way = SAC_ESCA(Cache, index); break;
		default:
			printf("Error: wrong replacement policy selection.\n");
			exit(0);
		}
		uint32_t address = (Cache[index][way].tag << 8) + (index << 6);

		Cache[index][way].tag = tag;
		Cache[index][way].valid = true;
		Cache[index][way].frequency = 0;
		Cache[index][way].timestamp = cycle;

		if (WRITEBACK) {
			if (Cache[index][way].dirty) Cache_to_Memory(Cache[index][way].data, address);
			Memory_to_Cache(Cache[index][way].data, Address);
			Cache[index][way].data[offset >> 2] = value;
			Cache[index][way].dirty = true;
		}
		else {
			Memory_Access(Address, 32, value, 0, 1);
			Memory_to_Cache(Cache[index][way].data, Address);
		}
	}
}

/* random algorithm */
int SAC_Random() {
	return rand() % MAXWAY;
}

/* least recently used algorithm */
int SAC_LRU(SACLine Cache[][MAXWAY], uint16_t index) {	
	int current;
	int LRUIdx = index;
	int timestamp = Cache[index][0].timestamp;

	for (int way = 0; way < MAXWAY; way++) {
		cache_total++;
		current = Cache[index][way].timestamp;
		if (timestamp > current) {
			LRUIdx = way;
			timestamp = current;
		}
	}
	return LRUIdx;
}

/* least frequently used algorithm */
int SAC_LFU(SACLine Cache[][MAXWAY], uint16_t index) {
	int current;
	int LFUIdx = index;
	int frequency = Cache[index][0].frequency;

	for (int way = 0; way < MAXWAY; way++) {
		cache_total++;
		current = Cache[index][way].frequency;
		if (frequency > current) {
			LFUIdx = way;
			frequency = current;
		}
	}
	return LFUIdx;
}

/* first in first out algorithm */
int SAC_FIFO(uint16_t index) {
	cache_total++;
	int FIFOIdx = SAC_Deque[index][MAXWAY - 1];
	Deque_Cycle(SAC_Deque[index], MAXWAY);
	return FIFOIdx;
}

/* second chance algorithm */
int SAC_SCA(SACLine Cache[][MAXWAY], uint16_t index) {
	int SCAIdx = SAC_Deque[index][MAXWAY - 1];
	while (Cache[index][SCAIdx].sca) {
		cache_total++;
		Cache[index][SCAIdx].sca = 0;
		Deque_Cycle(SAC_Deque[index], MAXWAY);
		SCAIdx = SAC_Deque[index][MAXWAY - 1];
	}
	cache_total++;
	Cache[index][SCAIdx].sca = 1;
	Deque_Cycle(SAC_Deque[index], MAXWAY);
	return SCAIdx;
}

/* enhanced second chance algorithm */
int SAC_ESCA(SACLine Cache[][MAXWAY], uint16_t index) {
	int ESCAIdx = SAC_Deque[index][MAXWAY - 1];
	while (Cache[index][ESCAIdx].esca) {
		cache_total++;
		Cache[index][ESCAIdx].esca -= 1;
		Deque_Cycle(SAC_Deque[index], MAXWAY);
		ESCAIdx = SAC_Deque[index][MAXWAY - 1];
	}
	cache_total++;
	Cache[index][ESCAIdx].esca = 0b11;
	Deque_Cycle(SAC_Deque[index], MAXWAY);
	return ESCAIdx;
}
