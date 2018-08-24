#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * Main
 */
//int main(void)
int main(int argc, char **argv)
{
  struct CPU cpu;
  if (argc < 2) {
    fprintf(stderr, "usage: ./ls8 + example/<programfile> \n");
    exit(2);
  }
  else {
    char *filename = argv[1];
    cpu_init(&cpu);
    cpu_load(filename, &cpu);
    cpu_run(&cpu);
  }



  return 0;
}