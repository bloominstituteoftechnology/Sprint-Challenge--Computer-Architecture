#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define DATA_LEN 1024;

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char MAR)//MAR  memory address register,holds the memory address we're reading or writing
{
  return cpu->RAM[MAR];
}

void cpu_ram_write(struct cpu *cpu, unsigned char MAR, unsigned char MDR)//MDR Memory Data Register, holds the value to write or the value just read
{
  cpu->RAM[MAR] = MDR;
}
/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
// void cpu_load(struct cpu *cpu)
// {
//   char data[DATA_LEN] = {
//     // From print8.ls8
//     0b10000010, // LDI R0,8
//     0b00000000,
//     0b00001000,
//     0b01000111, // PRN R0
//     0b00000000,
//     0b00000001  // HLT
//   };

//   int address = 0;

//   for (int i = 0; i < DATA_LEN; i++) {
//     cpu->ram[address++] = data[i];
//   }

//   // TODO: Replace this with something less hard-coded
// }

void cpu_load(struct cpu *cpu, char *argv[])
{
  char line[1024];
  int address = 0;
  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL)
  {
    fprintf(stderr, "File pointer is not working");
    exit(1);
  }
  while (fgets(line, sizeof(line), fp))
  {
    char *endchar;
    unsigned long v;
    v = strtoul(line, &endchar, 2);
    if (!(*line == *endchar))
      cpu_ram_write(cpu, address++, v);
  }
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      cpu->registers[regA] *= cpu->registers[regB];
      break;
    case ALU_ADD:
      cpu->registers[regA] += cpu->registers[regB];
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
  int FL = 0;
  while (running) {
  
    // TODO
    int PC = cpu->PC;
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);
    // 2. switch() over it to decide on a course of action.

 //printf("TRACE: %02X:  %02X  %02X  %02X\n", cpu->PC, IR, operandA, operandB);
   
    int instruction_set_pc = (IR >> 4) & 1;

    switch (IR)
    {
case LDI:
      cpu->registers[operandA] = operandB;
      break;

    case PRN:
      printf("\nValue at register %d: %d\n", operandA, cpu->registers[operandA]);
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

    case CALL:
      cpu->registers[SP]--;
      cpu_ram_write(cpu, cpu->registers[SP], cpu->PC + 2);
      cpu->PC = cpu->registers[operandA];
      break;

    case RET:
      cpu->PC = cpu_ram_read(cpu, cpu->registers[SP]);
      cpu->registers[SP]++;
      break;

    case PUSH:
      cpu->registers[SP]--;
      cpu_ram_write(cpu, cpu->registers[SP], cpu->registers[operandA]);
      break;

    case POP:
      cpu->registers[operandA] = cpu->RAM[cpu->registers[SP]];
      cpu->registers[SP]++;
      break;

      case CMP:
      if (cpu->registers[operandA] < cpu->registers[operandB])
      {
       printf("CMP: %d < %d\n", cpu->registers[operandA], cpu->registers[operandB]);
        FL = 4;
      }
      else if (cpu->registers[operandA] > cpu->registers[operandB])
      {
        printf("CMP: %d > %d\n", cpu->registers[operandA], cpu->registers[operandB]);
        FL = 2;
      }
      else
      {
        printf("CMP: %d = %d\n", cpu->registers[operandA], cpu->registers[operandB]);
        FL = 1;
      }
      break;

    case JMP:
      cpu->PC = cpu->registers[operandA];
      break;

    case JEQ:
      if (FL == 1)
      {
        printf("JEQ: Jumping to %02X\n", cpu->registers[operandA]);
        cpu->PC = cpu->registers[operandA];
      }
      else
      {
        //printf("JEQ: Not Jumping\n");
        cpu->PC += 2;
      }

      break;

    case JNE:
      if (FL == 2 || FL == 4)
      {
       printf("JNE: Jumping to %02X\n", cpu->registers[operandA]);
        cpu->PC = cpu->registers[operandA];
      }
      else
      {
        printf("JNE: Not Jumping\n");
        cpu->PC += 2;
      }
      break;



       default:
       fprintf(stderr, "PC %02x: unknown instruction %02x\n", cpu->PC, IR);
       exit(3);
     }
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
     if (!instruction_set_pc && IR != JMP && IR != JEQ && IR != JNE)
    {
      cpu->PC += (IR >> 6) + 1;
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
  memset(cpu->registers, 0, sizeof cpu->registers);
  memset(cpu->RAM, 0, sizeof cpu->RAM);

  
   // Initialize SP
  cpu->registers[SP] = EMPTY_STACK;
}
