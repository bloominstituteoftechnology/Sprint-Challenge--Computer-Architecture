#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cpu.h"

// #define DATA_LEN 6
#define SP 7

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
void cpu_load(struct cpu *cpu, char *file)
{
  FILE *fp;
  char line[1024];
  int address = 0;

  fp = fopen(file, "r");

  while (fgets(line, sizeof line, fp) != NULL)
  {
    if (line[0] == '\n' || line[0] == '#') continue;

    unsigned char b;
    b = strtoul(line, NULL, 2);
    cpu->ram[address++] = b;
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      cpu->reg[regA] *= cpu->reg[regB];
      break;

    // TODO: implement more ALU ops
    case ALU_ADD:
      cpu->reg[regA] += cpu->reg[regB];
      break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char IR, opA, opB;

  printf("TRACE: PC | IR oA oB | 00 01 02 03 04 05 06 07\n");

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu->PC);
    opA = cpu_ram_read(cpu, (cpu->PC+1) & 0xff);
    opB = cpu_ram_read(cpu, (cpu->PC+2) & 0xff);

    int add_to_pc = (IR >> 6) + 1;

    printf("TRACE: %02X | %02X %02X %02X |", cpu->PC, IR, opA, opB);

    for (int i = 0; i < 8; i++)
    {
      printf(" %02X", cpu->reg[i]);
    }

    printf("\n");

    // 2. switch() over it to decide on a course of action.
    switch(IR)
    {
      case LDI:
        cpu->reg[opA] = opB;
        break;

      case MUL:
        alu(cpu, ALU_MUL, opA, opB);
        break;

      case PUSH:
        // decrement the SP 
        cpu->reg[SP]--;
        // copy the value in the given register to the address pointed to by SP 
        cpu->ram[cpu->reg[SP]] = cpu->reg[opA]; 
        break;

      case POP:
        // copy the value from the address pointed by SP to the given register
        cpu->reg[opA] = cpu->ram[cpu->reg[SP]];
        // increment the SP
        cpu->reg[SP]++;
        break;

      case ADD:
        alu(cpu, ALU_ADD, opA, opB);
        break;

      case CALL:
        // push address of instruction onto stack after call instruction
        cpu->reg[SP]--;
        cpu->ram[cpu->reg[SP]] = cpu->PC + 2; 
        // PC is set to the address stored in the given register
        cpu->PC = cpu->reg[opA];
        break;

      case RET:
        // pop the value from the top of the stack and store it in the PC
        cpu->reg[opA] = cpu->ram[cpu->reg[SP]];
        cpu->PC = cpu->reg[opA];
        cpu->reg[SP]++;
        break;
/*
      case CMP:
        if (cpu->reg[opA] == cpu->reg[opB])
        {
          cpu->FL = 0b00000001;
        }
        else if (cpu->reg[opA] < cpu->reg[opB])
        {
          cpu->FL = 0b00000100;
        }
        else if (cpu->reg[opA] > cpu->reg[opB])
        {
          cpu->FL = 0b00000010;
        }
        else
        {
          cpu->FL = 0b00000000;
        }
        break;

      case JMP:
        cpu->PC = cpu->reg[opA]; 
        break;

      case JEQ:
        cpu->PC = cpu->reg[opA]; 
        break;

      case JNE:
        cpu->PC = cpu->reg[opA]; 
        break;
*/
      case PRN:
        printf("%d\n", cpu->reg[opA]);
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
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0; 
  cpu->FL = 0;

  // TODO: Zero registers and RAM
  memset(cpu->ram, 0, sizeof cpu->ram); 
  memset(cpu->reg, 0, sizeof cpu->reg); 

  cpu->reg[SP] = 0xF4;
}
