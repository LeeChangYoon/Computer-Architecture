//------------------------------------------------------------------------
// File    : calculator.c
// Author  : ChangYoon Lee
// Written : 5 Mar 2022
// 
// Simple Calculator which contains following operations
// 1) Change operand1, operand2 into operand1, operand2
// 2) Arithmetic operations (+, -, *, /)
// 3) Implement Move (M) operation
// 4) Implement Halt (H) operation
// 5) Implement Jump (J) operation
// 6) Implement Compare (C) operation
// 7) Implement Branch (B) operation
// 8) Implement GCD (G) operation
//
// Copyright (C) 2022 Dankook.Univ
//------------------------------------------------------------------------


#include "calculator.h"


// main
int main(int argc, char* argv[]) {
    flag = 0;
    PC = 0x0000;
    fp_out = NULL;
    PROGRAM = argv[1];
    struct inst_t *inst = malloc(sizeof(struct inst_t));

    // initialize output.txt
    fp_out = fopen("output.txt", "w");
    if (fp_out == NULL){
        printf("output.txt not found\n");
        return 0;
    }
    fclose(fp_out);

    // open the output.txt
    fp_out = fopen("output.txt", "a");
    if (fp_out == NULL){
        printf("output.txt not found\n");
        return 0;
    }

    load();
    while (Memory[PC][0] && !flag) {
        fetch();
        inst = decode();
        execute(inst);
        printf("\n");
        fprintf(fp_out, "\n");
        PC++;
    }

    // close the output.txt
    fclose(fp_out);
    free(inst);
    return 0;
}
