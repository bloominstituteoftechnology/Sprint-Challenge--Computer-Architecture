#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar)
{
  return cpu->ram[mar];
}

void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr)
{
  cpu->ram[mar] = mdr;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fp = fopen(filename, "r");
  char line[1024];
  unsigned char addr = 0x00;

  if (fp == NULL) {
    fprintf(stderr, "error opening file %s\n", filename);
    exit(2);
  }

  while (fgets(line, sizeof line, fp) != NULL) {

    char *endchar;
    unsigned char b = strtoul(line, &endchar, 2);

    if (endchar == line) {
      continue;
    }

    cpu_ram_write(cpu, addr++, b);
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
      cpu->reg[regA] *= cpu->reg[regB];
      break;
    // TODO: implement more ALU ops
    case ALU_CMP:
      if (cpu->reg[regA] < cpu->reg[regB]) {
        cpu->eq_fl = 4;
      } else if (cpu->reg[regA] > cpu->reg[regB]) {
        cpu->eq_fl = 2;
      } else {
        cpu->eq_fl = 1;
      }
      
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char IR, operandA, operandB;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu->PC);
    operandA = cpu_ram_read(cpu, cpu->PC+1);
    operandB = cpu_ram_read(cpu, cpu->PC+2);

    int add_to_pc = (IR >> 6) + 1;

    printf("TRACE: %02X: %02X %02X %02X\n", cpu->PC, IR, operandA, operandB);
    for (int i = 0; i < 8; i++) {
      printf("%02X", cpu->reg[i]);
    }
    printf("\n");

    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    switch(IR) {
      case LDI:
        cpu->reg[operandA] = operandB;
        break;
      
      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;

      case POP: 
        cpu->reg[operandA & 7] = cpu_ram_read(cpu, cpu->reg[7]);
        cpu->reg[7]++;
        break;

      case PUSH:
        cpu->reg[7]--;
        cpu_ram_write(cpu, cpu->reg[7], cpu->reg[operandA]);
        break;

      case JMP:
        cpu->PC = cpu->reg[operandA];
        add_to_pc = 0;
        break;

      case CMP:
        alu(cpu, ALU_CMP, operandA, operandB);
        break;

      case HLT:
        running = 0;
        break;

      case JEQ:
        if (cpu->eq_fl == 1) {
          cpu->PC = cpu->reg[operandA];
          add_to_pc = 0;
        }
        break;
      
      default: 
      printf("ERROR\n");
      exit(1);
    }

    // 4. Move the PC to the next instruction.
    cpu->PC += add_to_pc;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  cpu->eq_fl = 0;
  // TODO: Zero registers and RAM
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->reg, 0, sizeof cpu->reg);
}
