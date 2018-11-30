#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char* arg)
{

  // TODO: Replace this with something less hard-coded

  FILE *fp;
  char data[1024];
  int line = 0;
  char *address;
  fp = fopen(arg, "r");
  
  if (fp == NULL) {
    perror("Can not open the file\n");
    exit(1);
  }
  
  while(fgets(data, sizeof(data), fp) != NULL) {
      
      unsigned char byte = strtoul(data, &address, 2);
      
      if(address == data){
        continue;
      }
      cpu_ram_write(cpu, line++, byte);
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

    case ALU_CMP:
      if (reg[regA] < reg[regB]){
        cpu->LESS_THAN_FLAG = 1;
      } else {
        cpu->LESS_THAN_FLAG = 0;
      }
      if (reg[regA] > reg[regB]){
        cpu->GREATER_THAN_FLAG = 1;
      } else {
        cpu->GREATER_THAN_FLAG = 0;
      }
      if (reg[regA] == reg[regB]){
        cpu->EQUAL_FLAG = 1;
      } else {
        cpu->EQUAL_FLAG = 0;
      }
      break;

    case ALU_LD:
      reg[regA] = reg[regB];
      break;

    case ALU_OR:
      reg[regA] = reg[regA] | reg[regB];
      break;

    case ALU_AND:
      reg[regA] = reg[regA] & reg[regB];
      break;

    case ALU_XOR:
      reg[regA] = reg[regA] ^ reg[regB];
      break;

    case ALU_SHL:
      reg[regA] = reg[regA] << reg[regB];
      break;

    case ALU_SHR:
      reg[regA] = reg[regA] >> reg[regB];
      break;

    case ALU_ADDI:
      reg[regA] += regB;
      break;

    case ALU_LDI:
      reg[regA] = regB;
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
        alu(cpu, ALU_LDI, operandA, operandB);
        PC += shift;
        break;

      case LD:
        alu(cpu, ALU_LD, operandA, operandB);
        PC += shift;
        break;

      case PRN:
        printf("%d \n", reg[operandA]);
        PC += shift;
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        PC += shift;
        break;
      
      case SUB:
        alu(cpu, ALU_SUB, operandA, operandB);
        PC += shift;
        break;

      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        PC += shift;
        break;
      
      case DIV:
        alu(cpu, ALU_DIV, operandA, operandB);
        PC += shift;
        break;

      case MOD:
        alu(cpu, ALU_MOD, operandA, operandB);
        PC += shift;
        break;

      case POP:
        reg[operandA] = cpu_ram_read(cpu, reg[SP]++);
        PC += shift;
        break;

      case PUSH:
        cpu_ram_write(cpu, --reg[SP], reg[operandA]);
        PC += shift;
        break;

      case CALL:
        reg[SP] = reg[SP - 1];
        cpu_ram_write(cpu, reg[SP], PC + shift);
        PC = reg[operandA];
        break;
      
      case RET:
        PC = cpu_ram_read(cpu, reg[SP]++);
        shift = 0;
        break;

      case CMP:
        alu(cpu, ALU_CMP, operandA, operandB);
        PC += shift;
        break;

      case JMP:
        reg[SP] = reg[SP - 1];
        PC = reg[operandA];
        //printf("Jumped to %d:\n ", PC);
        break;

      case JNE:
        if (cpu->EQUAL_FLAG == 0) {
          PC = reg[operandA];
        } else {
          PC += shift;
        }
        break;

      case JEQ:
        if (cpu->EQUAL_FLAG == 1) {
          PC = reg[operandA];
        } else {
          PC += shift;
        }
        break;

      case OR:
        alu(cpu, ALU_OR, operandA, operandB);
        PC += shift;
        break;

      case AND:
        alu(cpu, ALU_AND, operandA, operandB);
        PC += shift;
        break;

      case NOT:
        reg[operandA] = ~reg[operandA];
        PC += shift;
        break;

      case XOR:
        alu(cpu, ALU_XOR, operandA, operandB);
        PC += shift;
        break;

      case SHL:
        alu(cpu, ALU_SHL, operandA, operandB);
        PC += shift;
        break;

      case SHR:
        alu(cpu, ALU_SHR, operandA, operandB);
        PC += shift;
        break;

      case ADDI:
        alu(cpu, ALU_ADDI, operandA, operandB);
        PC += shift;
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

  // TODO: Zero registers and RAM
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));

  cpu->PC = 0x00;
  cpu->reg[SP] = 0xF4;

  cpu->EQUAL_FLAG = 0x00;
  cpu->LESS_THAN_FLAG = 0x00;
  cpu->GREATER_THAN_FLAG = 0x00;
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index) {
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char value) {
  cpu->ram[index] = value;
}