#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

unsigned char cpu_ram_read(struct cpu *cpu, int index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, int index, unsigned char value)
{
  cpu->ram[index] = value;
}

void cpu_push(struct cpu *cpu, unsigned char val)
{
  cpu->reg[5]--;

  cpu->ram[cpu->reg[5]] = val;
}

unsigned char cpu_pop(struct cpu *cpu)
{
  unsigned char val = cpu->ram[cpu->reg[5]];
  cpu->reg[5]++;
  return val;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(char *filename, struct cpu *cpu)
{
  FILE *fp;
  char line[1024];
  int address = 0x00;

  if ((fp = fopen(filename, "r")) == NULL)
  {
    fprintf(stderr, "Cannot open file %s\n", filename);
    exit(2);
  }

  while (fgets(line, sizeof line, fp) != NULL)
  {
    char *endchar;
    unsigned char byte = strtol(line, &endchar, 2);

    if (endchar == line)
    {
      continue;
    }

    cpu->ram[address++] = byte;
  }
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
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
  int equals = 0;

  unsigned char *reg = cpu->reg;
  unsigned char *ram = cpu->ram;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.

    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);

    int add_to_pc = (IR >> 6) + 1;

    switch (IR)
    {
      case LDI:
        reg[operandA] = operandB;
        break;

      case ADD:
       reg[operandA] += reg[operandB];
       break;

      case MUL:
        reg[operandA] *= reg[operandB];
        break;

      case CMP:
        if (reg[operandA] == reg[operandB])
        {
          equals = 1;
        }
        else {
          equals = 0;
        }
        break;

      case JMP:
        cpu->PC = cpu->reg[operandA];
        add_to_pc = 0;
        break;

      case JEQ:
        if (equals == 1)
        {
          cpu->PC = cpu->reg[operandA];
          add_to_pc = 0;
        }
        break;

      case JNE:
       if (equals == 0)
       {
         cpu->PC = cpu->reg[operandA];
         add_to_pc = 0;
       }
       break;

      case PUSH:
        cpu_push(cpu, reg[operandA]);
        break;

      case POP:
        reg[operandA] = cpu_pop(cpu);
        break;
      
      case CALL:
        cpu_push(cpu, cpu->PC + 2);
        cpu->PC = reg[operandA];
        break;

      case PRN:
        printf("%d\n", reg[operandA]);
        break;

      case RET:
        cpu->PC = cpu_pop(cpu);
        break;

      case HLT:
        running = 0;
        break;

      default:
        fprintf(stderr, "PC %02x: unknown instruction %02x\n", cpu->PC, IR);
        running = 0;
        break;
    }

    cpu->PC += add_to_pc;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
   cpu->PC = 0;

  // Zero registers and RAM
  memset(cpu->reg, 0, sizeof cpu->reg);
  memset(cpu->ram, 0, sizeof cpu->ram);

  // Initialize SP
  cpu->reg[5] = 0xF4;
}
