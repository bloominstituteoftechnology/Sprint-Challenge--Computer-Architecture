#include "cpu.h"
#include <stdio.h>
#include <string.h>

#define DATA_LEN 6
// Last Register
#define LR 7

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char i)
{
  return cpu->ram[i];
}

void cpu_ram_write(struct cpu *cpu, unsigned char i, unsigned char val)
{
  cpu->ram[i] = val;
}

void cpu_load(struct cpu *cpu, char *myfile)
{
  FILE *fp;
  char buffer[512];
  fp = fopen(myfile, "r");
  unsigned char address = 0;

  while (fgets(buffer, sizeof buffer, fp) != NULL)
  {
    char *ptr;
    unsigned char string = strtol(buffer, &ptr, 2);
    if (ptr == buffer)
    {
      continue;
    }
    cpu->ram[++address] = string;
  }
  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    cpu->registers[regA] *= cpu->registers[regB];
    break;
    // TODO: implement more ALU ops
    /* case ALU_CMP: */
    /*   cpu->next = cpu_ram_read(cpu, cpu->PC++); */
    /*   if (cpu->registers[regA] = cpu->registers[regA] & cpu->registers[regB]){ */

    /*   } */
  }
}
// pop for cpu stack
void push(struct cpu *cpu, unsigned char val)
{
  cpu->registers[LR] = cpu->registers[LR] - 1;
  cpu_ram_write(cpu, cpu->registers[LR], val);
}
// pop for cpu stack
char pop(struct cpu *cpu)
{
  unsigned char myram = cpu->ram[cpu->registers[LR]];
  cpu->registers[LR] = cpu->registers[LR] + 1;
  return myram;
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char curr = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    unsigned int operands = curr >> 6;
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operandA = cpu_ram_read(cpu, (cpu->PC + 1) & 0xFF);
    unsigned char operandB = cpu_ram_read(cpu, (cpu->PC + 2) & 0xFF);
    // 4. switch() over it to decide on a course of action.
    switch (curr)
    {
    // HLT -- Halt the CPU and exit the emulator:
    case HLT:
      running = 0;
      break;
    case LDI:
      cpu->registers[operandA] = operandB;
      break;
    case PRN:
      printf("%d\n", cpu->registers[operandA]);
      break;
    case MULT:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;
    case PUSH:
      push(cpu, cpu->registers[operandA]);
      break;
    case POP:
      cpu->registers[operandA] = pop(cpu);
      break;
    default:
      break;
    }
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    cpu->PC = cpu->PC + operands + 1;
    // TODO: Initialize the PC and other special registers
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  memset(cpu->registers, 0, sizeof cpu->registers);
  memset(cpu->ram, 0, sizeof cpu->ram);
  cpu->equal = 0;
}
