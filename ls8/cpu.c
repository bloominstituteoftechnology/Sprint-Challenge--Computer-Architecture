#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#define DATA_LEN 6
// Helpers
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}
/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file)
{
  FILE *f;
  f = fopen(file, "r");

  int address = 0;

  char line[256];

  while (fgets(line, sizeof(line), f))
  {
    char *endpntr;
    unsigned long int new_line;
    new_line = strtoul(line, &endpntr, 2);
    if (endpntr != line)
    {
      cpu->ram[address++] = new_line;
    }
  }

  fclose(f);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    // TODO
    cpu->reg[regA] = cpu->reg[regA] * cpu->reg[regB];
    break;
  case ALU_ADD:
    cpu->reg[regA] = cpu->reg[regA] + cpu->reg[regB];
    break;
  case ALU_DIV:
    cpu->reg[regA] /= cpu->reg[regB];
    break;
  case ALU_SUB:
    cpu->reg[regA] -= cpu->reg[regB];
    break;
  case ALU_MOD:
    cpu->reg[regA] = cpu->reg[regA] % cpu->reg[regB];
    break;

    // TODO: implement more ALU ops
  }
}
unsigned char pop(struct cpu *cpu)
{
  unsigned char value = cpu->reg[7];
  cpu->reg[7]++;
  return value;
}
void push(struct cpu *cpu, unsigned char registers)
{
  cpu->reg[7]--;
  cpu_ram_write(cpu, cpu->reg[7], cpu->reg[registers]);
}
void compare(struct cpu *cpu, unsigned char regA, unsigned char regB)
{
  cpu->fl = 0x00;
  unsigned char valA = cpu->reg[regA];
  unsigned char valB = cpu->reg[regB];
  if (valA == valB)
  {
    cpu->fl = 1;
  }
  else if (valA > valB)
  {
    cpu->fl = 2;
  }
  else
  {
    cpu->fl = 4;
  }
}
void jump(struct cpu *cpu, unsigned char registers, unsigned char IR)
{
  cpu->pc = cpu->reg[registers] - ((IR >> 6) + 1);
}
/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    unsigned char IR = cpu_ram_read(cpu, cpu->pc);
    unsigned char operandA = cpu_ram_read(cpu, cpu->pc + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->pc + 2);

    //printf("TRACE: %02x: %02x\n", cpu->pc, IR);

    switch (IR)
    {

    case LDI:
      cpu->reg[operandA] = operandB;
      cpu->pc += 3;
      break;
    case PRN:
      printf("%d\n", cpu->reg[operandA]);
      cpu->pc += 2;
      break;
    case HLT:
      running = 0;
      break;
    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;
    case CALL:
      push(cpu, cpu->pc + 2);
      cpu->pc = cpu->reg[operandA];
      break;
    case RET:
      cpu->pc = pop(cpu);
      break;
    case POP:
      cpu->reg[operandA] = pop(cpu);
      break;
    case PUSH:
      push(cpu, operandA);
      break;
    case ADD:
      alu(cpu, ALU_ADD, operandA, operandB);
      break;
    case CMP:
      compare(cpu, operandA, operandB);
      break;
    case JMP:
      jump(cpu, operandA, IR);
      break;
    case JEQ:
      if (cpu->fl == 00000001)
      {
        jump(cpu, operandA, IR);
      }
      break;
    case JNE:
      if (cpu->fl != 00000001)
      {
        jump(cpu, operandA, IR);
      }
      break;
    default:
      printf("unknown instructions at %02x: %02x\n", cpu->pc, IR);
      exit(2);
    }
    // move to next instruction
    cpu->pc += (IR >> 6) + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;
  cpu->reg[7] = 0xF4;
  cpu->fl = 0x00;
  // TODO: Zero registers and RAM
}
