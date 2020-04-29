#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  if (argc != 2) {
    printf("Incorrect number of arguments");
    exit(1);
  }
  
  char *programToLoad = argv[1];  // path to the program that the user entered in the command line
  
  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu, programToLoad);
  cpu_run(&cpu);

  return 0;
}