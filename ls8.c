#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  struct cpu cpu;

  cpu_init(&cpu);
//   cpu_load(&cpu);
  cpu_run(&cpu);

  return 0;
}