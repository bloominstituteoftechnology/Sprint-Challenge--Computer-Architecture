#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address) {
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value) {
  cpu->ram[address] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *argv[])
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
    FILE *fp;
    char line[100];
    int address = 0;
    
    fp = fopen(argv[1], "r");
    if (fp == NULL) 
    {
      fprintf(stderr, "File pointer error");
      exit(1);
    }
    while(fgets(line, sizeof(line), fp)) 
    {
      char *endchar;
      unsigned long v;
      v = strtoul(line, &endchar, 2);
      if (!(*line == *endchar))
        cpu_ram_write(cpu, address++, v);
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
      // TODO
      cpu->registers[regA] *= cpu->registers[regB];
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
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC+2);
    // 2. switch() over it to decide on a course of action.
    switch(IR) {
      case LDI:
        cpu->registers[operandA] = operandB;
        cpu->PC += 3;
        break;

      case PRN:
        printf("%d \n", cpu->registers[operandA]);
        cpu->PC += 2;
        break;

      case HLT:
        running = 0;
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        cpu->PC += 3;
        break;

      case PUSH:
        cpu->registers[SP]--;
        cpu_ram_write(cpu, cpu->registers[SP], cpu->registers[operandA]);
        cpu->PC += 2;
        break;

      case POP:
        cpu->registers[operandA] = cpu->ram[cpu->registers[SP]];
        cpu->registers[SP]++;
        cpu->PC += 2;
        break;

      default:
        break;
    }
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

  // TODO: Zero registers and RAM
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->registers, 0, sizeof cpu->registers);
}
