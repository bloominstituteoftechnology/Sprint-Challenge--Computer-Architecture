#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar)
{
  return cpu->ram[mar];
}

void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr)
{
  cpu->ram[mar] = mdr;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fp = fopen(filename, "r");
  char line[1024];
  unsigned char address = 0;

  if (fp == NULL)
  {
    fprintf(stderr, "error opening file %s\n", filename);
    exit(2);
  }

  while (fgets(line, sizeof line, fp) != NULL)
  {
    char *endpointer = NULL;
    unsigned char byte = strtoul(line, &endpointer, 2);

    if (endpointer == line)
    {
      continue;
    }

    cpu_ram_write(cpu, address++, byte);
  }

  fclose(fp);

#if 0
  char data[DATA_LEN] = {
      // From print8.ls8
      0b10000010, // LDI R0,8
      0b00000000,
      0b00001000,
      0b01000111, // PRN R0
      0b00000000,
      0b00000001 // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++)
  {
    cpu->ram[address++] = data[i];
  }
#endif
  // TODO: Replace this with something less hard-coded
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

    // implement more ALU ops
  case ALU_CMP:
    if (cpu->registers[regA] == cpu->registers[regB])
    {
      cpu->FL = 0b00000001;
    }
    else
    {
      cpu->FL = 0b00000000;
    }
    break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char IR, operandA, operandB;

  while (running)
  {
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu->PC);
    operandA = cpu_ram_read(cpu, (cpu->PC + 1) & 0xff);
    operandB = cpu_ram_read(cpu, (cpu->PC + 2) & 0xff);

    int add_to_pc = (IR >> 6) + 1;

    printf("TRACE: %02X | %02X %02X %02X |", cpu->PC, IR, operandA, operandB);

    for (int i = 0; i < 8; i++)
    {
      printf(" %02X", cpu->registers[i]);
    }

    printf("\n");

    // 2. switch() over it to decide on a course of action.
    switch (IR)
    {
    case LDI:
      cpu->registers[operandA] = operandB;
      break;

    case PRN:
      printf("%d\n", cpu->registers[operandA]);
      break;

    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;

    case CMP:
      alu(cpu, ALU_CMP, operandA, operandB);
      break;

    case JMP:
      cpu->PC = cpu->registers[operandA & 7];
      add_to_pc = 0;
      break;

    case HLT:
      running = 0;
      break;
    }
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    cpu->PC += add_to_pc;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // Initialize the PC and other special registers
  cpu->PC = 0;
  cpu->FL = 0;

  // Zero registers and RAM
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->registers, 0, sizeof cpu->registers);
  cpu->registers[7] = 0b11110100; //Beej said to add this?
}
