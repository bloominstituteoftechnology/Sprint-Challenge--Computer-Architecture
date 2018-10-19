#include <string.h>
#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char **argv)
{
  struct cpu cpu;
  _Bool show_trace = 0;

  if (argc == 1)
  {
    fprintf(stderr, "enter an ls8 file and optional trace: ls8 file.ls8 1\n");
    return 1;
  }
  if (argc == 3)
  {
    if (strcmp(argv[2], "1") == 0 || strcmp(argv[2], "t") == 0)
    {
      show_trace = 1;
    }
  }

  cpu_init(&cpu);
  cpu_load(argv[1], &cpu);
  cpu_run(&cpu, show_trace);

  return 0;
}