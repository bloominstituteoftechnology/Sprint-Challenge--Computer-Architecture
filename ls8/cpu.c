#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char* filename)
{
  FILE * fp = fopen(filename, "r");
  if (fp == NULL)
  {
    printf("fopen failed to open the file\n");
    exit(-1);
  }

  char line[128];
  char instruction[8];
  int index = 0;
  char databuff[256];

  while(fgets(line, sizeof(line), fp) != NULL)
  {
    if(sscanf(line, "%s", instruction) == 1)
    {
      if ( line[0] == '0' || line[0] == '1' ){
        databuff[index++] = strtoul(instruction, NULL, 2);
      }
    }
  }

  int address = 0;

  for (int i = 0; i < index; i++) {
    cpu->ram[address++] = databuff[i];
  }
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char data)
{
  cpu->ram[index] = data;
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB];
      break;

    case ALU_ADD:
      cpu->registers[regA] = cpu->registers[regA] + cpu->registers[regB];
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
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);
    int autoset = 1;
    
    // 2. switch() over it to decide on a course of action.
    switch(IR)
    {
      case LDI:
        cpu->registers[operandA] = operandB;
        break;

      case PRN:
        printf("%d\n", cpu->registers[operandA]);
        break;

      case HLT:
        running = 0;
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;

      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        break;

      case PUSH:
        cpu->registers[7] -= 1;
        cpu->ram[cpu->registers[7]] = cpu->registers[operandA];
        break;

      case POP:
        cpu->registers[operandA] = cpu->ram[cpu->registers[7]];
        cpu->registers[7] += 1; //initially decrement
        break;

      case CALL:
        cpu->registers[7] -= 1;
        cpu->ram[cpu->registers[7]] = cpu->PC+2;
        cpu->PC = cpu->registers[operandA];
        autoset = 0;
        break;

      case RET:
        cpu->PC = cpu->ram[cpu->registers[7]];
        cpu->registers[7] += 1; //same as above
        autoset = 0;
        break;

      case ST:
        cpu->ram[cpu->registers[operandA]] = cpu->registers[operandB];
        break;

      case CMP:
        if (cpu->registers[operandA] < cpu->registers[operandB]) cpu->FL = 4;
        else if (cpu->registers[operandA] > cpu->registers[operandB]) cpu->FL = 2;
        else cpu->FL = 1;
        break;

      default:
        running = 0;
        break;
    }

    if (autoset == 1) cpu->PC += (IR >> 6) + 1;
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.

  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  cpu->FL = 0;
  cpu->registers[7] = 0xF4;

  // TODO: Zero registers and RAM
}
