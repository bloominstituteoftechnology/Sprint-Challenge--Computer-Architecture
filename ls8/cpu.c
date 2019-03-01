#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *argv[])
{
  // TODO: Replace this with something less hard-coded
  FILE *fp = fopen(argv[1], "r");
  char *line[250];
  int index = 0;

  if (fp == NULL)
  {
    printf("Error opening specified file\n");
    exit(1);
  }

  while (fgets(line, sizeof(line), fp) != NULL)
  {
    char *endptr;
    unsigned long item;
    item = strtoul(line, &endptr, 2);
    if (!(*line == *endptr))
    {
      cpu_ram_write(cpu, index++, item);
    }
  }
}
unsigned char cpu_cmp(unsigned char numA, unsigned char numB)
{
  if (numA > numB)
  {
    return 2;
  }
  else if (numA < numB)
  {
    return 4;
  }
  else if (numA == numB)
  {
    return 1;
  }
  else
  {
    return 0;
  }
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
    cpu->reg[regA] *= cpu->reg[regB];
    break;
  // TODO: implement more ALU ops
  case ALU_ADD:
    cpu->reg[regA] += cpu->reg[regB];
    break;
  case ALU_CMP:
    cpu->FL += cpu_cmp(cpu->reg[regA], cpu->reg[regB]);
    break;
  }
}
unsigned char cpu_ram_read(struct cpu *cpu, int index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, int index, unsigned char thing)
{
  cpu->ram[index] = thing;
}

void cpu_push(struct cpu *cpu, unsigned char target)
{
  cpu->reg[7] -= 1;
  cpu_ram_write(cpu, cpu->reg[7], cpu->reg[target]);
}

void cpu_pop(struct cpu *cpu, unsigned char target)
{
  cpu->reg[target] = cpu->ram[cpu->reg[7]];
  cpu->reg[7] += 1;
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
    unsigned char instruction = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);
    // 4. switch() over it to decide on a course of action.
    switch (instruction)
    {
    case LDI:
      printf("LDI:\n");
      // 5. Do whatever the instruction should do according to the spec.
      cpu->reg[operandA] = operandB;
      
      // 6. Move the PC to the next instruction.
      break;
    case PRN:
      printf("PRN: ----- ");
      printf("%u", cpu->reg[operandA]);
      printf(" ----- \n");
      break;
    case HLT:
      running = 0;
      break;
    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;
    case PUSH:
      cpu_push(cpu, operandA);
      break;
    case POP:
      cpu_pop(cpu, operandA);
      break;

    case ADD:
      printf("Adding\n");
      alu(cpu, ALU_ADD, operandA, operandB);
      break;
    case CALL:
      cpu->reg[7] -= 1;
      cpu_ram_write(cpu, cpu->reg[7], cpu->PC + 2);
      cpu->PC = cpu->reg[operandA];
      printf("CALL PC: %d\n", cpu->PC);
      break;
    case RET:
      cpu->PC = cpu->ram[cpu->reg[7]];
      cpu->reg[7] += 1;

      break;
    case CMP:
      printf("CMP:\n");
      alu(cpu, ALU_CMP, operandA, operandB);
      break;

    case JMP:

      cpu->PC = cpu->reg[operandA];
      break;

    case JEQ:
      printf("JEQ:");
      printf("                        CPU->FL: %d    ", cpu->FL);
      if (cpu->FL == 1)
      {
        printf("FL == 1  JUMPS!!     ");
        printf("my cpu->PC %u, my cpu-FL %u, my cpu->reg[operandA] %u \n", cpu->PC, cpu->FL, cpu->reg[operandA]);

        cpu->PC = cpu->reg[operandA];
        cpu->PC += 2;
        cpu->FL = 0;
        break;
      }
      else
      {
        printf("FL != 1       Doesn't jump   \n");
        cpu->PC += (instruction >> 6 & 3) + 1;
        // cpu->PC += 2;
        cpu->FL = 0;
        break;
      }
      break;

    case JNE:
      printf("JNE:");
      printf("                        CPU->FL: %d    ", cpu->FL);
      if ((cpu->FL & 1) != 1)
      {
        printf("cpu->FL & 1 != 1  JUMPS!\n");
        cpu->PC = cpu->reg[operandA];
        cpu->FL = 0;
        break;
      }
      else
      {
        printf("cpu->FL & 1 == 1\n");
        cpu->PC += (instruction >> 6 & 3) + 1;
        //  cpu->PC += 2;
        cpu->FL = 0;
        break;
      }
      break;
    }

    if (!(instruction >> 4 & 1))
    {
      cpu->PC += (instruction >> 6 & 0x3) + 1;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special reg
  cpu->PC = 0;
  cpu->FL = 0b00000000;
  //void *memset(void *ptr, int x, size_t n);
  //initalize both ram and reg from cpu.h
  memset(cpu->reg, 0, 8 * sizeof(char));
  memset(cpu->ram, 0, 256 * sizeof(char));
  cpu->reg[7] = cpu->ram[0xf4];
}
