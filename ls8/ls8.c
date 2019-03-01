#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char **argv)
{
  if (argc != 2)
  {
    printf("usage: ./lsa.c [filename]\n");
    return (1);
  }
  printf("--init--\n");
  struct cpu cpu;
  char *filename = argv[1];
  cpu_init(&cpu);
  cpu_load(&cpu, filename);
  cpu_run(&cpu);

  return 0;
}