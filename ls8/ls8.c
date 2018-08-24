#include <stdio.h>
#include "cpu.h"
#include <stdlib.h>

/**
 * Main
 */
int main(int argc, char *argv[])
{

  if (argc < 2){
    fprintf(stderr, "Usage: ./ls8 <filename>\n");
    exit(1);
  }  

  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu, argv);
  cpu_run(&cpu);

  return 0;
}