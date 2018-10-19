#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char **argv)
{
  if(argc > 1)
  {
    struct cpu cpu;
    cpu_init(&cpu);
    cpu_load(&cpu, argv[1]);
    cpu_run(&cpu);
  }else{
    printf("Please enter a valid filename\n");
  }

  return 0;
}