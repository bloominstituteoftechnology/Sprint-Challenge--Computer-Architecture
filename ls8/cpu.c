#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
  FILE *fp;
  char line[1024];
  int address = 0;
  if ((fp = fopen(filename, "r")) == NULL)
  {
    fprintf(stderr, "File not found");
    exit(2);
  }
  while (fgets(line, sizeof line, fp) != NULL)
  {
    char *endchar;
    unsigned char j = strtol(line, &endchar, 2);

    if (endchar == line)
    {
      continue;
    }
    cpu->ram[address++] = j;
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    cpu->reg[regA] = cpu->reg[regA] * cpu->reg[regB];
    break;
  case ALU_ADD:
    cpu->reg[regA] += cpu->reg[regB];
    break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  // Just so we don't have to type cpu-> every time
  unsigned char *reg = cpu->reg;
  unsigned char *ram = cpu->ram;

  int running = 1; // True until we get a HLT instruction
  int FL = 0;

  while (running)
  {
    int PC = cpu->PC;
    unsigned char IR = cpu_ram_read(cpu, PC); //instruction register
    unsigned char operandA = cpu_ram_read(cpu, PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, PC + 2);

    // True if this instruction might set the PC
    int instruction_set_pc = (IR >> 4) & 1;

    switch (IR)
    {

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

    case CMP:
      if (reg[operandA] < reg[operandB])
      {
        FL = 4;
      }
      else if (reg[operandA] > reg[operandB])
      {
        FL = 2;
      }
      else
      {
        FL = 1;
      }
      break;

    case JMP:
      cpu->PC = reg[operandA];
      break;

    case JEQ:
      if (FL == 1)
      {
        cpu->PC = reg[operandA];
        break;
      }
      cpu->PC += 2;
      break;

    case JNE:
      if (FL == 2 || FL == 4)
      {
        cpu->PC = reg[operandA];
        break;
      }
      cpu->PC += 2;
      break;

    case HLT:
      running = 0;
      break;

    case PRA:
      printf("%c\n", reg[operandA]);
      break;

    case CALL:
      reg[SP]--;
      cpu_ram_write(cpu, reg[SP], cpu->PC + 2);
      cpu->PC = reg[operandA];
      break;

    case RET:
      cpu->PC = cpu_ram_read(cpu, reg[SP]);
      reg[SP]++;
      break;

    case PUSH:
      reg[SP]--;
      cpu_ram_write(cpu, reg[SP], reg[operandA]);
      break;

    case POP:
      reg[operandA] = ram[reg[SP]];
      reg[SP]++;
      break;

    default:
      fprintf(stderr, "PC %02x: unknown instruction %02x\n", cpu->PC, IR);
      exit(3);
    }

    if (!instruction_set_pc)
    {
      cpu->PC += ((IR >> 6) & 0x3) + 1;
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

  // TODO: Zero registers and RAM
  memset(cpu->ram, 0, sizeof cpu->ram); // Clear out the ram
  memset(cpu->reg, 0, sizeof cpu->reg); // Clear out the registers

  // Initialize SP
  cpu->reg[SP] = ADDR_EMPTY_STACK;
}
