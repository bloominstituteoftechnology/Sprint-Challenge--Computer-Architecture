#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  struct cpu cpu;
  if (argc < 2)
  {
    printf("ERROR: Invalid file/directory. EXAMPLE: ./ls8 examples/print8.ls8\n");

    return 1;
  }
  cpu_init(&cpu);
  cpu_load(argv[1], &cpu);
  cpu_run(&cpu);

  return 0;
}