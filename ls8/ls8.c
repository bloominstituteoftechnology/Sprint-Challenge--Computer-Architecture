#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * Main
 * argc - argument count
 * argv - an array of strings that hold the individual arguments
 */
int main(int argc, char **argv)
{
  struct cpu cpu;

  // check to make sure the user has put a command line argument where you
  // expect, and print an error and exit if they didn't
  if (argc != 2)
  {
    fprintf(stderr, "usage: ls8 filename\n");
    exit(1);
  }

  char *filename = argv[1];

  cpu_init(&cpu);
  cpu_load(&cpu, filename);
  cpu_run(&cpu);

  return 0;
}