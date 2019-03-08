#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
	if (argc != 2){
		fprintf(stderr, "Invalid number of arguments. Add arg to ls8 file.\n");
		return 1;
	}
  
  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu, argv[1]);
  cpu_run(&cpu);

  return 0;
}