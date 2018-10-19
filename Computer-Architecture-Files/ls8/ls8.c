#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  struct cpu cpu;
  
  cpu_init(&cpu);

  // argv[1] is the file we want to open
  cpu_load(&cpu, argv[1]);
  cpu_run(&cpu);

  return 0;
}