#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  struct cpu cpu;

  //add in argv check here
  if(argc != 2) {
    printf("Correct usage: ./ls8 <path>\n");
  }else{
    cpu_init(&cpu);
    cpu_load(&cpu, argv[1]);
    cpu_run(&cpu);
  }
  return 0;
}
