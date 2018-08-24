#include <stdio.h>
#include "cpu.h"
#include <stdlib.h>

/**
 * Main
 */
int main(int argc, char **argv)
{
  printf("***CHECK FOR ARGUMENTS: argc = %d\n", argc);
  if (argc == 1)
  {
    perror("ERROR A file path must be provided.\n");
    exit(1);
  }

  printf("main 0: START\n");
  struct cpu cpu;

  printf("main 1\n");
  cpu_init(&cpu);
  printf("main 2\n");
  cpu_load(&cpu, argv[1]);
  printf("main 3\n");
  cpu_run(&cpu);
  printf("\nmain 4: END\n");

  return 0;
}
