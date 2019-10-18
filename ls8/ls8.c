#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  struct cpu cpu;

  cpu_init(&cpu);
  
  if(argc > 1){
    cpu_load(&cpu, argv[1]); // if there is a filename
  }
  else
  {
    cpu_load(&cpu, "./examples/print8.ls8"); // load the print example
  }
  
  cpu_run(&cpu);

  return 0;
}