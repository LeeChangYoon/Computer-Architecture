#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "mips_reg.h"
#include "mips_inst.h"

#define LINE    printf("---------------------------------------------------------------\n")

/* size variables */
char CACHE; // cache
char PREDICTOR; // branch predictor
char* PROGRAM; //  executing program
int WRITEBACK; // true if the write policy is write through
char REPLACEMENT; // replacement policy for full associative and set associative cache

#define REGSIZE 32 // size of the registers
#define BHTENTRY 0x100 // size of the branch history table
#define PHTENTRY 0x100 // size of the pattern history table
#define LHTENTRY 0x100 // size of the local history register table
#define MEMSIZE 0x1000000 // size of the memory -> 16MB

/* cache variables */
#define MAXWAY 4 // number of the ways
#define MAXSIZE 16 // size of the data block
#define MAXLINE 16 // number of the cache line

/* cache line structure */
/* fully associative cache */
typedef struct FACLine {
	bool sca;
	bool dirty;
	bool valid;
	uint8_t esca;
	uint32_t tag;
	int frequency;
	int timestamp;
	uint32_t data[MAXSIZE];
} FACLine;
 
/* direct mapping cache */
typedef struct DMCLine {
	bool dirty;
	bool valid;
	uint32_t tag;
	uint32_t data[MAXSIZE];
} DMCLine;

/* set associative cache */
typedef struct SACLine {
	bool sca;
	bool dirty;
	bool valid;
	uint8_t esca;
	uint32_t tag;
	int frequency;
	int  timestamp;
	uint32_t data[MAXSIZE];
} SACLine;

/* instruction type */
enum inst_type { J_type, I_type, R_type };

/* instruction fields */
typedef struct instruction {
	uint8_t opcode;
	uint8_t rs;
	uint8_t rt;
	uint8_t rd;
	uint8_t shamt;
	uint8_t funct;
	uint16_t immediate;
	uint32_t address;
} instruction;

/* control signals for each stages */
/* ID stage signals */
bool Jump; // jump
bool SignEx; // sign extended
uint8_t FwdA; // forward A
uint8_t FwdB; // forward B
bool PCWrite; // PC write
bool IFIDWrite; // IFID latch write
bool IDEXCtrlSrc; // IDEX initialization

/* EX stage signals */
typedef struct IDEX_SIG {
	bool RegDst; // register destination
	bool ReAddr; // return address of 31
	bool RegWrite; // register write
	bool ALUSrc; // ALU source
	bool Shift; // bit shift
	bool Rtype; // R type instruction
	uint8_t ALUOp; // ALU operation
	bool Equal; // branch equal
	bool MemRead; // memory read
	bool MemWrite; // memory write
	bool Branch; // branch taken
	bool MemToReg; // memory to register
	bool JumpReg; // jump register
	bool JumpLink; // jump link
	bool PreTaken; // predict taken
	bool BHTFound; // check that the instruction found in BTB
} IDEX_SIG;

/* MEM stage signals */
typedef struct EXMEM_SIG {
	bool RegWrite; // register write
	bool ALUSrc; // ALU source
	bool Shift; // bit shift
	bool Rtype; // R type instruction
	uint8_t ALUOp; // ALU operation
	bool Equal; // branch equal
	bool MemRead; // memory read
	bool MemWrite; // memory write
	bool Jump; // jump
	bool Branch; // branch taken
	bool MemToReg; // memory to register
	bool JumpReg; // jump register
	bool JumpLink; // jump link
} EXMEM_SIG;

/* WB stage signals */
typedef struct MEMWB_SIG {
	bool RegWrite; // register write
	bool MemToReg; // memory to register
	bool JumpLink; // jump link
} MEMWB_SIG;

/* latches for each stages */
/* IFID latch */
typedef struct IFID_LAT {
	bool valid;  // latch valid bit
	uint32_t NPC; // PC + 4
	uint32_t IR; // instruction register
	uint32_t BHTIdx; // branch history index
	uint32_t PHTIdx; // pattern history index
	uint32_t LHTIdx; // pattern history index
	bool PreTaken; // predict taken
	bool BHTFound; // check that the instruction found in BTB
} IFID_LAT;

/* IDEX latch */
typedef struct IDEX_LAT {
	bool valid; // latch valid bit
	uint32_t NPC; // PC + 4
	uint32_t ReadData1; // read data from R[rs]
	uint32_t ReadData2; // read data from R[rt]
	uint8_t rt; // target register
	uint8_t rd; // destination register
	uint8_t rs; // source register
	uint32_t imm; // immediate
	uint8_t funct; // function field
	uint8_t shamt; // shift amount
	uint8_t opcode; // operation code
	uint32_t BHTIdx; // branch history table index
	uint32_t PHTIdx; // pattern history index
	uint32_t LHTIdx; // pattern history index
	IDEX_SIG Control; // control signals for later stages
} IDEX_LAT;

/* EXMEM latch */
typedef struct EXMEM_LAT {
	bool valid; // latch valid bit
	uint32_t NPC; // PC + 4
	uint32_t BrTarget; // branch target address
	bool bcond; // branch condition bit
	uint32_t ALUResult; // arithmetic logic unit operation result
	uint32_t ReadData1; // read data from R[rs]
	uint32_t ReadData2; // read data from R[rt]
	uint8_t rd; // destination register
	EXMEM_SIG Control; // control signals for later stages
} EXMEM_LAT;

/* MEMWB latch */
typedef struct MEMWB_LAT {
	bool valid; // latch valid bit
	uint32_t NPC; // PC + 4
	uint32_t ReadData; // read data from M[R[rs] + sign_ext_imm]
	uint32_t ALUResult; // arithmetic logic unit operation result
	uint8_t rd; // destination register
	MEMWB_SIG Control; // control signals for write back stage
} MEMWB_LAT;

/* branch history table entries */
typedef struct BHT_ELEMENT {
	uint32_t branch_inst_addr; // branch instruction address (PC) field
	uint32_t branch_target_addr; // branch target address field
	uint8_t prediction_bit; // branch prediction state bit
} BHT_ELEMENT;

/* adder */
uint32_t PCAddr(uint32_t PC);
uint32_t SignExtend(uint16_t immediate);
uint32_t ZeroExtend(uint16_t immediate);
uint32_t JumpAddr(uint32_t PC, uint32_t address);
uint32_t BranchAddr(uint32_t PC, uint32_t immediate);

/* multiplexer */
uint32_t MUX2(uint32_t IN1, uint32_t IN2, bool S);
uint32_t MUX3(uint32_t IN1, uint32_t IN2, uint32_t IN3, uint8_t S);

/* control unit */
void CU_Init();
void CU_Operation(uint8_t opcode, uint32_t funct);

/* register file */
void RF_Init();
void RF_Read(uint8_t RdReg1, uint8_t RdReg2);
void RF_Write(uint8_t WrtReg, uint32_t WrtData, bool WrtEnable);

/* arithmetic logic unit */
uint8_t ALU_Control(uint8_t ALUOp, uint8_t opcode);
uint32_t ALU_Operation(uint8_t ALU_Control, uint32_t operand1, uint32_t operand2);

/* memory */
void Memory_Init();
void Memory_to_Cache(uint32_t* Cache, uint32_t Address);
void Cache_to_Memory(uint32_t* Cache, uint32_t Address);
void Memory_Access(uint32_t Address, int size, uint32_t WriteData, bool MemRead, bool MemWrite);

/* latch */
void LAT_Init();
void LAT_Update();
void IDEX_LAT_WIRE();
void IDEX_EXMEM_CPY();
void EXMEM_LAT_WIRE();
void EXMEM_MEMWB_CPY();
void MEMWB_LAT_WIRE();

/* forward unit */
void FW_Init();
void FW_Operation();

/* hazard detection unit */
void HU_Init();
void HU_Operation();

/* static branch predictor */
void ANT_Execution();
void ALT_Init();
void ALT_Prediction();
void ALT_Execution();

/* 1-bit last time predictor */
void LTP1_Init();
void LTP1_Prediction();
void LTP1_Execution();

/* 2-bit last time predictor */
void LTP2_Init();
void LTP2_Prediction();
void LTP2_Execution();

/* global history table */
void GSH_Init();
void GSH_Prediction();
void GSH_Execution();

/* 2 level predictor */
void LVL2_Init();
void LVL2_Prediction();
void LVL2_Execution();

/* deque for various cache replacement policies */
void Deque_Cycle(int* Deque, int size);

/* fully associative cache operation */
uint32_t FAC_FIFO();
uint32_t FAC_Random();
void FAC_Init(FACLine* Cache);
uint32_t FAC_LFU(FACLine* Cache);
uint32_t FAC_LRU(FACLine* Cache);
uint32_t FAC_SCA(FACLine* Cache);
uint32_t FAC_ESCA(FACLine* Cache);
uint32_t FAC_L1_Inst_Operation(uint32_t Address);
uint32_t FAC_Read(FACLine* Cache, uint32_t Address);
void FAC_Write(FACLine* Cache, uint32_t Address, uint32_t value);
void FAC_L1_Data_Operation(uint32_t Address, int size, uint32_t WriteData, bool MemRead, bool MemWrite);

/* direct mapping cache operation */
void DMC_Init(DMCLine* Cache);
uint32_t DMC_L1_Inst_Operation(uint32_t Address);
uint32_t DMC_Read(DMCLine* Cache, uint32_t Address);
void DMC_Write(DMCLine* Cache, uint32_t Address, uint32_t value);
void DMC_L1_Data_Operation(uint32_t Address, int size, uint32_t WriteData, bool MemRead, bool MemWrite);

/* set associative cache operation */
int SAC_Random();
int SAC_FIFO(uint16_t index);
void SAC_Init(SACLine Cache[][MAXWAY]);
uint32_t SAC_L1_Inst_Operation(uint32_t Address);
int SAC_LFU(SACLine Cache[][MAXWAY], uint16_t index);
int SAC_LRU(SACLine Cache[][MAXWAY], uint16_t index);
int SAC_SCA(SACLine Cache[][MAXWAY], uint16_t index);
int SAC_ESCA(SACLine Cache[][MAXWAY], uint16_t index);
uint32_t SAC_Read(SACLine Cache[][MAXWAY], uint32_t Address);
void SAC_Write(SACLine Cache[][MAXWAY], uint32_t Address, uint32_t value);
void SAC_L1_Data_Operation(uint32_t Address, int size, uint32_t WriteData, bool MemRead, bool MemWrite);

/* log */
void LOG_Init();
void LOG_Print();

/* log variables */
int cycle;
int total_instruction;
int memory_access;
int register_operation;
int branch_taken;
int branch_hit;
int branch_miss;
int jump_instruction;
int cache_hit;
int cache_miss;
int cache_total;

/* control process unit */
/* control process unit operations */
void IF();
void ID();
void EX();
void MEM();
void WB();
void init();
void terminate();

/* control process unit variables */
uint32_t PC; // program counter
uint8_t Memory[MEMSIZE]; // data memory
instruction* inst; // instruction structure
uint32_t Register[REGSIZE]; // register file

IFID_LAT IFID[2]; // IFID IN/OUT latch
IDEX_LAT IDEX[2]; // IDEX IN/OUT latch
EXMEM_LAT EXMEM[2]; // EXMEM IN/OUT latch
MEMWB_LAT MEMWB[2]; // MEMWB IN/OUT latch

/* cache */
/* fully associative cache */
int FAC_Deque[MAXLINE * MAXWAY]; // deque for the fully associative cache
FACLine FAC_L1_Data[MAXLINE * MAXWAY]; // fully associative L1 data cache
FACLine FAC_L1_Inst[MAXLINE * MAXWAY]; // fully associative L1 instruction cache

/* direct mapping cache */
DMCLine DMC_L1_Data[MAXLINE * MAXWAY]; // direct mapping L1 data cache
DMCLine DMC_L1_Inst[MAXLINE * MAXWAY]; // direct mapping L1 instruction cache

/* set associative cache */
int SAC_Deque[MAXLINE][MAXWAY]; // deque for the set associative cache
SACLine SAC_L1_Data[MAXLINE][MAXWAY]; // set associative L1 data cache
SACLine SAC_L1_Inst[MAXLINE][MAXWAY]; // set associative L1 instruction cache

/* branch predictor unit variables */
uint8_t GHR; // global history register
uint8_t PHT[PHTENTRY]; // pattern history table
uint8_t LHT[LHTENTRY]; // local history register table
BHT_ELEMENT BHT[BHTENTRY]; // branch history table
