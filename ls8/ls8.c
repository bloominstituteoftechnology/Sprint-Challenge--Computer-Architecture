#include <stdio.h>
#include "cpu.h"

// One Ring to rule them all,
// One Ring to find them,
// One Ring to bring them all
// and in the darkness bind them.


// this is our main loop that runs the entire ls8 emulator
// hence the name of the file
int main(int argc, char **argv) {
  struct cpu cpu; // so to me this is like initializing an instance of cpu

  if (argc != 2) { // gotta make sure the user properly uses the program
    fprintf(stderr, "usage: ls8 file.ls8\n");
    return 1;
  }

  cpu_init(&cpu); 
  cpu_load(argv[1], &cpu);
  cpu_run(&cpu);
  return 0;
}