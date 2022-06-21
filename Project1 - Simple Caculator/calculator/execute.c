#include "calculator.h"


// execute
void execute(struct inst_t *inst) {
    int opr1 = 0;
    int opr2 = 0;

    // operand1
    switch (inst->opr1.type)
    {
        case IMMEDIATE: opr1 = inst->opr1.value; break;
        case REGISTER:
            if (inst->op == MOVE) opr1 = inst->opr1.value;
            else opr1 = R[inst->opr1.value]; 
            break;
        default: break;
    }

    // operand2
    switch (inst->opr2.type)
    {
        case IMMEDIATE: opr2 = inst->opr2.value; break;
        case REGISTER: opr2 = R[inst->opr2.value]; break;
        default: break;
    }

    // execute
    switch (inst->op)
    {
        case ADD: 
            R[0] = opr1 + opr2;
            printf("EXE: R[0]: 0x%x = 0x%x + 0x%x\n", R[0], opr1, opr2);
            fprintf(fp_out, "EXE: R[0]: 0x%x = 0x%x + 0x%x\n", R[0], opr1, opr2);
            break;
        case SUB: 
            R[0] = opr1 - opr2;
            printf("EXE: R[0]: 0x%x = 0x%x - 0x%x\n", R[0], opr1, opr2);
            fprintf(fp_out, "EXE: R[0]: 0x%x = 0x%x - 0x%x\n", R[0], opr1, opr2);
            break;
        case MUL: 
            R[0] = opr1 * opr2;
            printf("EXE: R[0]: 0x%x = 0x%x * 0x%x\n", R[0], opr1, opr2);
            fprintf(fp_out, "EXE: R[0]: 0x%x = 0x%x * 0x%x\n", R[0], opr1, opr2);
            break;
        case DIV: 
            R[0] = opr1 / opr2;
            printf("EXE: R[0]: 0x%x = 0x%x / 0x%x\n", R[0], opr1, opr2);
            fprintf(fp_out, "EXE: R[0]: 0x%x = 0x%x / 0x%x\n", R[0], opr1, opr2);
            break;
        case MOVE: 
            R[opr1] = opr2; 
            printf("EXE: R[%d]: 0x%x\n", opr1, opr2);
            fprintf(fp_out, "EXE: R[%d]: 0x%x\n", opr1, opr2);
            break;
        case HALT: 
            printf("EXE: Program Terminated\n");
            fprintf(fp_out, "EXE: Program Terminated\n");
            flag = 1; 
            exit(0);
        case JUMP: 
            PC = opr1 - 1; 
            printf("EXE: Jump to %d\n", opr1);
            fprintf(fp_out, "EXE: Jump to %d\n", opr1);
            break;
        case COMPARE: 
            if (opr1 >= opr2) R[0] = 0;
            else R[0] = 1;
            printf("EXE: R[0]: %d\n", R[0]);
            fprintf(fp_out, "EXE: R[0]: %d\n", R[0]);
            break;
        case BRANCH: 
            if (R[0] == 1) {
                PC = opr1 - 1;
                printf("EXE: Jump to %d\n", opr1);
                fprintf(fp_out, "EXE: Jump to %d\n", opr1);
            }
            break;
        default: 
            printf("EXE: Program Terminated\n");
            fprintf(fp_out, "EXE: Program Terminated\n");
            break;
    }
}

