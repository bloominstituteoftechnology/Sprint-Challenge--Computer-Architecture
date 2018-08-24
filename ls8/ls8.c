#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "cpu_instr.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("usage: ./ls8 [program_path]");
    exit(1);
  }

  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu, argv[1]);
  cpu_run(&cpu);

  return 0;
}