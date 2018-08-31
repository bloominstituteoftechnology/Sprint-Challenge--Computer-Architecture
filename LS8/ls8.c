#include <stdio.h>
#include "retyped_cpu.h"

// argc is the argument count, and argv is an array of strings that hold 
// the individual arguments, starting with the command name itself
int main(int argc, char *argv[])
{
  struct cpu cpu;

  if (argc != 2)
  {
    // check to make sure the user has put a command line argument where 
    // you expect, and print an error and exit if they didn't
    fprintf(stderr, "usage: ./ls8 [filename].ls8\n");
    return 1;
  }

  cpu_init(&cpu);
  // argv[1] = the filename
  cpu_load(argv[1], &cpu);
  cpu_run(&cpu);

  return 0;
}
