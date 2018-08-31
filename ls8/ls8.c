#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char* argv[])
{
  if (argc == 2) {
    struct cpu my_cpu;

    cpu_init(&my_cpu);
    cpu_load(&my_cpu, argv[1]);
    cpu_run(&my_cpu);
  }
  else{
    printf("You done messed up.\n");
  }

  return 0;
}