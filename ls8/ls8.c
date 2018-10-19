#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[]) // need to take arguments
{
  struct cpu cpu;
  
  if (argc != 2)
  {
    fprintf(stderr, "Oops - usage ./ls8 filename\n");
    exit(1);
  }
  cpu_init(&cpu);
  cpu_load(&cpu, argv);
  cpu_run(&cpu);

  return 0;
}
