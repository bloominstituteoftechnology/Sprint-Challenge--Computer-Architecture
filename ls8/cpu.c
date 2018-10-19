#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  return cpu->ram[index];
}
void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char value)
{
  cpu->ram[index] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file)
{
  FILE *fp = fopen(file, "r");
  char line[1024];
  int address = 0;

  if (fp == NULL) {
    printf("\033[0;31mERROR\033[0m The file you are looking for doesn't exist\n");
    exit(2);
  }
  while (fgets(line, sizeof(line), fp) != NULL) {
    if (line[0] == '\n' || line[0] == '#') {
      continue;
    }
    cpu_ram_write(cpu, address++, strtoul(line, NULL, 2));
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
      cpu->registers[regA] *= cpu->registers[regB];
      break;
    case ALU_ADD:
      cpu->registers[regA] += cpu->registers[regB];
      break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    cpu->IR = cpu_ram_read(cpu, cpu->PC);
    int argc = cpu->IR >> 6;
    unsigned char argv[2];
    for (int i=0; i < argc; i++) {
      argv[i] = cpu_ram_read(cpu, cpu->PC + i + 1);
    }

    switch(cpu->IR) {
      case LDI:
        cpu->registers[argv[0]] = argv[1];
        break;
      case PRN:
        printf("%d\n", cpu->registers[argv[0]]);
        break;      
      case HLT:
        printf("Halting...\n");
        running = 0;
        break;      
      case MUL:
        alu(cpu, ALU_MUL, argv[0], argv[1]);
        break;      
      case ADD:
        alu(cpu, ALU_ADD, argv[0], argv[1]);
        break;      
      case PUSH:
        cpu_ram_write(cpu, --cpu->registers[7], cpu->registers[argv[0]]);
        break;      
      case POP:
        cpu->registers[argv[0]] = cpu_ram_read(cpu, cpu->registers[7]++);
        break;      
      case CALL:
        cpu_ram_write(cpu, --cpu->registers[7], cpu->PC + 2);
        cpu->PC = cpu->registers[argv[0]];
        break;      
      case RET:
        cpu->PC = cpu_ram_read(cpu, cpu->registers[7]++);
        break;
      case CMP:
        if (cpu->registers[argv[0]] < cpu->registers[argv[1]]) {
          cpu->FL = 0b00000100;
        } else if (cpu->registers[argv[0]] > cpu->registers[argv[1]]) {
          cpu->FL = 0b00000010;
        } else if (cpu->registers[argv[0]] == cpu->registers[argv[1]]) {
          cpu->FL = 0b00000001;
        }
        break;
      case JMP:
        cpu->PC = cpu->registers[argv[0]];
        break;
      case JEQ:
        if ((cpu->FL & 1) == 1) {
          cpu->PC = cpu->registers[argv[0]];
        } else {
          cpu->PC += argc + 1;
        }
        break;
      case JNE:
        if ((cpu->FL & 1) == 0) {
          cpu->PC = cpu->registers[argv[0]];
        } else {
          cpu->PC += argc + 1;
        }
        break;
      default:
        printf("Unknown instructions: %d\n", cpu->IR);
        exit(3);
    }

    int changed = cpu->IR >> 4 & 1;
    if (!changed) {
      cpu->PC += 1 + argc;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  cpu->IR = 0;
  cpu->FL = 0;

  for (int i=0; i<8; i++) {
    cpu->registers[i] = 0;
  }
  for (int i=0; i<256; i++) {
    cpu->ram[i] = 0;
  }
  cpu->registers[7] = 0xf4;
}
