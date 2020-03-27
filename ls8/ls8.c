#include <stdio.h>
#include "cpu.h"
#include <string.h> // for handling argv inputs
/**
 * Main
 */
int main(int argc, char *argv[])
{
  struct cpu cpu;

  cpu_init(&cpu);

  // TODO: Implement a file search function that looks for filenames in nearby directories
  // Without this, the user will have to input the direct file path in the command like e.g. ./examples/call.ls8 instead of simply call.ls8

  if(argc > 1){
    cpu_load(&cpu, argv[1]); // if a filename is given, load that file
  } else {
    cpu_load(&cpu, "./examples/print8.ls8"); // otherwise, load the print example
  }
  cpu_run(&cpu);

  return 0;
}