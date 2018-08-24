#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address) {
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value) {
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
    unsigned char *endptr;
    unsigned char new_line;

    if (f == NULL) {
      fprintf(stderr, "We cannot open the given file.\n");
      exit(1);
    }

  while (fgets(line, sizeof(line), f) != NULL) {
    new_line = strtoul(line, &endptr, 2);
    if (line == endptr) {
      continue;
    }
    // cpu_ram_write(cpu, address++, new_line);
    cpu->ram[address++] = new_line;
  }

  fclose(f);

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *reg = cpu->reg;
  switch (op) {
    case ALU_MUL:
      // TODO
      cpu->reg[regA] *= cpu->reg[regB];
      break;
    
    case ALU_ADD:
      cpu->reg[regA] += cpu-> reg[regB];
      break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  // int address;
  // int IR_size;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->pc);
    unsigned char operandA = cpu_ram_read(cpu, cpu->pc + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->pc + 2);
    // IR_size = (IR >> 6) + 1;
    // address = cpu->pc;
    
    switch(IR) {

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
        cpu->pc += 3;
        break;

      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        cpu->pc += 3;
        break;

      case PUSH:
        cpu->reg[7] -= 1;
        cpu->ram[cpu->reg[7]] = cpu->reg[operandA];
        break;

      case POP:
        cpu-> reg[operandA] = cpu->ram[cpu->reg[7]];
        cpu->reg[7] += 1;
        cpu->pc += 2;
        break;

      case CALL:
        cpu->reg[7] -= 1;
        // cpu_ram_write(cpu, cpu->reg[7], cpu->pc + (IR_size - 1));
        cpu->ram[cpu->reg[7]] = cpu->pc + 2;
        cpu->pc = cpu->reg[operandA];
        break;

      case RET:
        cpu->pc = cpu->ram[cpu, cpu->reg[7]];
        cpu->reg[7] += 1;
        break;
        
      case CMP:
        if (cpu->reg[operandA] == cpu->reg[operandB]) {
          cpu->fl = 1;
        }
        cpu->pc += 3;
        break;

      case JMP:
        cpu->pc = cpu->reg[operandA];
        break;

      case JEQ:
        if (cpu->fl == 1) {
          cpu->pc = cpu->reg[operandA];
        } else {
          cpu->pc += 2;
        }
        break;

      case JNE:
        if (cpu->fl == 0) {
          cpu->pc = cpu->reg[operandA];
        } else {
          cpu->pc += 2;
        }
        break;

      default:
        printf("Cannot identify instruction at %02x: %02x\n", cpu->pc, IR);
        exit(2);
    }

    cpu->pc += (IR >> 6) + 1;

    // Just a test to see if this format works, need to confirm (Stack Overflow!!)
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
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
  for (int i = 0; i < 256; i++) {
    cpu->ram[i] = 0;
  }

  for (int i = 0; i < 8; i++) {
    cpu->reg[i] = 0;
  }
}
