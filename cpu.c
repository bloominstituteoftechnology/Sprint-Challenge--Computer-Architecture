#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define DEBUG 0
/**
* Load the binary bytes from a .ls8 source file into RAM array
*/
void cpu_load(char* arg, struct CPU * cpu){
    FILE * fp;
    char line[1024];
    int line_no = 0;
    char *pointer;

    fp = fopen(arg, "r");// open file and read

    if (fp == Null){
        fprint(stderr, "Failed to open %s\n", arg);
        exit(1);// error message if it doesnt open if message and exit
    }
    else{
        while(fgets(line, sizeof(line), fp) != NULL) {
            unsigned char vlue = strtoul(line, & pointer, 2);
            if (pointer == line){
                continue;
            }
            cpu_ram-write(cpu, line_no++, value);

        }
        fclose(fp);//close file
    }
}
