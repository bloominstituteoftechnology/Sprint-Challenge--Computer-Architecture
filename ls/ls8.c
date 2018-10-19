#include <stdio.h>
#include "cpu.h"
#include <stdio.h>

/**
 * Main
 */
int main(int argc, char **argv)
{
  printf("\nStarting Main....\n");
    
  //Instantiate the cpu structure:
  struct cpu cpu;

  if (argc != 2){
    fprintf(stderr, "usage: ls8 filename.ls8\n");
    return 1;
  }
  
  //Call the initialize function on address of cpu:
  cpu_init(&cpu);
  cpu_load(&cpu, argv[1]); 
  cpu_run(&cpu);

  return 0;
}