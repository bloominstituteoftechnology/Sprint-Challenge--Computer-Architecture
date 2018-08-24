#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

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
  // const int DATA_LEN = 6;
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };

  // int address = 0;

  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  FILE *f;
  f = fopen(file, "r");
  if (f == NULL)
  {
    fprintf(stderr, "We can't open the file.\n");
    exit(1);
  }
  unsigned char address = 0;
  unsigned char line[1024];

  while (fgets(line, sizeof(line), f) != NULL)
  {
    unsigned char *endptr;
    unsigned long int new_line;
    new_line = strtoul(line, &endpts, 2);
    if (line == endptr)
    {
      continue;
    }
    cpu_ram_write(cpu, address++, new_line);
    cpu->ram[address++] = new_line;
  }
  fclose(f);
}

// TODO: Replace this with something less hard-coded

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    // TODO
    cpu->reg[regA] *= cpu->reg[regB];
    break;

  case ALU_ADD:
    cpu->reg[regA] += cpu->reg[regB];
    break;
    // TODO: implement more ALU ops
  }
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
    IR_size = (IR >> 6) + 1;
    address = cpu->pc;

    switch (IR)
    {

    case LDI:
      cpu->reg[operandA] = operandB;
      // cpu->pc += 3;
      break;

    case PRN:
      printf("%d\n", cpu->reg[operandA]);
      // cpu->pc += 2;
      break;

    case HLT:
      running = 0;
      break;

    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;

    case ADD:
      alu(cpu, ALU_ADD, operandA, operandB);
      break;

    case PUSH:
      cpu->reg[7]--;
      cpu->ram[cpu->reg[7]] = cpu->reg[operandA];
      break;

    case POP:
      cpu->reg[operandA] = cpu->ram[cpu->reg[7]];
      cpu->reg[7]++;
      break;

    case CALL:
      cpu->reg[7]--;
      cpu_ram_write(cpu, cpu->reg[7], cpu->pc + (IR_size - 1));
      cpu->pc = cpu->reg[operandA] - IR_size;
      break;

    case RET:
      cpu->pc = cpu_ram_read(cpu, cpu->reg[7]);
      cpu->reg[7]++;
      break;

    default:
      printf("unknown instruction at %02x: %02x\n", cpu->pc, IR);
      exit(2);
    }
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
  // TODO: Zero registers and RAM
  for (int i = 0; i < 256; i++){
    cpu->reg[i] = 0;
  }
}
