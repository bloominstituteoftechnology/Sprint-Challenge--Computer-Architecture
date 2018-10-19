#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, int index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, int index, unsigned char data)
{
  cpu->ram[index] = data;
}

void cpu_push(struct cpu *cpu, unsigned char val)
{
  cpu->reg[SP]--;

  cpu->ram[cpu->reg[SP]] = val;
}

unsigned char cpu_pop(struct cpu *cpu)
{
  unsigned char val = cpu->ram[cpu->reg[SP]];

  cpu->reg[SP]++;

  return val;
}
/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(char *filename, struct cpu *cpu)
{
  FILE *fp;
  char line[1024];
  int address = ADDR_PROGRAM_ENTRY;

  // Open the source file
  if ((fp = fopen(filename, "r")) == NULL)
  {
    fprintf(stderr, "Cannot open file %s\n", filename);
    exit(2);
  }

  // Read all the lines and store them in RAM
  while (fgets(line, sizeof line, fp) != NULL)
  {

    // Convert string to a number
    char *endchar;
    unsigned char byte = strtol(line, &endchar, 2);
    ;

    // Ignore lines from whicn no numbers were read
    if (endchar == line)
    {
      continue;
    }

    // Store in ram
    cpu->ram[address++] = byte;
  }
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *reg = cpu->reg;

  unsigned char valB = reg[regB];

  switch (op)
  {
  case ALU_MUL:
    reg[regA] *= valB;
    break;

  case ALU_ADD:
    reg[regA] += valB;
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

    unsigned char IR = cpu->ram[cpu->PC];

    unsigned char opA = cpu->ram[cpu->PC + 1];
    unsigned char opB = cpu->ram[cpu->PC + 2];
    unsigned char index = cpu->reg[7];
    int pcLoad = (IR >> 6) + 1;
    switch (IR)
    {
    case LDI:
      cpu->reg[opA] = opB;
      break;
    case PRN:
      printf("%d\n", cpu->reg[opA]);
      break;
    case MUL:
      alu(cpu, ALU_MUL, opA, opB);
      break;
    case PUSH:
      index--;
      cpu->ram[index] = cpu->reg[opA & 7];
      break;
    case POP:
      cpu->reg[opA & 7] = cpu->ram[index];
      index++;
      break;
    case JMP:
      cpu->PC = cpu->reg[opA & 7];
      pcLoad = 0;
      break;
    case CALL:
      index--;
      cpu->ram[index] = cpu->reg[opA];
      cpu->PC = cpu->reg[opA & 7];
      pcLoad = 0;
      break;
    case RET:
      cpu->PC = cpu->ram[index];
      index++;
      break;
    case CMP:
      if (cpu->reg[opA] < cpu->reg[opB])
      {
        cpu->FL[2] = 1;
      }
      else
      {
        cpu->FL[2] = 0;
      }
      if (cpu->reg[opA] > cpu->reg[opB])
      {
        cpu->FL[1] = 1;
      }
      else
      {
        cpu->FL[1] = 0;
      }
      if (cpu->reg[opA] == cpu->reg[opB])
      {
        cpu->FL[0] = 1;
      }
      else
      {
        cpu->FL[0] = 0;
      }
      break;

    case JEQ:
      if (cpu->FL[0] == 1)
      {
        cpu->PC = cpu->reg[opA];
        pcLoad = 0;
      }
      break;
    case JNE:
      if (cpu->FL[0] == 0)
      {
        cpu->PC = cpu->reg[opA];
        pcLoad = 0;
      }
      break;

    case HLT:
      running = 0;
      break;
    }
    cpu->PC += pcLoad;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;

  // TODO: Zero registers and RAM
  memset(cpu->reg, 0, sizeof cpu->reg);
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->FL, 0, sizeof(cpu->FL));

  // Initialize SP
  cpu->reg[SP] = ADDR_EMPTY_STACK;
}
