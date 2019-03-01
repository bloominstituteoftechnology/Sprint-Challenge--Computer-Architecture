#include "cpu.h"
#define DATA_LEN 6
#define SP 7
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_ram_write(struct cpu *cpu, unsigned char value, unsigned char address)
{
  cpu->ram[address] = value;
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void cpu_push(struct cpu *cpu, unsigned char val)
{
  cpu->reg[SP]--;
  cpu_ram_write(cpu, val, cpu->reg[SP]);
}

unsigned char cpu_pop(struct cpu *cpu)
{
  unsigned char val = cpu_ram_read(cpu, cpu->reg[SP]);
  cpu->reg[SP]++;

  return val;
}

void cpu_load(struct cpu *cpu, char *filename)
{
  
  int address = 0;

  FILE *file = fopen(filename, "r"); // open the filename we are given as arg

  char data[1024];

  if(file == NULL)
  {
    fprintf(stderr, "No file with that name found\n");
    exit(1);
  }

  // TODO: Replace this with something less hard-coded

  while(fgets(data, sizeof(data), file) != NULL)
  {
    char *ptr;
    unsigned char ret = strtol(data, &ptr, 2);

    if(ptr == data)
    {
      continue;
    }
    cpu_ram_write(cpu, ret, address++);
  }
  fclose(file);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *reg = cpu->reg;
  unsigned char value_b = reg[regB];

  switch (op) {
    case ALU_MUL:
      // TODO
      reg[regA] *= value_b;
      break;
    case ALU_ADD:
      cpu->reg[regA] = cpu->reg[regA] + cpu->reg[regB];
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


  while (running) {

    // initialize operands
    unsigned char operandA;
    unsigned char operandB;
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char instruction = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    unsigned int operands = instruction >> 6; 
    // 3. Get the appropriate value(s) of the operands following this instruction
    if(operands == 2)
    {
      operandA = cpu_ram_read(cpu, (cpu->PC+1) & 0xff);
      operandB = cpu_ram_read(cpu, (cpu->PC+2) & 0xff);
    }
    else if (operands == 1)
    {
      operandA = cpu_ram_read(cpu, (cpu->PC+1) & 0xff);
    }
    // 4. switch() over it to decide on a course of action.
    switch(instruction)
    {
      case HLT:
        running = 0;
        break;
      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        break;
      case LDI:
        cpu->reg[operandA] = operandB;
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;
      case PUSH:
        cpu_push(cpu, cpu->reg[operandA]);
        break;
      case POP:
        cpu->reg[operandA] = cpu_pop(cpu);
        break;
      case CALL:
        cpu->reg[SP]--;
        cpu_ram_write(cpu, cpu->reg[SP], cpu->PC + operands);

        cpu->PC = cpu->reg[operandA];
        break;
      case RET:
        cpu->PC = cpu_ram_read(cpu, cpu->reg[SP]);
        cpu->reg[SP]++;
        break;
      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        break;
      default:
        break;
    }
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    cpu->PC += operands+1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = -1;
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->reg, 0, sizeof cpu->reg);
  cpu->reg[SP] = ADDR_EMPTY_STACK;
  cpu->E = 0;
}
