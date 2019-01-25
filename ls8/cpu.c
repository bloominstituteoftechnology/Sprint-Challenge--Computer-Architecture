#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

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

void cpu_load(char *filename, struct cpu *cpu)
{
  FILE *fp;
  char line[1024];
  int address = ADDR_PROGRAM_ENTRY;

  if ((fp = fopen(filename, "r")) == NULL)
  {
    fprintf(stderr, "Cannot open file %s\n", filename);
    exit(2);
  }

  while (fgets(line, sizeof line, fp) != NULL)
  {
    char *endchar;
    unsigned char byte = strtol(line, &endchar, 2);

    if (endchar == line)
    {
      continue;
    }

    cpu->ram[address++] = byte;
  }
}

void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *reg = cpu->reg;

  unsigned char valB = reg[regB];

  switch (op)
  {
    if (cpu->reg[regA] > cpu->reg[regB])
    {
      cpu->FL = 0b00000010;
    }
    else
    {
      cpu->FL = 0b00000100;
    }
    break;

  case ALU_MUL:
    reg[regA] *= valB;
    break;
  case ALU_ADD:
    reg[regA] += valB;
    break;
  }
}

void cpu_run(struct cpu *cpu)
{
  unsigned char *reg = cpu->reg;
  unsigned char *ram = cpu->ram;

  int running = 1;

  while (running)
  {
    unsigned char IR = ram[cpu->PC];
    unsigned char operandA = ram[(cpu->PC + 1) & 0xff];
    unsigned char operandB = ram[(cpu->PC + 2) & 0xff];

    int instruction_set_pc = (IR >> 4) & 1;

    switch (IR)
    {
    case JMP:
      cpu->PC = cpu->reg[operandA];
      break;

    case LDI:
      reg[operandA] = operandB;
      break;
      
    case PRN:
      printf("%d\n", reg[operandA]);
      break;

    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;

    case ADD:
      alu(cpu, ALU_ADD, operandA, operandB);
      break;

    case HLT:
      running = 0;
      break;

    case PRA:
      printf("%c\n", reg[operandA]);
      break;
    case CALL:
      cpu_push(cpu, cpu->PC + 2);
      cpu->PC = reg[operandA];
      break;

    case RET:
      cpu->PC = cpu_pop(cpu);
      break;

    case PUSH:
      cpu_push(cpu, reg[operandA]);
      break;

    case POP:
      reg[operandA] = cpu_pop(cpu);
      break;

    default:
      exit(3);
    }

    if (!instruction_set_pc)
    {
      cpu->PC += ((IR >> 6) & 0x3) + 1;
    }
  }
}

void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  cpu->FL = 0;
  memset(cpu->reg, 0, sizeof cpu->reg);
  memset(cpu->ram, 0, sizeof cpu->ram);
  cpu->reg[SP] = ADDR_EMPTY_STACK;
}