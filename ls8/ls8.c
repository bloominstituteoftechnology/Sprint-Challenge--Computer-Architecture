#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
    struct cpu cpu;
    if (argc != 2)
    {
        fprintf(stderr, "usage: ls8 file.ls8\n");
        exit(2);
    }

    cpu_init(&cpu);
    cpu_load(&cpu, argv[1]);
    cpu_run(&cpu);

    return 0;
}