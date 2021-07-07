#include <stdio.h>
#include "cpu.h"
#include <string.h>

/**
 * Main
 */
int main(int argc, char *argv[])
{
  struct cpu cpu;

  char path[50] = "./examples/";
  strcat(path, argv[1]);
  cpu_init(&cpu);
  cpu_load(&cpu, path);
  cpu_run(&cpu);

  return 0;
}
