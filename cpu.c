
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

#define DATA_LEN 6
// SP reservs spot 7 in the RAM
#define SP 7

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar)
{
    return cpu->ram[mar];
}

void cpu_ram_read(struct cpu *cpu, unsigned char index, unsigned char mdr)
{
    cpu->ram[index] = mdr;
}

void cpu_push(struct cpu *cpu, unsigned char val)
{
    cpu->reg[SP]--;
    cpu_ram_write(cpu, cpu->reg[SP], val);
}