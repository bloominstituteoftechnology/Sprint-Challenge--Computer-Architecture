#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
/**
 * Main
 */
int main(int argc, char **argv)
{
    struct cpu cpu;
    if (argc != 2)
    {
        fprintf(stderr, "usage: main <filename>\n");
        exit(1);
    }

    char *filename = argv[1];

    cpu_init(&cpu);
    cpu_load(&cpu, filename);
    cpu_run(&cpu);

    return 0;
}