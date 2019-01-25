#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  printf("\nStarting Main....\n");
  struct cpu cpu;
  if (argc != 2) {
    printf("Usage: ./ls8 filename \n");
    return 1;
  }

  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu, argv[1]);
  cpu_run(&cpu);

  return 0;
}