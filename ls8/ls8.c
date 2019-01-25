#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{ 
  printf("\nStarting Main ls8.c....\n");
  

  if (argc !=2) {
    printf( "usage: ./ls8 filename\n");
    return 1;
  }
  struct cpu cpu;
  
  cpu_init(&cpu);
  cpu_load(&cpu, argv);                 //cpu_load(&cpu, argv[1]);
  cpu_run(&cpu);

  return 0;
}