#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  struct cpu cpu;

  if (argc != 2) {
    printf("\033[0;31mERROR\033[0m Usage: ./ls8 [program]\n");
  } else {
    cpu_init(&cpu);
    cpu_load(&cpu, argv[1]);
    cpu_run(&cpu);
  }

  return 0;
}