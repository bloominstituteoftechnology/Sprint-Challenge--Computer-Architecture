#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

void cpu_load(struct cpu *cpu, char *argv[])
{
  FILE *fp;
  char file_data[1024];
  int address = 0;
  // printf("argv[1]: %s\n", argv[1]);
  fp = fopen(argv[1], "r");

  if (fp == NULL)
  {
    fprintf(stderr, "file not found\n");
    exit(1);
  }

  while (fgets(file_data, sizeof file_data, fp) != NULL)
  {
    char *endptr;
    unsigned char line_value = strtol(file_data, &endptr, 2);
    if (endptr == file_data)
    {
      continue;
    }
    cpu_ram_write(cpu, address, line_value);
    address++;
  }
  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    cpu->registers[regA] *= cpu->registers[regB];
    break;
  case ALU_ADD:
    cpu->registers[regA] += cpu->registers[regB];
    break;
    // TODO: implement more ALU ops
  }
}

unsigned char pop(struct cpu *cpu)
{
  unsigned char pop_value = cpu->ram[cpu->registers[cpu->sp]];
  cpu->registers[cpu->sp]++;
  return pop_value;
}

unsigned char push(struct cpu *cpu, unsigned char value)
{
  cpu->registers[cpu->sp]--;
  cpu->ram[cpu->registers[cpu->sp]] = value;
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
    // pc = cpu->pc;
    // value = cpu->ram[pc];
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.

    unsigned char current_value = cpu_ram_read(cpu, cpu->pc);
    unsigned char index_value_1 = cpu_ram_read(cpu, cpu->pc + 1);
    unsigned char index_value_2 = cpu_ram_read(cpu, cpu->pc + 2);
    int inc_pc = (current_value >> 6) + 1;

    switch (current_value)
    {
    case LDI:
      cpu->registers[index_value_1] = index_value_2;
      break;
    case PRN:
      printf("%d \n", cpu->registers[index_value_1]);
      break;
    case HLT:
      running = 0;
      break;
    case MUL:
      alu(cpu, ALU_MUL, index_value_1, index_value_2);
      break;
    case ADD:
      alu(cpu, ALU_ADD, index_value_1, index_value_2);
      break;
    case POP:
      cpu->registers[index_value_1] = pop(cpu);
      break;
    case PUSH:
      push(cpu, cpu->registers[index_value_1]);
      break;
    case CALL:
      push(cpu, cpu->pc + 2);
      cpu->pc = cpu->registers[index_value_1];
      inc_pc = 0;
      break;
    case RET:
      cpu->pc = pop(cpu);
      inc_pc = 0;
      break;
      // sprint
    case CMP:
      if (cpu->registers[index_value_1] == cpu->registers[index_value_2])
      {
        cpu->fl = 1;
      }
      else if (cpu->registers[index_value_1] > cpu->registers[index_value_2])
      {
        cpu->fl = 2;
      }
      else if (cpu->registers[index_value_1] < cpu->registers[index_value_2])
      {
        cpu->fl = 4;
      }
      break;
    case JMP:
      cpu->pc = cpu->registers[index_value_1];
      inc_pc = 0;
      break;
    case JEQ:
      if (cpu->fl == 1)
      {
        cpu->pc = cpu->registers[index_value_1];
        inc_pc = 0;
      }
      break;
    case JNE:
      if (cpu->fl == 2 || cpu->fl == 4)
      {
        cpu->pc = cpu->registers[index_value_1];
        inc_pc = 0;
      }
      break;
      // --------------
    default:
      printf("Unknown instruction %02x at address %02x\n", cpu->ram[current_value], current_value);
      exit(1);
    }
    cpu->pc += inc_pc;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the pc and other special registers => zero ram and registers
  cpu->pc = 0;
  cpu->sp = 7;
  cpu->registers[cpu->sp] = 0xF4;
  // sprint
  cpu->fl = 0;
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->registers, 0, sizeof cpu->registers);
}
