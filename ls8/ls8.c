#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{

  struct cpu cpu;
  char *filename;

  if(argc < 2){
    printf("You need run the ./ls8 with another argument");
    exit(1);
  } else if(argv[0] == NULL || argv[1] == NULL){
    printf("You need run the ./ls8 with another argument");
    exit(1);
  } else {
    filename = argv[1];
    cpu_init(&cpu);
    cpu_load(&cpu, filename);
    cpu_run(&cpu);
  }

  return 0;
}