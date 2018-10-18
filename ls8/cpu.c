#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

void cpu_load(struct cpu *cpu, char *argv[])
{

  // TODO: Replace this with something less hard-coded
  FILE *fp = fopen(argv[1], "r");
  int address = 0;
  char read[100];

  if (fp == NULL)
  {
    fprintf(stderr, "File pointer has an issue, recheck it!\n");
    exit(1);
  }

  while (fgets(read, sizeof(read), fp))
  {
    unsigned long initial;
    char *endptr;
    initial = strtoul(read, &endptr, 2);
    if (!(*read == *endptr))
      cpu_ram_write(cpu, address++, initial);
  }
};



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
    case ALU_ADD:
      cpu->reg[regA] += cpu->reg[regB];
      break;
    default:
      printf("No instruction given\n");
      break;
      // TODO: implement more ALU ops
  }
}

void cpu_switch(struct cpu *cpu, unsigned char address)
{
  cpu->pc = cpu->reg[address];
}
/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; 

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    unsigned char IR = cpu_ram_read(cpu, cpu->pc);
    unsigned char operandA = cpu_ram_read(cpu, cpu->pc + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->pc + 2);

    int instruction_set_pc = (IR >> 4) & 1;
    switch (IR)
    {
    case CMP:
      if (cpu->reg[operandA] == cpu->reg[operandB])
        cpu->fl = 1;
      break;
    case JMP:
      cpu_switch(cpu, operandA);
      instruction_set_pc = 1;
      break;
    case JEQ:
      if (cpu->fl)
        cpu_switch(cpu, operandA);
      else
        cpu->pc += 2;
      break;
    case JNE:
      if (!cpu->fl)
        cpu_switch(cpu, operandA);
      else
        cpu->pc += 2;
      break;
    case ST:
      cpu_ram_write(cpu, cpu->reg[operandA], cpu->reg[operandB]);
      break;
    case ADD:
      alu(cpu, ALU_ADD, operandA, operandB);
      break;
    case CALL:
      cpu->fl--;
      cpu_ram_write(cpu, cpu->fl, (cpu->pc + 2));
      cpu_switch(cpu, operandA);
      break;
    case RET:
      cpu->pc = cpu_ram_read(cpu, cpu->fl++);
      break;
    case PUSH:
      cpu->fl--;
      cpu_ram_write(cpu, cpu->fl, cpu->reg[operandA]);
      break;
    case POP:
      if (cpu->fl == 244)
        fprintf(stderr, "Stack is empty, cant pop!\n");
      cpu->reg[operandA] = cpu_ram_read(cpu, cpu->fl++);
      break;
    case LDI:
      cpu->reg[operandA] = operandB;
      break;
    case HLT:
      running = 0;
      break;
    case PRN:
      printf("%d\n", cpu->reg[operandA]);
      break;
    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;
    default:
      printf("Unknown instruction at %02x: %02x\n", cpu->pc, IR);
      exit(2);
    }
    if (!instruction_set_pc)
    {
      cpu->pc += ((IR >> 6) & 0x3) + 1;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;
  cpu->fl = 0;
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));
  // TODO: Zero registers and RAM
}
