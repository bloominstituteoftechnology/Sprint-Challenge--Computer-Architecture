#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  if (argc > 2 || argc < 2)
  {
    fprintf(stderr, "Usage: %s /path/to/file.ls8\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu, argv[1]);
  cpu_run(&cpu);

  return 0;
}
