#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  struct cpu cpu;
  char *file = "examples/print8.ls8";
  if (argc == 2) {
    file == argv[1];
  }

  cpu_init(&cpu);
  cpu_load(&cpu, file);
  cpu_run(&cpu);

  return 0;
}