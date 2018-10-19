#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6
#define SP 5

void stack_push(struct cpu *cpu, unsigned char val){
  cpu->reg[SP]--;
  cpu->ram[cpu->reg[SP]] = val;
}

unsigned char stack_pop(struct cpu *cpu){
  unsigned char value = cpu->ram[cpu->reg[SP]];
  cpu->reg[SP]++;
  return value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(char *filename, struct cpu *cpu)
{
  FILE *fp;
  char line[1024];
  unsigned char address = 0x00;

  fp = fopen(filename, "r");

  if(fp == NULL) {
    fprintf(stderr, "Error opening file %s\n", filename);
    exit(2);
  }

  while (fgets(line, sizeof line, fp) != NULL) {
		char *endptr = NULL;

    unsigned char b = strtoul(line, &endptr, 2);

		if (endptr == line) {
			printf("Ignoring this line.\n");
			continue;
		}

    cpu_ram_write(cpu, address++, b);
	}

	fclose(fp);



  // TODO: Replace this with something less hard-coded
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
      cpu->reg[regA] += cpu->reg[regB];
      break;
    case ALU_CMP:
      if(cpu->reg[regA] < cpu->reg[regB])
        cpu->FL += 0b100;
      if(cpu->reg[regA] > cpu->reg[regB])
        cpu->FL += 0b010;
      if(cpu->reg[regA] == cpu->reg[regB])
        cpu->FL += 0b001;
      else
        cpu->FL = 0;
      break;

    // TODO: implement more ALU ops
  }
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
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
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);

    int inst_len = (IR >> 6) + 1;
    // 2. switch() over it to decide on a course of action.
    switch(IR){
      // 3. Do whatever the instruction should do according to the spec.
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
        stack_push(cpu, cpu->reg[operandA]);
        break;
      case POP:
        cpu->reg[operandA] = cpu_pop(cpu);
        break;
      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        break;
      case CALL:
        stack_push(cpu, cpu->PC + 2);
        cpu->PC = cpu->reg[operandA];
        break;
      case RET:
        cpu->PC = cpu_pop(cpu);
        break;
      case CMP:
        alu(cpu, ALU_CMP, operandA, operandB);
        break;
      case JMP:
        cpu->PC = cpu->reg[operandA];
        break;
      case JEQ:
        break;
      case JNE:
        break;
      default:
        fprintf(stderr, "PC %02x: unknown instruction %02x\n", cpu->PC, IR);
        exit(3);
    }
    
    // 4. Move the PC to the next instruction.
    if(!inst_len) {
      cpu->PC += ((IR >> 6) & 0x3) + 1;
    }
    else
     cpu->PC += inst_len;

  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  cpu->FL = 0;

  // TODO: Zero registers and RAM
  memset(cpu->reg, 0, sizeof cpu->reg);
  memset(cpu->ram, 0, sizeof cpu->ram);

  cpu->reg[5] = 0xF4;
}

