#include "calculator.h"


// fetch
void fetch() {
    strcpy(IR, Memory[PC]); // take the instruction into instruction register (IR).
    printf("FET: %s\n", IR);
    fprintf(fp_out, "FET: %s\n", IR);
}
