#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fptr;
  char line[1024];
  unsigned char address = 0;
  fptr = fopen(filename, "r");

  if (fptr == NULL)
  {
    fprintf(stderr, "Error opening file");
    exit(1);
  }

  while (fgets(line, sizeof line, fptr) != NULL)
  {

    unsigned char b;
    b = strtoul(line, NULL, 2);
    cpu_ram_write(cpu, address++, b);
  }
  fclose(fptr);
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
  case ALU_ADD:
    cpu->registers[regA] += cpu->registers[regB];
    break;
  case ALU_CMP:
    if (cpu->registers[regA] > cpu->registers[regB])
    {
      cpu->flag = 00000010;
    }
    else if (cpu->registers[regA] < cpu->registers[regB])
    {
      cpu->flag = 00000100;
    }
    else
    {
      cpu->flag = 00000001;
    }
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1;

  while (running)
  {

    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, (cpu->PC + 1));
    unsigned char operandB = cpu_ram_read(cpu, (cpu->PC + 2));

    int add_to_pc = (IR >> 6) + 1;

    switch (IR)
    {
    case LDI:
      cpu->registers[operandA] = operandB;
      break;
    case PRN:
      printf("%d \n", cpu->registers[operandA]);
      break;
    case HLT:
      running = 0;
      break;
    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;
    case PUSH:
      cpu->registers[7] -= 1;
      cpu->ram[cpu->registers[7]] = cpu->registers[operandA];
      break;
    case POP:
      cpu->registers[operandA] = cpu->ram[cpu->registers[7]];
      cpu->registers[7]++;
      break;
    case ADD:
      alu(cpu, ALU_ADD, operandA, operandB);
      break;
    case CALL:
      cpu->registers[7] -= 1;
      cpu->ram[cpu->registers[7]] = cpu->PC + 2;
      cpu->PC = cpu->registers[operandA];
      add_to_pc = 0;
      break;
    case RET:
      cpu->PC = cpu->ram[cpu->registers[7]];
      cpu->registers[7] += 1;
      add_to_pc = 0;
      break;
    case CMP: //opcode to compare two registers
      alu(cpu, ALU_CMP, operandA, operandB);
      break;
    case JMP: // opcode to jump to the address stored at the given register
      cpu->PC = cpu->registers[operandA];
      add_to_pc = 0;
      break;
    case JEQ: // opcode to jump if the  equal flag is set to true
      if (cpu->flag == 00000001)
      {
        cpu->PC = cpu->registers[operandA];
        add_to_pc = 0;
      }
      break;
    case JNE:
      if (cpu->flag != 00000001)
      {
        cpu->PC = cpu->registers[operandA];
        add_to_pc = 0;
      }
      break;
    }

    cpu->PC += add_to_pc;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;

  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->registers, 0, sizeof cpu->registers);
}