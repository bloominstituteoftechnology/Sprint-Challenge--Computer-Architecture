#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

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

unsigned char pop(struct cpu *cpu) 
{
  unsigned char val = cpu_ram_read(cpu, cpu->registers[7]);
  cpu->registers[7] ++;
  return val;
}

void push(struct cpu *cpu, unsigned char reg) 
{
  cpu->registers[7] --;
  cpu_ram_write(cpu, cpu->registers[7], cpu->registers[reg]);
}

void compare(struct cpu *cpu, unsigned char regA, unsigned char regB) {
  cpu->fl = 0x00;
  unsigned char valA = cpu->registers[regA];
  unsigned char valB = cpu->registers[regB];
  if (valA == valB) {
    cpu->fl += 1;
  } else if (valA > valB) {
    cpu->fl += 2;
  } else {
    cpu->fl += 4;
  }
}

void jump(struct cpu *cpu, unsigned char reg) {
  cpu->pc = cpu->registers[reg];
}

void cpu_load(struct cpu *cpu, char *file)
{
  FILE * f;
  f = fopen(file, "r");

  int address = 0;

  char line[256];

  while (fgets(line, sizeof(line), f)) {
    char *endptr;
    unsigned long int new_line;

    new_line = strtoul(line, &endptr, 2);

    if (endptr == line) {
      continue;
    }

    cpu->ram[address++] = new_line;
    printf("%lu\n", new_line);
  }

  fclose(f);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB];
      break;

    case ALU_ADD:
      cpu->registers[regA] = cpu->registers[regA] + cpu->registers[regB];
      break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    // Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->pc);
    unsigned char operandA = cpu_ram_read(cpu, cpu->pc+1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->pc+2);

    switch(IR) {
      case LDI:
        cpu->registers[operandA] = operandB;
        break;

      case PRN:
        printf("%d\n", cpu->registers[operandA]);
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
      
      case CALL:
        push(cpu, cpu->pc + 2);
        cpu->pc = cpu->registers[operandA];
        break;

      case RET:
        cpu->pc = pop(cpu);
        break;

      case POP:
        cpu->registers[operandA] = pop(cpu);
        break;

      case PUSH:
        push(cpu, operandA);
        break;

      case CMP:
        compare(cpu, operandA, operandB);
        break;

      case JMP:
        jump(cpu, operandA);
        break;

      case JEQ:
        if(cpu->fl == 00000001) {
          jump(cpu, operandA);
        }
        break;

      default:
        printf("unknown instruction: %02x, %02x", cpu->pc, IR);
        exit(2);
    }

    // Move the PC to the next instruction.
    cpu->pc += (IR >> 6) + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // Initialize the PC and other special registers
  cpu->pc = 0;
  cpu->registers[7] = 0xF4;
  cpu->fl = 0x00;
  // TODO: Zero registers and RAM
}
