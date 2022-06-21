#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "register.h"
#include "instruction.h"


char* PROGRAM;
#define REGSIZE	32 
#define MEMSIZE	0x1000000 // 16MB	


// instruction type
enum inst_type { J_type, I_type, R_type };


// instruction structure
typedef struct instruction {
    uint8_t opcode;
    uint8_t rs;
    uint8_t rt;
    uint8_t rd;
    uint8_t shamt;
    uint8_t funct;
    uint32_t immediate;
    uint32_t address;
} instruction;


// control unit variables
bool RegDst;
bool SignEx;
bool Shift;
bool ALUSrc;
bool MemtoReg;
bool RegWrite;
bool MemRead;
bool MemWrite;
bool Branch;
bool Jump;
bool JumpReg;
bool JumpLink;
bool Rtype;
bool Equal;
bool RA;
uint8_t ALUOp;


// addr
uint32_t PCAddr(uint32_t PC);
uint32_t SignExtend(uint16_t immediate);
uint32_t ZeroExtend(uint16_t immediate);
uint32_t JumpAddr(uint32_t PC, uint32_t address);
uint32_t BranchAddr(uint32_t PC, uint32_t immediate);

// control unit
void CU_Init();
void CU_Operation(uint8_t opcode, uint32_t funct);

// instruction memory
void IM_Init();
uint32_t IM_ReadMemory(uint32_t ReadAddress);

// registers
void RF_Init();
void RF_Read(uint8_t RdReg1, uint8_t RdReg2);
void RF_Write(uint8_t WrtReg, uint32_t WrtData, bool WrtEnable);

// ALU
uint8_t ALU_Control(uint8_t ALUOp, uint8_t opcode);
uint32_t ALU_Operation(uint8_t ALU_Control, uint32_t operand1, uint32_t operand2);

// data memory
void DM_Init();
void DM_MemoryAccess(uint32_t Address, int size, uint32_t WriteData, bool MemRead, bool MemWrite);

// main
void init_all();
void terminate();
void Inst_Decode();
void Print_Decode();
void Print_Execute();
uint32_t MUX(uint32_t IN1, uint32_t IN2, bool S);


// instruction count
int inst_count;
int R_count;
int I_count;
int J_count;
int M_count;
int B_count;


// CPU variables
bool bcond; // branch condition
uint32_t IR; // instruction register
uint32_t PC; // program counter
instruction* inst; // instruction
uint8_t ALUControl; // output of ALU control units
uint32_t ALUResult; // output of ALU unit opration
uint32_t ReadData; // output of data memory
uint32_t ReadData1; // output of RF
uint32_t ReadData2; // output of RF
uint32_t Register[REGSIZE]; // register
uint8_t InstMemory[MEMSIZE]; // instruction memory
uint8_t DataMemory[MEMSIZE]; // data memory
