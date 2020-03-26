#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{

  if (argc != 2)
  {
    printf("To many arguments!, (2) allowed.\n");
    exit(1);
  }
  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu, argv);
  cpu_run(&cpu);

  return 0;
}