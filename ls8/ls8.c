#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char * argv[])
{
  struct cpu cpu;
  if(argc ) {
    
  }
  cpu_init(&cpu);
  cpu_load(&cpu, argv);
  cpu_run(&cpu);

  return 0;
}