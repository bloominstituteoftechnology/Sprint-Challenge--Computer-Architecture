#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/**
 * CPU Read
 */
char cpu_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

/**
 * CPU Write
 */
void cpu_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  char line[1000];
  int address = 0;
  
  FILE *fp = fopen(filename, "r");
  
  if (fp == NULL) {
    fprintf(stderr, "file not opened successfully\n");
    exit(2);
  }
  
  
  while(fgets(line, sizeof(line), fp) != NULL){
    char *endchar;
    unsigned char value = strtoul(line, &endchar, 2);

    if (line == endchar) {
      continue;
    }

    cpu_write(cpu, address++, value);
  }
  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->reg[regA] *= cpu->reg[regB];
      break;
    case ALU_ADD:
      break;
    case ALU_CMP:
      
      if (cpu->reg[regA] - cpu->reg[regB] > 0) {
        cpu->fl = 4;
      } else if (cpu->reg[regA] - cpu->reg[regB] < 0) {
        cpu->fl = 2;
      } else {
        cpu->fl = 1;
      }
      
      break;

    // TODO: implement more ALU ops
  }
}

void cpu_jump(struct cpu *cpu, unsigned char address)
{
  cpu->pc = cpu->reg[address];
}


/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC). //
    unsigned char IR = cpu_read(cpu, cpu->pc);
    unsigned char operandA = cpu_read(cpu, cpu->pc+1);
    unsigned char operandB = cpu_read(cpu, cpu->pc+2);
    // 2. switch() over it to decide on a course of action.

    switch (IR)
    {
      case LDI:
        cpu->reg[operandA] = operandB;
        break;

      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        break;

      case HLT:
        running = 0;
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;

      case PUSH:
        cpu->reg[SP]--;
        cpu_write(cpu, cpu->reg[SP], cpu->reg[operandA]);
        break;
        
      case POP:
        cpu->reg[operandA] = cpu_read(cpu, cpu->reg[SP]);
        cpu->reg[SP]++;
        break;

      case CMP:
        alu(cpu, ALU_CMP, operandA, operandB);
        cpu->pc += 2;
        break;

      case JMP:
        cpu_jump(cpu, operandA);
        break;

      case JEQ:
        if(cpu->fl == 1) {
          cpu_jump(cpu, operandA);
        }
        cpu->pc++;

        break;

      case JNE:
        if(cpu->fl == 0) {
          cpu_jump(cpu, operandA);
        }
        cpu->pc++;

        break;

      default:
        printf("unknown instruction at %02x: %02x\n", cpu->pc, IR);
        exit(2);
    }
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    cpu->pc += (IR >> 6) + 1;

  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;
  cpu->reg[SP] = 0xf4;
  cpu->fl = 00000000;
  // TODO: Zero registers and RAM
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));
  
}
