#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */

// 1. Argc --> argument counter
// 2. Argv --> array of strings that hold the individual args, starting with command name

int main(int argc, char *argv[])
{
  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu, argc, argv);
  cpu_run(&cpu);

  return 0;
}