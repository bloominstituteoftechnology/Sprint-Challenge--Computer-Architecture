#include <stdio.h>
#include "cpu.h"
#include <stdlib.h>

/**
 * Main
 */
int main(int argc, char **argv)
{
  struct cpu cpu;
  
  if (argc != 2) {
    fprintf(stderr, "Provide a file to read! USAGE: ./ls8 filename\n");
    exit(1);
  }
  char *filename = argv[1];

  cpu_init(&cpu);
  cpu_load(filename, &cpu);
  cpu_run(&cpu); 

  return 0;
}