#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file)
{
  FILE *fp;
  char line[1024];
  fp = fopen(file, "r");
  int address = 0;

  while (fgets(line, 1024, fp))
  {
    char *endptr;
    unsigned char val = strtol(line, &endptr, 2);
    if (endptr == line)
    {
      continue;
    }
    cpu->ram[address] = val;
    address++;
  }

  fclose(fp);
}

unsigned char cpu_ram_read(struct cpu *cpu, int index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, int index, unsigned char new_value)
{
  cpu->ram[index] = new_value;
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    // TODO
    cpu->registers[regA] *= cpu->registers[regB];
    break;
  case ALU_ADD:
    cpu->registers[regA] += cpu->registers[regB];
    break;
  }
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
    unsigned char current_instruction = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    unsigned int operands = current_instruction >> 6;
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB;
    if (operands > 1)
    {
      operandB = cpu_ram_read(cpu, cpu->PC + 2);
    }
    // 4. switch() over it to decide on a course of action.
    switch (current_instruction)
    {
      // 5. Do whatever the instruction should do according to the spec.
    case HLT:
      running = 0;
      break;
    case LDI:
      cpu->registers[operandA] = operandB;
      break;
    case PRN:
      printf("%d\n", cpu->registers[operandA]);
      break;
    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;
    case PUSH:
      cpu->registers[7]--;
      cpu_ram_write(cpu, cpu->registers[7], cpu->registers[operandA]);
      break;
    case POP:
      cpu->registers[operandA] = cpu_ram_read(cpu, cpu->registers[7]);
      cpu->registers[7]++;
      break;
    case RET:
      cpu->PC = cpu_ram_read(cpu, cpu->registers[7]);
      cpu->registers[7]++;
      break;
    case CALL:
      cpu->registers[7]--;
      cpu_ram_write(cpu, cpu->registers[7], cpu->PC + 2);
      cpu->PC = cpu->registers[operandA];
      break;
    case ADD:
      alu(cpu, ALU_ADD, operandA, operandB);
      break;
    default:
      break;
    }
    if (!(current_instruction >> 4 & 1))
    {
      cpu->PC += operands + 1;
    }
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
}
