#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

// #define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char* file)
{
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };

  // int address = 0;

  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }

  // TODO: Replace this with something less hard-coded
  FILE *fp = fopen(file,"r");
  if (fp == NULL)
  {
    printf("ERROR: Could not open file.");
  }
  int address = 0;
  char string [256];
  while (fgets(string, sizeof(string),fp) != NULL)
  {
    unsigned char data = strtol(string, NULL, 2);
    cpu->ram[address++] = data;
  }
  fclose(fp);
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char towrite)
{
  cpu->ram[address] = towrite;
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

    case ALU_CMP:
      if(cpu->registers[regA] == cpu->registers[regB])
      {
        cpu->FL = 0b00000001;
      }
      else if (cpu->registers[regA] > cpu->registers[regB])
      {
        cpu->FL = 0b00000010;
      }
      else if(cpu->registers[regA] < cpu->registers[regB])
      {
        cpu->FL = 0b00000100;
      }
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
    unsigned char IR, operandA, operandB;
    IR = cpu_ram_read(cpu, cpu->PC);
    operandA = cpu_ram_read(cpu, cpu->PC+1);
    operandB = cpu_ram_read(cpu, cpu->PC+2);

    switch (IR)
    {
      case HLT:
        running = 0;
        break;
      case LDI:
        cpu->registers[operandA] = operandB;
        // printf("LDI: %d at reg %d\n",cpu->registers[operandA],operandA);
        break;
      case PRN:
        printf("%d\n", cpu->registers[operandA]);
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;
      case PUSH:
        cpu->registers[7]--;
        cpu->ram[cpu->registers[7]] = cpu->registers[operandA];
        break;
      case POP:
        cpu->registers[operandA] = cpu->ram[cpu->registers[7]];
        cpu->registers[7]++;
        break;
      case CMP:
        alu(cpu, ALU_CMP, operandA, operandB);
        break;
      case JEQ:
        if (cpu->FL == 0b00000001)
        {
          cpu->PC += operandA + (IR >> 6) + 1;
        }
        cpu->FL = 0b00000000;
        break;
      case JNE:
        if (cpu->FL != 0b00000001)
        {
          cpu->PC += operandA + (IR >> 6) + 1;
        }
        cpu->FL = 0b00000000;
        break;
      case JMP:
        cpu->PC += operandA;
        break;
    }
    cpu->PC += (IR >> 6) + 1;
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;  
  cpu->registers = calloc(8, sizeof(unsigned char));
  cpu->ram = calloc(256, sizeof(unsigned char)); 
  cpu->registers[7] = 0xF4;
  cpu->FL = 0b00000000;
}
