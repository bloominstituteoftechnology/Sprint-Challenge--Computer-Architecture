#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define DATA_LEN 6


// "In cpu.c, add functions cpu_ram_read() and cpu_ram_write() that access the RAM inside the struct cpu."
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar) // mar <=> memory address register
{
  return cpu->ram[mar];
}

void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr) // mdr <=> memory data register
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
  unsigned char addr = 0x00;

  if (fp == NULL) {
    fprintf(stderr, "error opening file %s\n", filename);
    exit(2);
  }

  while (fgets(line, sizeof line, fp) != NULL) {
    char *endptr;

    unsigned char b = strtoul(line, &endptr, 2);

    //              endptr
    //                v 
    // Input "10000010 # Comment"
    //        ^
    //      line

    if (endptr == line) {
      // we go no numbers
      continue;
    }
    cpu_ram_write(cpu, addr++, b);
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
    0b00000001  // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++) {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
#endif
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
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char IR, operandA, operandB;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu,cpu->PC); // index/address of the instruction in the ram array
    operandA = cpu_ram_read(cpu,(cpu->PC+1) & 0xff); // 1 byte after instruction; the & 0xff serves to set the maximum of the value(0xff <=> 255) on the LHS
    operandB = cpu_ram_read(cpu,(cpu->PC+2) & 0xff); // 2 bytes after instruction

    // >> operator takes the binary representation of a number and shifts the number of bits to the right (putting in zeros for the new bits and the ones the get shifted out gets discarded); << to shift to the left 
    //        -the (IR >> 6) is the number of operands and the plus 1 is for the opcode itself, adding up to the number of bytes making up instruction
    int add_to_pc = (IR >> 6) + 1;

    printf("TRACE: %02X | %02X %02X %02X |", cpu->PC, IR, operandA, operandB);

    for (int i = 0; i <8; i++) {
      printf(" %02X", cpu->reg[i]);
    }
    printf("\n");

    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.

    switch(IR) {
      case LDI: // instruction LDI sets the value of a register to an integer
        // set value of register operandA to the value operandB
        cpu->reg[operandA] = operandB;
        break;
      
      case PRN: // instruction PRN prints the numeric value stored in the given register
        printf("%d\n", cpu->reg[operandA]);
        break;

      case PUSH:
        cpu->reg[7]--;
        cpu_ram_write(cpu, cpu->reg[7], cpu->reg[operandA & 7]); // & 7 clamps range of operandA to number up to 7  
        // cpu->ram[cpu->reg[7]] = cpu->reg[operandA]; same functionality as line above
        break;
      
      case POP:
        cpu->reg[operandA & 7] = cpu_ram_read(cpu, cpu->reg[7]);
        cpu->reg[7]++;
        break;
      
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;
      
      case JMP:
        cpu->PC = cpu->reg[operandA];
        add_to_pc=0;
        break;
      
      case HLT:
        running = 0;
        break;

      default:
        printf("Unknown instruction %02x\n", IR);
        exit(3);
    }
    // 4. Move the PC to the next instruction.
    cpu->PC += add_to_pc;
    cpu->PC &= 0xff;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;

  // TODO: Zero registers and RAM
  memset(cpu->ram, 0, sizeof(cpu->ram));
  memset(cpu->reg, 0,  sizeof(cpu->reg));
}
