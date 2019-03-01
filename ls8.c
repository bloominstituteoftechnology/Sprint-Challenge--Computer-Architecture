#include <stdlib.h>
#include "cpu.h"

int main(int argc, char *argv[])
{
  // base case to ensure argument exists
  if (argc > 1)
  {
    struct cpu cpu;

    cpu_init(&cpu);
    cpu_loader(&cpu, argv[1]);
    cpu_runner(&cpu);
  }

  return 0;
} 