#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"


void cpu_push(struct cpu *cpu, unsigned char value) {
  cpu->registers[SP]--;
  cpu->RAM[cpu->registers[SP]] = value;
}

unsigned char cpu_pop(struct cpu *cpu) {
  unsigned char value = cpu->RAM[cpu->registers[SP]];
  cpu->registers[SP]++;
  return value;
}

void cpu_load(char *filename, struct cpu *cpu) {
  FILE *fp;
  char line[1024];
  int address = ADDR_PROGRAM_ENTRY;

  if ((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Can't open file %s\n.", filename);
    exit(2);
  }

  while (fgets(line, sizeof line, fp) != NULL) {
    // turn string into number
    char *endchar;
    unsigned char byte = strtol(line, &endchar, 2);

    if (endchar == line) {
      continue;
    }

    // store the value in RAM
    cpu->RAM[address++] = byte;
  }
}

void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB) {
  unsigned char *registers = cpu->registers;
  unsigned char valB = registers[regB];

  switch (op) {
    case ALU_MUL:
        registers[regA] *= valB;
        break;
    case ALU_ADD:
        registers[regA] += valB;
        break;
  }
}

void cpu_run(struct cpu *cpu) {

  int running = 1; // C's version of a boolean 1 is true, 0 is false

  while (running) {
    unsigned char IR = cpu->RAM[cpu->PC];

    unsigned char operandA = cpu->RAM[(cpu->PC + 1) & 0xff];
    unsigned char operandB = cpu->RAM[(cpu->PC + 2) & 0xff];

    int instruction_set_pc = (IR >> 4) & 1;

    switch (IR) {
      
      case LDI:
          cpu->registers[operandA] = operandB;
          break;
      case PRN:
          printf("%d\n", cpu->registers[operandA]);
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
      case PRA:
          printf("%c\n", cpu->registers[operandA]);
          break;
      case CALL:
          cpu_push(cpu, cpu->PC + 2);
          cpu->PC = cpu->registers[operandA];
          break;
      case RET:
          cpu->PC = cpu_pop(cpu);
          break;
      case PUSH:
          cpu_push(cpu, cpu->registers[operandA]);
          break;
      case POP:
          cpu->registers[operandA] = cpu_pop(cpu);
          break;
      case CMP:
          if (cpu->registers[operandA] == cpu->registers[operandB]) {
          cpu->FL += 1; 
        } else if (cpu->registers[operandA] > cpu->registers[operandB]) {
          cpu->FL += 1;
        } else if (cpu->registers[operandA] < cpu->registers[operandB]) {
         cpu->FL += 0;
        }
        break;
      case JMP:
          cpu->PC = cpu->registers[operandA];
      case JNE:
          if (cpu->FL == 0) {
          cpu->PC = cpu->registers[operandA];
        }
        break;
      case JEQ:
          if (cpu->FL == 1) {
          cpu->PC = cpu->registers[operandA];
        } 
        break;
      default:
          fprintf(stderr, "PC %0sx: unknown instruction %02x\n", cpu->PC, IR);
          exit(3);
    }
    if (!instruction_set_pc) {
      cpu->PC += ((IR >> 6) & 0x3) + 1;
    }
  }
}


void cpu_init(struct cpu *cpu) {
  cpu->PC = 0;

  memset(cpu->registers, 0, sizeof cpu->registers);
  memset(cpu->RAM, 0, sizeof cpu->RAM);

  // initialize SP (stack pointer)
  cpu->registers[SP] = ADDR_EMPTY_STACK;
}