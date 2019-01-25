#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
  RAM interface functions
*/

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
 return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char value, unsigned char address)
{
  cpu->ram[address] = value;
}


/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{

  FILE *fp;

  char line[1024];
  if ((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Cannot open file: %s\n", filename);
    // exit(1);
  }

  int address = 0;

  while (fgets(line, sizeof(line), fp) != NULL) {

    char *ptr;
    unsigned char ret = strtol(line, &ptr, 2);

    if (ptr == line) {
      continue;
    }

    cpu_ram_write(cpu, ret, address);
    address ++;

  }
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->reg[regA] = cpu->reg[regA] * cpu->reg[regB];
      break;
    case ALU_ADD:
      cpu->reg[regA] = cpu->reg[regA] + cpu->reg[regB];

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  unsigned char operand_a;
  unsigned char operand_b;
  unsigned char SP = cpu->reg[7];

  while (running) {
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    unsigned int number_of_operands = (IR >> 6);
    // 3. Get the appropriate value(s) of the operands following this instruction
    if (number_of_operands == 2) {
      operand_a = cpu_ram_read(cpu, (cpu->PC+1) & 0xff);
      operand_b = cpu_ram_read(cpu, (cpu->PC+2) & 0xff);
    } else if (number_of_operands == 1) {
      operand_a = cpu_ram_read(cpu, (cpu->PC+1) & 0xff);
    } else {

    }
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    switch (IR) {
      case LDI:
        cpu->reg[operand_a] = operand_b;
        break;
      case PRN:
        printf("%d\n", cpu->reg[operand_a]);
        break;
      case PUSH:
        SP--;
        cpu_ram_write(cpu, cpu->reg[operand_a], SP);
        break;
      case POP:
        cpu->reg[operand_a] = cpu_ram_read(cpu, SP);
        SP++;
        break;
      case MUL:
        alu(cpu, ALU_MUL, operand_a, operand_b);
        break;
      case ADD:
        alu(cpu, ALU_ADD, operand_a, operand_b);
        break;
      case CALL:
        //should be refactored to call PUSH but with PC +1
        SP--;
        cpu_ram_write(cpu, cpu->PC+1, SP);

        cpu->PC = cpu->reg[operand_a];
        number_of_operands = -1; // this is to execute the instruction at the new PC
        break;
      case RET:
        // should be refactored to call POP except set the PC
        cpu->PC = cpu_ram_read(cpu, SP);
        SP++;

        break;
      case HLT:
        running = 0;
        break;
      default:
        break;
    }
    // 6. Move the PC to the next instruction.
    cpu->PC = cpu->PC + number_of_operands + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  memset(cpu->ram, 0, 256);

  cpu->PC = 0;

  memset(cpu->reg, 0, 8);
  cpu->reg[7] = 0xF4;
}
