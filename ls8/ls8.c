#include <stdio.h>
#include "cpu.h"


/**
 * Main
 */
int main(int argc, char **argv)
{
  struct cpu cpu;

  if (argc !=2 ) {
    fprintf(stderr, "usage: ls8 filename\n");
    return 1;
  }

  char *filename = argv[1];

  cpu_init(&cpu);
  cpu_load(&cpu, filename);
  cpu_run(&cpu);

  return 0;
}