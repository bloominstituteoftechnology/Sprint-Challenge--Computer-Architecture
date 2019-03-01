#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  struct cpu cpu;

  cpu_init(&cpu);
  if (argc > 1)
  {
    cpu_load(&cpu, argv[1]);
  }
  else
  {
    printf("No instructions passed\n");
  }

  cpu_run(&cpu);

  return 0;
}