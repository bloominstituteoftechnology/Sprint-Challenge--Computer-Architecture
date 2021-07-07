#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cpu.h"

#define SP 7

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address) {
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value) {
  cpu->ram[address] = value;
}

void cpu_load(struct cpu *cpu, char *argv[])
{
  FILE *fp;
  char data[1024];
  unsigned char address = 0;
  fp = fopen(argv[1], "r");

  if(!fp) {
    printf("Error opening file \n");
    exit(1);
  }
  
  while(fgets(data, sizeof(data), fp) != NULL) {
    char *endptr;
    unsigned char ret = strtol(data, &endptr, 2);
    if (endptr == data) {
      continue;
    }
    cpu->ram[address++] = ret;
  }

  fclose(fp);
}

void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
      case ALU_MUL:
      cpu->reg[regA] *= cpu->reg[regB];
      break;
    case ALU_ADD:
      cpu->reg[regA] += cpu->reg[regB];
      break;
    case ALU_AND:
      cpu->reg[regA] = cpu->reg[regA] & cpu->reg[regB];
      break;
    case ALU_OR:
      cpu->reg[regA] = cpu->reg[regA] | cpu->reg[regB];
      break;
    case ALU_XOR:
      cpu->reg[regA] = cpu->reg[regA] ^ cpu->reg[regB];
      break;
    case ALU_NOT:
      cpu->reg[regA] = ~cpu->reg[regA];
      break;
    case ALU_SHL:
      break;
    case ALU_SHR:
      break;
    case ALU_MOD:
      break;
    case ALU_CMP:
      if (cpu->reg[regA] == cpu->reg[regB]) {
        cpu->FL = 0b00000001;
      }
      else {
        cpu->FL = 0b00000000;
      }
      // if (cpu->reg[regA] == cpu->reg[regB]) {
      //   cpu->FL = (cpu->FL & ~7) | 1; // 0b00000001; // cpu->FL | (1 << 0)
      // }
      // else if (cpu->reg[regA] > cpu->reg[regB]) {
      //   cpu->FL = (cpu->FL & ~7) | 2; // 0b00000010; //cpu->FL | (1 << 1)
      // }
      // else {
      //   cpu->FL = (cpu->FL & ~7) | 4; // 0b00000100; // cpu->FL | (1 << 2)
      // }
      break;

    default:
      break;
  }
}

unsigned char cpu_pop(struct cpu *cpu) {
  unsigned char ret = cpu->ram[cpu->reg[SP]];
  cpu->reg[SP]++;
  return ret;
}

void cpu_push(struct cpu *cpu, unsigned char val) {
  cpu->reg[SP]--;
  cpu->ram[cpu->reg[SP]] = val;
}

void cpu_run(struct cpu *cpu)
{
  int running = 1;

  while (running) {
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC+1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC+2);
    int num_operands = (IR>>6) + 1;
    
    switch(IR){
      case LDI:
        cpu->reg[operandA] = operandB;
        break;
      case PRN:
        printf("%d \n", cpu->reg[operandA]);
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;
      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        break;
      case CMP:
        alu(cpu, ALU_CMP, operandA, operandB);
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
        alu(cpu, ALU_NOT, operandA, operandB);
        break;
      case SHL:
        alu(cpu, ALU_SHL, operandA, operandB);
        break;
      case SHR:
        alu(cpu, ALU_SHR, operandA, operandB);
        break;
      case MOD:
        alu(cpu, ALU_MOD, operandA, operandB);
        break;
      case PUSH:
        cpu_push(cpu, cpu->reg[operandA]);
        break;
      case POP:
        cpu->reg[operandA] = cpu_pop(cpu);
        break;
      case CALL:
        cpu_push(cpu, cpu->PC+2);
        cpu->PC = cpu->reg[operandA];
        num_operands = 0;
        break;
      case RET:
        cpu->PC = cpu_pop(cpu);
        num_operands = 0;
        break;
      case JMP:
        cpu->PC = cpu->reg[operandA];
        num_operands = 0;
        break;
      case JEQ:
        if (cpu->FL == 0b00000001) {
          cpu->PC = cpu->reg[operandA];
          num_operands = 0;
        }
        break;
      case JNE:
        if (cpu->FL == 0b00000000) {
          cpu->PC = cpu->reg[operandA];
          num_operands = 0;
        }
        break;
      case HLT:
        running = 0;
        break;
    }
    cpu->PC += num_operands;
  }
}

void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  cpu->FL = 0x00;
  cpu->reg[SP] = 0xF4;
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->reg, 0, sizeof cpu->reg);
}
