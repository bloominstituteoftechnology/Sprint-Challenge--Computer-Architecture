#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

/**
 *  Reads from RAM
 */
char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

/**
 * Writes to RAM
 */
void cpu_ram_write(struct cpu *cpu, unsigned char value, unsigned char address)
{
  cpu->ram[address] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *arg)
{
  int address = 0;
  FILE *f = fopen(arg, "r");
  char str[256];

  if (f == NULL)
  {
    printf("File not found!\n");
    exit(1);
  }
  while (fgets(str, 256, f) != NULL)
  {
    char *endptr;
    unsigned char val = strtoul(str, &endptr, 2);
    if (endptr == str)
    {
      continue;
    }
    cpu_ram_write(cpu, val, address++);
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
    cpu->reg[regA] = cpu->reg[regA] * cpu->reg[regB];
    break;
  case ALU_ADD:
    cpu->reg[regA] = cpu->reg[regA] + cpu->reg[regB];
    break;
  }
}

/**
 * Perform LDI Instruction
 */
void ldi_instr(struct cpu *cpu)
{
  unsigned char operand_a = cpu_ram_read(cpu, cpu->pc + 1);
  unsigned char operand_b = cpu_ram_read(cpu, cpu->pc + 2);
  cpu->reg[operand_a] = operand_b;
  cpu->pc += 3;
}

/**
 * Perform PRN Instruction
 */
void prn_instr(struct cpu *cpu)
{
  unsigned char operand_a = cpu_ram_read(cpu, cpu->pc + 1);
  unsigned char print_value = cpu->reg[operand_a];

  printf("%u \n", print_value);

  cpu->pc += 2;
}

/**
 * Perform MUL Instruction
 */
void mul_instr(struct cpu *cpu)
{
  unsigned char operand_a = cpu_ram_read(cpu, cpu->pc + 1);
  unsigned char operand_b = cpu_ram_read(cpu, cpu->pc + 2);
  alu(cpu, ALU_MUL, operand_a, operand_b);
  cpu->pc += 3;
}

/**
 * Perform PUSH Instruction
 */
void push_instr(struct cpu *cpu)
{
  // Get the value at the next operand
  unsigned char operand_a = cpu_ram_read(cpu, cpu->pc + 1);
  // Decrement the available size of the stack
  cpu->reg[7]--;
  // In RAM, set the top stack value to the value in register[operand_a]
  cpu->ram[cpu->reg[7]] = cpu->reg[operand_a];
  // Increment the program counter
  cpu->pc += 2;
}

/**
 * Perform POP Instruction
 */
void pop_instr(struct cpu *cpu)
{
  // Get the value at the next operand
  unsigned char operand_a = cpu_ram_read(cpu, cpu->pc + 1);
  // Set the value at the specified register to the value at the top of the stack in RAM
  cpu->reg[operand_a] = cpu->ram[cpu->reg[7]];
  // Increase the size of the stack
  cpu->reg[7]++;
  // Increment the program counter
  cpu->pc += 2;
}

/**
 * Perform ADD Instruction
 */
void add_instr(struct cpu *cpu)
{
  unsigned char operand_a = cpu_ram_read(cpu, cpu->pc + 1);
  unsigned char operand_b = cpu_ram_read(cpu, cpu->pc + 2);
  alu(cpu, ALU_ADD, operand_a, operand_b);
  cpu->pc += 3;
}

/**
 * Perform CALL Instruction
 */
void call_instr(struct cpu *cpu)
{
  // Register in which contains our subroutine location
  unsigned char operand_a = cpu_ram_read(cpu, cpu->pc + 1);
  // Go into the register and grab the address for our subroutine
  unsigned char routine_loc = cpu->reg[operand_a];
  // Store the return location after subroutine completes
  unsigned char return_loc = cpu->pc + 2;
  // Decrement the count for the stack
  cpu->reg[7]--;
  // Add the return location to the stack
  cpu->ram[cpu->reg[7]] = return_loc;
  // Set the program counter equal to the location of the routine
  cpu->pc = routine_loc;
}

/**
 * Perform RET Instruction
 */
void ret_instr(struct cpu *cpu)
{
  // Set the PC to the location stored at the top of the Stack
  cpu->pc = cpu->ram[cpu->reg[7]];
  // Increase the size of the stack
  cpu->reg[7]++;
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running)
  {
    unsigned char c_instr = cpu_ram_read(cpu, cpu->pc);
    switch (c_instr)
    {
    case LDI:
      ldi_instr(cpu);
      break;
    case PRN:
      prn_instr(cpu);
      break;
    case MUL:
      mul_instr(cpu);
      break;
    case PUSH:
      push_instr(cpu);
      break;
    case POP:
      pop_instr(cpu);
      break;
    case ADD:
      add_instr(cpu);
      break;
    case CALL:
      call_instr(cpu);
      break;
    case RET:
      ret_instr(cpu);
      break;
    case HLT:
      running = 0;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->pc = 0;
  memset(cpu->ram, 0, 256 * sizeof(unsigned char));
  memset(cpu->reg, 0, 8 * sizeof(unsigned char));
}