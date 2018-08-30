#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

extern int errno;

/**
 * Main
 */
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "\n\n=============================================================================================\n\n");
        fprintf(stderr, "#############\n##  Error  ##\n#############\n\n");
        fprintf(stderr, "Two arguments are required when attempting to run this program\n");
        fprintf(stderr, "\nExample: $ ./ls8 ./examples/print8.ls8\n\n");
        fprintf(stderr, "This is envoke the ls8 program and run the commands specified inside of the print8.ls8 file\n\n");
        fprintf(stderr, "=============================================================================================\n\n");
        exit(0);
    }
    
    struct cpu cpu;

    cpu_init(&cpu);
    cpu_load(&cpu, argv[1]);
    cpu_run(&cpu);

    return 0;
}