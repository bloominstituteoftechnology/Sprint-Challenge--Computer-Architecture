#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"


/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file)
{
  FILE * fp = fopen(file, "r");
  if (fp == NULL) {
    fprintf(stderr, "There was an error opening the file \n");
    exit(1);
  }

  char str[128];
  char instruction[8];
  int index = 0;
  char databuff[256];

  while (fgets(str, sizeof(str), fp) != NULL) 
  {
    if (sscanf(str, "%s", instruction) == 1) 
    {
      if (str[0] == '0' || str[0] == '1') 
      {
        databuff[index++] = strtoul(instruction, NULL, 2);
      }
    }
  }

  int address = 0;

  for (int i = 0; i < index; i++)
  {
    cpu->ram[address++] = databuff[i];
  }
};

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address) 
{
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char value) 
{
  cpu->ram[address] = value;
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->reg[regA] = cpu->reg[regA] * cpu->reg[regB];
      cpu->PC += 3;
      break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC+1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC+2)
    // 2. switch() over it to decide on a course of action.
    switch(IR)
    {
      case LDI:
        cpu->reg[operandA] = operandB;
        cpu->PC += 3;
        break;
      
      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        cpu->PC += 2;
        break;
      
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;

      case PUSH:
        SP--;
        cpu_ram_write(cpu, SP, cpu->reg[operandA]);
        cpu->PC += 2;
        break;

      case POP:
        cpu->reg[operandA] = cpu_ram_read(cpu, SP);
        SP++;
        cpu->PC += 2;
        break;
      
      case CMP:
        if (cpu->reg[operandA] == cpu->reg[operandB])
        {
          cpu->FL = 1;
        } 
        else if (cpu->reg[operandA] < cpu->reg[operandB])
        {
          cpu->FL = 4;
        } else {
          cpu->FL = 2;
        }
        cpu->PC += 3;
        break;
      
      case JMP:
        cpu->PC = cpu->reg[operandA];
        break;
      
      case JEQ:
        if (cpu->FL == 1) {
          cpu->PC = cpu->reg[operandA];
        } else {
          cpu->PC += 2;
        }
        break;
      
      case JNE:
        if(cpu->FL != 1) 
        {
          cpu->PC = cpu->reg[operandA];
        } else {
          cpu->PC += 2;
        }
        break;
      
      case HLT:
        running = 0;
        break;
      
      default:
        printf("Uknown Instruction at %02x: %02x\n", cpu->PC, IR);
        exit(2);
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
  cpu->reg[7] = 0fx4;
  cpu->FL = 0;
}
