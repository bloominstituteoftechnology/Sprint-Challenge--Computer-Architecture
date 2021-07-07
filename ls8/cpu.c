#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6
#define SP 7

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  int address = 0;

  FILE *file = fopen(filename, "r");

  char *data[64];

  if (file == NULL)
  {
    fprintf(stderr, "No file with that filename was found.\n");
    exit(1);
  }

  while (fgets(data, sizeof(data), file) != NULL)
  {
    char *ptr;
    unsigned char ret = strtol(data, &ptr, 2);

    if (ptr == data)
    {
      continue;
    }

    cpu->ram[address] = ret;
    address++;
  }

  fclose(file);

  // TODO: Replace this with something less hard-coded
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char value, unsigned char address)
{
  cpu->ram[address] = value;
}

void cpu_push(struct cpu *cpu, unsigned char value)
{ // abstract push for use in CALL
  cpu->registers[SP]--;
  cpu->ram[cpu->registers[SP]] = value;
}

unsigned char cpu_pop(struct cpu *cpu)
{ // abstract pop for use in RET
  unsigned char new_address = cpu->ram[cpu->registers[SP]];
  cpu->registers[SP]++;
  return new_address;
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
    cpu->registers[regA] *= cpu->registers[regB];
    printf("Multiply has finished.\n");
    break;

  case ALU_ADD:
    cpu->registers[regA] += cpu->registers[regB];
    break;

  case ALU_SUB:
    break;

  case ALU_DIV:
    break;

  case ALU_INC:
    break;

  case ALU_DEC:
    break;

  default:
    break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */

void cpu_run(struct cpu *cpu)
{
  int running = 1;

  while (running)
  {
    // TODO
    unsigned char operandA, operandB;
    unsigned char instruction = cpu_ram_read(cpu, cpu->PC);
    unsigned int num_operands = instruction >> 6;

    if (num_operands == 2)
    {

      operandA = cpu_ram_read(cpu, (cpu->PC + 1) & 0xff),
      operandB = cpu_ram_read(cpu, (cpu->PC + 2) & 0xff);
    }
    else if (num_operands == 1)
    {
      operandA = cpu_ram_read(cpu, (cpu->PC + 1) & 0xff);
    }

    switch (instruction)
    {

    case HLT:
      printf("HLT received at line %d, ending program.\n", cpu->PC);
      running = 0;
      break;

    case PRN:
      printf("%d\n", cpu->registers[operandA]);
      break;

    case LDI:
      cpu->registers[operandA] = operandB;
      break;

    case ADD:
      alu(cpu, ALU_ADD, operandA, operandB);
      break;

    case MUL:
      printf("MUL was called.\n");
      alu(cpu, ALU_MUL, operandA, operandB);
      break;

    case PUSH:
      cpu_push(cpu, cpu->registers[operandA]);
      break;

    case POP:
      cpu->registers[operandA] = cpu_pop(cpu);

      break;
    case CALL:
      cpu_push(cpu, cpu->PC + 1);

      cpu->PC = cpu->registers[operandA] - 2;
      break;

    case RET:
      cpu->PC = cpu_pop(cpu);
      break;

    default:
      break;
    }
    cpu->PC += num_operands + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{

  memset(cpu->registers, 0, sizeof(cpu->registers));

  cpu->registers[SP] = 0xF4;

  cpu->PC = 0;
  cpu->FL = 0;

  memset(cpu->ram, 0, sizeof(cpu->ram));

  printf("CPU Initilized Successfully!\n");
}