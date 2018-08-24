#include <stdio.h>
#include "cpu.h"

// stdlib.h for exit
#include <stdlib.h>

/**
 * Main
 */

// Step 7.1: added argc and argv
int main(int argc, char *argv[])
{
  if (argc != 2){
    fprintf(stderr, "Incorrect Argument Length\nUsage: .ls8 example/[filename].ls8\n");
    exit(1);
  }

  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu, argv[1]);
  cpu_run(&cpu);

  return 0;
}