#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])  // main taking argc (argument count) and argv which is an array of strings
{
  struct cpu cpu; // initialize cpu structure

  // if (argc != 2)   // checck if the count/argc is not equal to 2 
  if (argc < 2) // if argument counter is less than 2  
  {    
    fprintf(stderr, "Usage: ./ls8 [filename]\n"); // print standard error if there's an error in the usage;
    exit(1);  // exit with code 1
  }

  char *filename = argv[1];  // create filename which will grab the first argument or the argument at index 1

  cpu_init(&cpu);

  cpu_load(&cpu, filename); // pass in the filename in cpu load; give a point to the file
  
  cpu_run(&cpu);

  return 0;
}
