#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6
// stack pointer
#define SP 7

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, int argc, char *argv[]) // make ./ls8 examples/print8.ls8
{
  // TODO: Replace this with something less hard-coded

  // if argc is < 2, "File does not exist"
  if (argc < 2)
  {
    printf("File does not exist.\n");
    exit(1);
  }

  char *file = argv[1];

  // need file
  FILE *fp = fopen(file, "r");

  // if file does not exist
  if (fp == NULL)
  {
    printf("File does not exist");
    exit(1);
  }
  else
  {
    // create a var that will hold line of file
    char file_line[1024];
    int address = 0;

    // while not end of file
    while (fgets(file_line, sizeof(file_line), fp) != NULL)
    {
      // cycle through ram and store value of line into ram
      char *ptr;
      unsigned char return_value;
      return_value = strtol(file_line, &ptr, 2);

      // if empty line continue
      if (file_line == NULL)
      {
        continue;
      }

      cpu->ram[address] = return_value;
      address++;
    }
  }
  // close file
  fclose(fp);
}

// cpu read ram
// would have to take a cpu struct and an index for ram
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char memadr)
{
  return cpu->ram[memadr];
}

// cpu write ram
// would have to take a cpu struct, an index for ram, and char value
void cpu_ram_write(struct cpu *cpu, unsigned char memadr, unsigned char value)
{
  cpu->ram[memadr] = value;
}

// cpu push
void cpu_push(struct cpu *cpu, unsigned char value)
{
  // decrement stack pointer
  cpu->reg[SP]--;
  cpu_ram_write(cpu, cpu->reg[SP], value);
}

// cpu pop
unsigned char cpu_pop(struct cpu *cpu)
{
  unsigned char ret = cpu->ram[cpu->reg[SP]];
  cpu->reg[SP]++;
  return ret;
}

//

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

  // TODO: implement more ALU ops
  case ALU_ADD:
    cpu->reg[regA] += cpu->reg[regB];
    break;

  case ALU_CMP:
    if (cpu->reg[regA] == cpu->reg[regB])
    {
      cpu->FL = 0b00000001;
    }
    else if (cpu->reg[regA] > cpu->reg[regB])
    {
      cpu->FL = 0b00000010;
    }
    else
    {
      cpu->FL = 0b00000100;
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
  // create ops
  unsigned char operandA;
  unsigned char operandB;

  while (running)
  {
    // TODO

    // 1. Get the value of the current instruction (in address PC).
    unsigned char instruction = cpu->ram[cpu->PC];

    // 2. Figure out how many operands this next instruction requires
    unsigned int num_operands = instruction >> 6;

    // 3. Get the appropriate value(s) of the operands following this instruction
    if (num_operands == 2)
    {
      operandA = cpu_ram_read(cpu, (cpu->PC + 1) & 0xff);
      operandB = cpu_ram_read(cpu, (cpu->PC + 2) & 0xff);
    }
    else if (num_operands == 1)
    {
      operandA = cpu_ram_read(cpu, (cpu->PC + 1) & 0xff);
    }

    // 4. switch() over it to decide on a course of action.
    switch (instruction)
    {
    case HLT:
      running = 0;
      break;

    case PRN:
      // print whatever is in the specified register
      printf("%d\n", cpu->reg[operandA]);
      break;

    case LDI:
      cpu->reg[operandA] = operandB;
      break;

    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;

    // uses function cpu_push that pushes value into reg
    case PUSH:
      cpu_push(cpu, cpu->reg[operandA]);
      break;

    case POP:
      cpu->reg[operandA] = cpu_pop(cpu);
      break;

    case ADD:
      alu(cpu, ALU_ADD, operandA, operandB);
      break;

    case CALL:
      cpu_push(cpu, cpu->PC + 1);
      cpu->PC = cpu->reg[operandA] - 1;
      break;

    case RET:
      cpu->PC = cpu_pop(cpu);
      break;

    case CMP:
      // should compare both values
      // and change the flag
      alu(cpu, ALU_CMP, operandA, operandB);
      break;

    case JMP:
      // should jump to location
      cpu->PC = cpu->reg[operandA];
      cpu->PC += 1;
      break;

    case JEQ:
      // should check if flag is true
      if (cpu->FL == 0b00000001)
      {
        // move counter
        cpu->PC = cpu->reg[operandA];
        cpu->PC -= 1;
      }
      break;

    case JNE:
      // should check if flag is false
      if (cpu->FL != 0b00000001)
      {
        // move counter
        cpu->PC = cpu->reg[operandA];
        cpu->PC -= 1;
      }
      break;

    default:
      break;
    }
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    cpu->PC += num_operands + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  // set all values to 0
  cpu->PC = 0;
  cpu->FL = 0;

  cpu->reg[SP] = ADDR_EMPTY_STACK;

  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));
}
