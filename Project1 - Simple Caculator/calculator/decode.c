#include "calculator.h"


// decode
struct inst_t* decode() {
    FILE *file = NULL;
    char *ptr = NULL;
    char temp[9] = {".", };
    struct inst_t *inst = malloc(sizeof(struct inst_t)); 

    // opcode
    ptr = strtok(IR, " ");
    switch (*ptr)
    {
        case '+': inst->op = ADD; break;
        case '-': inst->op = SUB; break;
        case '*': inst->op = MUL; break;
        case '/': inst->op = DIV; break;
        case 'M': inst->op = MOVE; break;
        case 'H': inst->op = HALT; break;
        case 'J': inst->op = JUMP; break;
        case 'C': inst->op = COMPARE; break;
        case 'B': inst->op = BRANCH; break;
        case 'G': inst->op = GCD; break;
        default: inst->op = NO_OP; break;
    }

    // operand1
    ptr = strtok(NULL, " ");
    if (ptr != NULL) {
        switch (*ptr)
        {
            case 'R': inst->opr1.type = REGISTER; inst->opr1.value = atoi(ptr + 1); break;
            case '0': inst->opr1.type = IMMEDIATE; inst->opr1.value = strtol(ptr, NULL, 16); break;
            default: inst->opr1.type = NONE; inst->opr1.value = 0; break;
        }
    }
    else  {
        inst->opr1.type = NONE;
        inst->opr1.value = 0;
    }


    // operand2
    ptr = strtok(NULL, " ");
    if (ptr != NULL) {
    switch (*ptr)
        {
            case 'R': inst->opr2.type = REGISTER; inst->opr2.value = atoi(ptr + 1); break;
            case '0': inst->opr2.type = IMMEDIATE; inst->opr2.value = strtol(ptr, NULL, 16); break;
            default: inst->opr2.type = NONE; inst->opr2.value = 0; break;
        }
    }
    else  {
        inst->opr2.type = NONE;
        inst->opr2.value = 0;
    }

    // write the log
    printf("DEC: %d.%d.%d.%d.%d\n", inst->opr1.type, inst->opr1.value, inst->op, inst->opr2.type, inst->opr2.value);
    fprintf(fp_out, "DEC: %d.%d.%d.%d.%d\n", inst->opr1.type, inst->opr1.value, inst->op, inst->opr2.type, inst->opr2.value);

    // return the instruction
    return inst;
    free(inst);
}