#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  // check if arguemnt count is more than one or if it's the correct count
  struct cpu cpu;

  if(argc != 2){
    fprintf(stderr, "Usage: //ls8 [filename]\n");
    exit(1);
  }
  //grab the first argument at index 1
  char *filename = argv[1];
  cpu_init(&cpu);
  cpu_load(&cpu, filename);
  cpu_run(&cpu);

  return 0;
}