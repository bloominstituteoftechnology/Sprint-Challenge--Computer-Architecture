#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

// returns value at ram address
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address) {
  return cpu -> ram[address];
}

// writes a value at ram address
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value) {
  cpu -> ram[address] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  // opens file
  FILE *fp = fopen(filename, "r");

  // checks to see if file exists
  if(fp == NULL) {
    fprintf(stderr, "ls8: error opening file: %s\n", filename);
    exit(2);
  }
  // buffer to read line by line
  char line[8192];\
  // starting address
  int address = 0;

  // while a line exists
  while (fgets(line, sizeof line, fp) != NULL) {
    // pointer that stores part of the line
    char *endptr;
    // int that stores number part of the line
    unsigned char val = strtoul(line, &endptr, 2);
    // if the string is equal to a blank line continue to the next
    if(endptr == line) {
      continue;
    }
    // store the converted number into ram and increment the address
    cpu_ram_write(cpu, address++, val);
    // cpu -> ram[address++] = val;
  }

  // closes file
  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char valA = cpu -> reg[regA];
  unsigned char valB = cpu -> reg[regB];

  switch (op) {
    case ALU_MUL:
      cpu -> reg[regA] = valA * valB;
      break;
    case ALU_ADD:
      cpu -> reg[regA] = valA + valB;
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
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu -> PC);

    // 2. Figure out how many operands this next instruction requires
    unsigned int num_of_op = IR >> 6;
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operandA = cpu_ram_read(cpu, cpu -> PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu -> PC + 2);

    // printf("TRACE: %02X | %02X %02X %02X |", cpu->PC, IR, operandA, operandB);

    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    switch(IR) {
      case LDI:
        // sets the value of a register to a int
        cpu -> reg[operandA] = operandB;
        // cpu -> PC += 3;
        break;
      case PRN:
        // prints the value stored in a given register
        printf("%d\n", cpu -> reg[operandA]);
        // cpu -> PC += 2;
        break;
      case MUL:
        // gets handled by alu
        // it multiplies the values in two registers and stores the result in registerA
        alu(cpu, ALU_MUL, operandA, operandB);
        // cpu -> PC += 3;
        break;
      case POP:
        // pops the value of the stack into a given register
        cpu -> reg[operandA] = cpu_ram_read(cpu, cpu -> SP++);
        break;
      case PUSH:
        // pushes the value of a given register on the stack
        cpu -> SP -= 1;
        cpu -> ram[cpu -> SP] = cpu -> reg[operandA];
        break;
      case ADD:
        // gets handled by alu
        // it adds the values in two registers and stores the result in registerA
        alu(cpu, ALU_ADD, operandA, operandB);
        break;
      case CALL:
        // stores the address of the next instruction after call
        cpu -> ram[--cpu -> SP] = cpu -> PC + 1 + num_of_op;
        // pc is set to the address stored in a given register
        cpu -> PC = cpu -> reg[operandA];
        break;
      case JMP:
        // jump to the address stored in a given register
        cpu -> PC = cpu -> reg[operandA];
        break;
      case RET:
        // pops the value from the stack and stores it in pc
        cpu -> PC = cpu_ram_read(cpu, cpu -> SP++);
        break;
      case HLT:
        // halts the cpu and exits the emulator
        running = 0;
        break;
      default:
        printf("unknown instruction 0x%02X at 0x%02X\n", IR, cpu -> PC);
        exit(1);
    }
    // 6. Move the PC to the next instruction.
    int set = IR >> 4 & 0x01;
    if(set == 0) {
      cpu -> PC += num_of_op + 1;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu -> PC = 0;

  // initializes ram and reg from cpu.h
  memset(cpu -> ram, 0, sizeof(cpu -> ram));
  memset(cpu -> reg, 0, sizeof(cpu -> reg));
}
