#include <stdio.h>
#include "cpu.h"
#include <stdlib.h>

/**
 * Main
 */
int main(int argc, char **argv){
  if (argc < 2)
  {
    puts("please specfiy a file in the examples directory");
    exit(-1);
  }
  struct cpu cpu;
  
  char *file = argv[1];


  cpu_init(&cpu);
  cpu_load(&cpu, file);
  cpu_run(&cpu);

  return 0;
}