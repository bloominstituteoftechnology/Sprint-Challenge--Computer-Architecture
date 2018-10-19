#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)  // returns the byte you're reading and returns the address of the cpu
{
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)  //write value to RAM
{
  cpu->ram[address] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
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
  // }

    // TODO: Replace this with something less hard-coded
  FILE *fp;
  char data[1024];
  int address = 0;

  fp = fopen(filename, "r");

  while (fgets(data, sizeof data, fp) != NULL) {
    unsigned char code = strtoul(data, NULL, 2);
    printf("%c\n", data[0]);
    if(data[0] == '\n' || data[0] == '#') {
      continue;
    }
    cpu->ram[address++] = code;
  }
  fclose(fp);
  // TODO: Replace this with something less hard-coded
}
/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->reg[regA] *= cpu->reg[regB];
      break;

      case ALU_ADD:
      // TODO
      cpu->reg[regA] += cpu->reg[regB];
      break;
    // TODO: implement more ALU ops
  }
}
void pop(struct cpu *cpu, unsigned char reg)
{
  cpu->reg[reg] = cpu->reg[7];
  cpu->reg[7] ++;
}

void push(struct cpu *cpu, unsigned char reg)
{
  cpu->reg[7] --;
  cpu->reg[7] = cpu->reg[reg];
}

/**
 * Run the CPU,
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);


    unsigned char operand_a = cpu_ram_read(cpu, (cpu->PC + 1) & 0xff);  //1st byte
    unsigned char operand_b = cpu_ram_read(cpu, (cpu->PC + 2) & 0xff);  //2nd byte

    int add_to_pc = (IR >> 6) + 1;

    printf("TRACE: %02X | %02X %02X %02X |", cpu->PC);

    // 2. switch() over it to decide on a course of action.
    switch (IR)
    {
    case LDI:  //Loading Program
      cpu->reg[operand_a] = operand_b;
      break;

    case PRN:  //Print
      printf("PRN: Print value %d\n\n", cpu->reg[operand_a]);
      break;

    case MUL:  //Multiply
      alu(cpu, ALU_MUL, operand_a, operand_b);
      break;

    case HLT:  //Halting
      running = 0;
      exit(0);

    case ADD:
      alu(cpu, ALU_ADD, operand_a, operand_b);
      break;

    case POP:
      pop(cpu, operand_a);
      break;

    case PUSH:
      push(cpu, operand_a);
      break;
    case JMP:
      cpu->PC = cpu->reg[operand_a & 7];
      add_to_pc = 0;

    default:
      printf("Does not exist\n\n");
      exit(1);
    }
      cpu->PC += add_to_pc;
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
  cpu->PC = 0;
  cpu->reg[7] = 0xF4;
  // TODO: Zero registers and RAM
  memset(cpu->reg, 0, sizeof cpu->reg);
  memset(cpu->ram, 0, sizeof cpu->ram);
}
