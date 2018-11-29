#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *argv[])
{

  // TODO: Replace this with something less hard-coded

  FILE *fp;
  char data[1024];
  unsigned char address = 0;
  fp = fopen(argv[1], "r");
  
  if (fp == NULL) {
    perror("Can not open the file\n");
    exit(1);
  }
  
  while(fgets(data, sizeof(data), fp) != NULL) {
      
      unsigned char byte = strtoul(data, NULL, 2);
      
      if(data == NULL){
        continue;
      }
      cpu->ram[address++] = byte;
    }
    fclose(fp);
  }

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *reg = cpu->reg;

  switch (op) {
    case ALU_MUL:
      // TODO
      reg[regA] *= reg[regB];
      break;

    // TODO: implement more ALU ops
    case ALU_SUB:
      reg[regA] -= reg[regB];
      break;

    case ALU_ADD:
      reg[regA] += reg[regB];
      break;

    case ALU_DIV:
      reg[regA] /= reg[regB];
      break;

    case ALU_MOD:
      reg[regA] = reg[regA]%reg[regB];
      break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  unsigned char *reg = cpu->reg;
  unsigned char PC = cpu->PC;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, PC);
    unsigned char operandA = cpu_ram_read(cpu, (PC + 1));
    unsigned char operandB = cpu_ram_read(cpu, (PC + 2));
    int shift = ((IR >> 6)) + 1;

    // 2. switch() over it to decide on a course of action.
    switch (IR) {
      
      case HLT:
        running = 0;
        break;

      case LDI:
        reg[operandA] = operandB;
        break;

      case PRN:
        printf("%d \n", reg[operandA]);
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;
      
      case SUB:
        alu(cpu, ALU_SUB, operandA, operandB);
        break;

      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        break;
      
      case DIV:
        alu(cpu, ALU_DIV, operandA, operandB);
        break;

      case MOD:
        alu(cpu, ALU_MOD, operandA, operandB);
        break;

      case POP:
        reg[operandA] = cpu_ram_read(cpu, reg[SP]++);
        break;

      case PUSH:
        cpu_ram_write(cpu, --reg[SP], reg[operandA]);
        break;

      case CALL:
        reg[SP] = reg[SP - 1];
        cpu_ram_write(cpu, reg[SP], PC + 2);
        PC = reg[operandA];
        shift = 0;
        break;
      
      case RET:
        PC = cpu_ram_read(cpu, reg[SP]++);
        shift = 0;
        break;

      /* default:
        printf("Unrecognized instruction %02x: %02x\n", PC, IR);
        exit(2); */
    }
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    PC += shift;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers

  // TODO: Zero registers and RAM
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));

  cpu->PC = 0x00;
  cpu->reg[SP] = 0xF4;

}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index) {
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char value) {
  cpu->ram[index] = value;
}

