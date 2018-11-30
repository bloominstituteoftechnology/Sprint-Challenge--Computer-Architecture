#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6

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

void cpu_load(struct cpu *cpu, char *argv[])
{
  // char data[DATA_LEN] = {
  //     // From print8.ls8
  //     0b10000010, // LDI R0,8
  //     0b00000000,
  //     0b00001000,
  //     0b01000111, // PRN R0
  //     0b00000000,
  //     0b00000001 // HLT
  // };

  // for (int i = 0; i < DATA_LEN; i++)
  // {
  //   cpu->ram[address++] = data[i];
  // }

  // TODO: Replace this with something less hard-coded

  FILE *fp = fopen(argv[1], "r");
  char str[256];
  int address = 0;
  char *ptr;

  while (fgets(str, sizeof(str), fp) != NULL)
  {
    unsigned char data = strtoul(str, &ptr, 2);
    if (ptr == str)
    {
      continue;
    }
    cpu->ram[address++] = data;
  }
  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *reg = cpu->registers;

  switch (op)
  {
  case ALU_MUL:
    // TODO
    reg[regA] *= reg[regB];
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

  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operB = cpu_ram_read(cpu, cpu->PC + 2);

    switch (IR)
    {
    case LDI:                        // set value of register to integer
      cpu->registers[operA] = operB; //operands R0 = operandA, R8 = operandsB
      cpu->PC += 3;
      break;
    case PRN: //print numeric value at the register
      printf("%d \n", cpu->registers[operA]);
      cpu->PC += 2;
      break;
    case MUL:
      alu(cpu, ALU_MUL, operA, operB);
      cpu->PC += 3;
      break;
    case PUSH:
      cpu->registers[7] --;
      cpu->ram[cpu->registers[7]] = cpu->registers[operA];
      cpu->PC += 2;
      break;
    case POP:
      cpu->registers[operA] = cpu_ram_read(cpu, cpu->registers[7]);
      cpu->registers[7] ++;
      cpu->PC += 2;
      break;
    case CMP: //compares
      if (cpu->registers[operA] == cpu->registers[operB]) {
        cpu->FL = 1;
      } else {
        cpu->FL = 0;
      }
      if (cpu->registers[operA] < cpu->registers[operB]) {
        cpu->FL = 1;
      } else {
        cpu->FL = 0;
      }
      if (cpu->registers[operA] > cpu->registers[operB]) {
        cpu->FL = 1;
      } else {
        cpu->FL = 0;
      }
      cpu->PC += 3;
      break;
    case JMP:
      cpu->PC = cpu->registers[operA];
      cpu->PC += 2;
      break;
    case JEQ:
      if (cpu->FL == 1) {
        cpu->PC = cpu->registers[operA];
      }
      cpu->PC += 2;
      break;
    case HLT: //exit the emulator
      running = 0;
      break;
    default:
      fprintf(stderr, "error\n");
      exit(2);
      break;
    }
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
  memset(cpu->ram, 0, sizeof(cpu->ram));
  memset(cpu->registers, 0, sizeof(cpu->registers));
}
