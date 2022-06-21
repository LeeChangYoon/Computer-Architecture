#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instruction.h"

#define line_size 1024
#define memory_size 0x8000


char* PROGRAM;


int flag; // flag for halt 
int R[10]; // register
int PC; // program counter
FILE *fp_out; // output.txt
char IR[line_size]; // instruction register
char Memory[memory_size][line_size]; // main memory


void load(); // load instructions to memory
void fetch(); // fetch instruction into instruction registerr (IR)
struct inst_t* decode(); // decode the instruction in IR
void execute(struct inst_t *inst); // execute the instruction
int gcd(int a, int b); // returns greatest common divisor
void store(); // print the result on the console screen and write log on output.txt
