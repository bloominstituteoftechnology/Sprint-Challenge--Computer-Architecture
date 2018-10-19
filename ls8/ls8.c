#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  
  if (argc != 2) {
    return fprintf(stderr, "Format for input is './ls8' and '/path/to/file'\n");
    exit(1);
  } 

  char *filename = argv[1];

  struct cpu cpu; 

  cpu_init(&cpu);
  cpu_load(&cpu, filename);
  cpu_run(&cpu);  

  return 0;  
}