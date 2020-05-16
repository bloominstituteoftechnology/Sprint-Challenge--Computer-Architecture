#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
//the way this is set up shows us the order that this should be built out
int main(int argc, char *argv[]) //changed from void for command line processing. arg count and array of strings that hold indiv args.
{
  struct cpu cpu;

  if (argc != 2)
  {
    fprintf(stderr, "This function requires two arguments.");
    return 1;  //same as exit(1) b/c normally return 0
  }

  char *filename = argv[1];

  cpu_init(&cpu);
  cpu_load(&cpu, filename);  //also update prototype to include char pointer
  cpu_run(&cpu);

  return 0;
}


//argv[0] == "./ls8"
//argv[1] == "examples/mult.ls8"