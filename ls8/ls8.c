#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
/*
int main(void)
{
  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu);
  cpu_run(&cpu);

  return 0;
}
*/

// `argc` argument count
// `argv` array of strings containing the arguments
// below, both ways ok??
// int main(int argc, char *argv[])
int main(int argc, char **argv)
{
  struct cpu cpu;

  if (argc != 2) {
    fprintf(stderr, "ERROR(user usage)\n\tEXAMPLE: ./ls8 examples/file.ls8\n");
    return 1;
  }

  cpu_init(&cpu);
  cpu_load(argv[1], &cpu);
  cpu_run(&cpu);

  return 0;
}