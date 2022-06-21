#include "calculator.h"


// load
void load() {
    int i = 0;
    FILE *fp_in = NULL;
    char line[line_size];

    // check if there is input.txt 
    fp_in = fopen(PROGRAM, "r");
    if (fp_in == NULL) {
        printf("program not found\n");
        exit(0);
    }
    while (fgets(line, sizeof(line), fp_in) != NULL) {
        line[strlen(line) - 1] = '\0';
        strcpy(Memory[i], line);
        i++;
    }
    // close the input.txt
    fclose(fp_in);
}
