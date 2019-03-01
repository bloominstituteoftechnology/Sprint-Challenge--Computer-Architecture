#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cpu.h"

#define SP 7

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

void cpu_load(struct cpu *cpu, char *argv[])
{
  FILE *fp;
  char data[1024];
  unsigned char address = 0;
  fp = fopen(argv[1], "r");

  if (!fp)
  {
    printf("Cannot opening file \n");
    exit(1);
  }

  while (fgets(data, sizeof(data), fp) != NULL)
  {
    char *ptr;
    unsigned char ret = strtol(data, &ptr, 2);
    if (ptr == data)
    {
      continue;
    }
    cpu->ram[address++] = ret;
  }

  fclose(fp);
}

void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    cpu->reg[regA] *= cpu->reg[regB];
    break;
  case ALU_ADD:
    cpu->reg[regA] += cpu->reg[regB];
    break;
  case ALU_CMP:
    if (cpu->reg[regA] == cpu->reg[regB])
    {
      cpu->FL = 1;
    }
    else
    {
      cpu->FL = 0;
    }
    break;

  default:
    break;
  }
}

unsigned char pop(struct cpu *cpu)
{
  unsigned char ret = cpu->ram[cpu->reg[SP]];
  cpu->reg[SP]++;
  return ret;
}

void push(struct cpu *cpu, unsigned char val)
{
  cpu->reg[SP]--;
  cpu->ram[cpu->reg[SP]] = val;
}

void cpu_run(struct cpu *cpu)
{
  int running = 1;

  while (running)
  {
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operand_a = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operand_b = cpu_ram_read(cpu, cpu->PC + 2);
    int num_operands = (IR >> 6) + 1;

    switch (IR)
    {
    case LDI:
      cpu->reg[operand_a] = operand_b;
      break;
    case PRN:
      printf("%d \n", cpu->reg[operand_a]);
      break;
    case MUL:
      alu(cpu, ALU_MUL, operand_a, operand_b);
      break;
    case ADD:
      alu(cpu, ALU_ADD, operand_a, operand_b);
      break;
    case PUSH:
      push(cpu, cpu->reg[operand_a]);
      break;
    case POP:
      cpu->reg[operand_a] = pop(cpu);
      break;
    case CALL:
      push(cpu, cpu->PC + 2);
      cpu->PC = cpu->reg[operand_a];
      num_operands = 0;
      break;
    case RET:
      cpu->PC = pop(cpu);
      num_operands = 0;
      break;
    case CMP:
      alu(cpu, ALU_CMP, operand_a, operand_b);
      break;
    case JMP:
      cpu->PC = cpu->reg[operand_a];
      num_operands = 0;
      break;
    case JEQ:
      if (cpu->FL == 1)
      {
        cpu->PC = cpu->reg[operand_a];
        num_operands = 0;
      }
      break;
    case JNE:
      if (cpu->FL == 0)
      {
        cpu->PC = cpu->reg[operand_a];
        num_operands = 0;
      }
      break;
    case HLT:
      running = 0;
      break;
    }
    cpu->PC += num_operands;
  }
}

void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  cpu->reg[SP] = 0xF4;
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->reg, 0, sizeof cpu->reg);
  cpu->FL = 0x00;
}