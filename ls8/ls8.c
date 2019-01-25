#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  if (argv[0] == NULL) {
    fprintf(stderr, "No source file loaded!");
    exit(1);
  }

  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu, argv[1]); // load the specified program into the CPUs RAM
  cpu_run(&cpu); // run the program

  return 0;
}
