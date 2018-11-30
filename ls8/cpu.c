#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DATA_LEN 6

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
  //
  // int address = 0;
  //
  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }

  // TODO: Replace this with something less hard-coded
  FILE *fp = fopen(argv[1], "r");
  char *line[250];
  int index = 0;

  if (fp == NULL) {
    printf("Error opening specified file\n");
    exit(1);
  }

  while (fgets(line, sizeof(line), fp) != NULL) {
    char *endptr;
    unsigned long item;
    item = strtoul(line, &endptr, 2);
    if (!(*line == *endptr)) {
      cpu_ram_write(cpu, index++, item);
    }
  }
}

unsigned char cpu_cmp(unsigned char numA, unsigned char numB) {
  if (numA > numB) {
    return 2;
  } else if (numA < numB) {
    return 4;
  } else if (numA == numB) {
    return 1;
  } else {
    return 0;
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
    // TODO: implement more ALU ops
    case ALU_ADD:
      cpu->registers[regA] += cpu->registers[regB];
      break;
    case ALU_CMP:
      cpu->FL += cpu_cmp(cpu->registers[regA], cpu->registers[regB]);
      break;
    case ALU_AND:
      cpu->registers[regA] &= cpu->registers[regB];
      break;
    case ALU_OR:
      cpu->registers[regA] |= cpu->registers[regB];
      break;
    case ALU_XOR:
      cpu->registers[regA] ^= cpu->registers[regB];
      break;
    case ALU_NOT:
      cpu->registers[regA] = ~(cpu->registers[regA]);
      break;
    case ALU_SHL:
      cpu->registers[regA] <<= cpu->registers[regB];
      cpu->registers[regA] &= pow(2, cpu->registers[regB]);
      break;
    case ALU_SHR:
      cpu->registers[regA] >>= cpu->registers[regB];
      cpu->registers[regA] &= 256 - pow(2, cpu->registers[regB]);
      break;
    case ALU_MOD:
      cpu->registers[regA] %= cpu->registers[regB];
      break;
  }
}
unsigned char cpu_ram_read(struct cpu *cpu, int index) {
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, int index, unsigned char thing) {
  cpu->ram[index] = thing;
}

void cpu_push(struct cpu *cpu, unsigned char target) {
  cpu->registers[7] -= 1;
  cpu_ram_write(cpu, cpu->registers[7], cpu->registers[target]);
}

void cpu_pop(struct cpu *cpu, unsigned char target) {
  cpu->registers[target] = cpu->ram[cpu->registers[7]];
  cpu->registers[7] += 1;
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
    switch (IR) {
      case LDI:
      // 3. Do whatever the instruction should do according to the spec.
        cpu->registers[operandA] = operandB;
        // 4. Move the PC to the next instruction.
        // cpu->PC += 3;
        break;
      case PRN:
        printf("%u\n", cpu->registers[operandA]);
        // cpu->PC += 2;
        break;
      case HLT:
        // cpu->PC += 1;
        running = 0;
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        // cpu->PC+=3;
        break;
      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        break;
      case PUSH:
        cpu_push(cpu, operandA);
        // cpu->PC += 2;
        break;
      case POP:
        cpu_pop(cpu, operandA);
        // cpu->PC += 2;
        break;
      case CALL:
        cpu->registers[7] -= 1;
        cpu_ram_write(cpu, cpu->registers[7], cpu->PC+2);
        cpu->PC = cpu->registers[operandA];
        break;
      case RET:
        cpu->PC = cpu->ram[cpu->registers[7]];
        cpu->registers[7] += 1;
        break;
      case CMP:
        alu(cpu, ALU_CMP, operandA, operandB);
        break;
      case JMP:
        cpu->PC = cpu->registers[operandA];
        break;
      case JEQ:
        if (cpu->FL == 1) {
          cpu->PC = cpu->registers[operandA];
          cpu->FL = 0;
        } else {
          cpu->PC += (IR >> 6 & 3) + 1;
          cpu->FL = 0;
        }
        break;
      case JNE:
        if ((cpu->FL & 1) != 1) {
          cpu->PC = cpu->registers[operandA];
          cpu->FL = 0;
        } else {
          cpu->PC += (IR >> 6 & 3) + 1;
          cpu->FL = 0;
        }
        break;
      case SHL:
        alu(cpu, ALU_SHL, operandA, operandB);
        break;
      case SHR:
        alu(cpu, ALU_SHR, operandA, operandB);
        break;
      case MOD:
        if (cpu->registers[operandB] == 0) {
          printf("Error: Second number cannot be 0/blank.\n");
          running = 0;
        } else {
          alu(cpu, ALU_MOD, operandA, operandB);
        }
        break;
      case AND:
        alu(cpu, ALU_AND, operandA, operandB);
        break;
      case OR:
        alu(cpu, ALU_OR, operandA, operandB);
        break;
      case XOR:
        alu(cpu, ALU_XOR, operandA, operandB);
        break;
      case NOT:
        alu(cpu, ALU_NOT, operandA);
        break;
    }
    if ((IR >> 4 & 1) != 1) {
      cpu->PC += (IR >> 6 & 3) + 1;
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
  cpu->FL = 0b00000000;
  // TODO: Zero registers and RAM
  memset(cpu->registers, 0, 8*sizeof(char));
  memset(cpu->ram, 0, 256*sizeof(char));
  cpu->registers[7] = cpu->ram[0xf4];
}
