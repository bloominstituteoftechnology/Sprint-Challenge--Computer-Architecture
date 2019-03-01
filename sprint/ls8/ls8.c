#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
/****** DAY-II ********
Make changes to `cpu.c` and `ls8.c` so that the program can be specified on the
command line like
./ls8 examples/mult.ls8 */

/**
 * Main
 */
int main(int argc, char *argv[])
{
    struct cpu cpu;
    //check to make sure the user has put a command line argument where you expect,
    // and print an error and exit if they didn't...
    if (argc < 2) {
        fprintf(stderr, "./ls8 + example/<programfile_name..ls8> \n");
        exit(0);
    }
    else {
        /*argv[0] == "./ls8"
          argv[1] == "examples/mult.ls8" */
        char *filename = argv[1];
        printf("\n%s\n",argv[1] );
        cpu_init(&cpu);
        cpu_load(filename, &cpu);
        cpu_run(&cpu);
    }
    return 0;
}