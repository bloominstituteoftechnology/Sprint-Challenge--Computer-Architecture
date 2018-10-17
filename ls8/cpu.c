#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->RAM[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->RAM[address] = value;
}

void cpu_load(struct cpu *cpu, char *argv[])
{
  // file pointer
  FILE *fp;
  int address = 0;
  char read[100];

  fp = fopen(argv[1], "r");
  if(fp == NULL) {
    fprintf(stderr, "Filepointer goofed\n");
    exit(1);
  }

  // loop until fgets is at the end of the file
  while (fgets(read, sizeof(read), fp)) {
    unsigned long instruction;
    char *endptr;
    // cast line to binary until instruction is captured
    instruction = strtoul(read, &endptr, 2);
    // write instruction to RAM @address
    if(!(*read == *endptr)) cpu_ram_write(cpu, address++, instruction);
  }
}

void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->reg[regA] *= cpu->reg[regB];
      break;
    case ALU_ADD:
      cpu->reg[regA] += cpu->reg[regB];
      break;
    default:
      printf("No instruciton for ALU provided\n"); 
      break;
  }
}

void cpu_jump(struct cpu *cpu, unsigned char address){
  cpu->pc = cpu->reg[address];
}

void cpu_run(struct cpu *cpu)
{
  int running = 1;

  while (running) {
    // Read Instructions from RAM
    unsigned char IR = cpu_ram_read(cpu, cpu->pc);
    // Read additional operands 
    unsigned char operandA = cpu_ram_read(cpu, cpu->pc + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->pc + 2);

    int instruction_set_pc = (IR >> 4) & 1;
    switch(IR){
      case JNE:
        if(!cpu->fl) cpu_jump(cpu, operandA);
        else cpu->pc += 2;
        break;
      case JEQ:
        if(cpu->fl) cpu_jump(cpu, operandA);
        else cpu->pc += 2;
        break;
      case JMP:
        
        cpu_jump(cpu, operandA);
        instruction_set_pc = 1;
        break;
      case CMP:
        // compares values of register @operandA and @operandB
        if(cpu->reg[operandA] == cpu->reg[operandB]) cpu->fl = 1;
        break;
      case ST:
        // writes value of reg@operandB at the RAM[cpu->reg[operandA]] 
        cpu_ram_write(cpu, cpu->reg[operandA], cpu->reg[operandB]);
        break;
      case ADD:
        // self explanatory
        alu(cpu, ALU_ADD, operandA, operandB);
        break;
      case CALL:
        // saves the current pc to RAM, sets pc to
        // subroutine address
        cpu->sp--;
        cpu_ram_write(cpu, cpu->sp, (cpu->pc + 2));
        cpu_jump(cpu, operandA);
        break;
      case RET:
        // return to prev saved index
        cpu->pc = cpu_ram_read(cpu, cpu->sp++);
        break;
      case PUSH:
        // decrease the stack pointer, write value of register
        // to new address 
        cpu->sp--;
        cpu_ram_write(cpu, cpu->sp, cpu->reg[operandA]);
        break;
      case POP:
        // move the stack pointer and return the popped item
        if(cpu->sp == 244) fprintf(stderr, "Can't pop off an empty stack!\n");
        cpu->reg[operandA] = cpu_ram_read(cpu, cpu->sp++);
        break;
      case LDI:
        // Set register to int
        cpu->reg[operandA] = operandB;
        break;
      case PRN:
        // print register
        printf("%d\n", cpu->reg[operandA]);
        break;
      case HLT:
        // HALT
        running = 0;
        break;
      case MUL:
        // Call ALU to execute MUL instruction
        alu(cpu, ALU_MUL, operandA, operandB);
        break;
      default:
        printf("unknown instruction at %02x: %02x\n", cpu->pc, IR);
        exit(2);
    }
    // add to the PC according to the executed instruction
    // using >> bitwise shifting of the binary instruction
    // if flag for action handling the pc is not set
    if (!instruction_set_pc) {
      cpu->pc += ((IR >> 6) & 0x3) + 1;
    }
  }
}

void cpu_init(struct cpu *cpu)
{
  // Zeroing out the pc, registers and ram
  cpu->sp = 244;
  cpu->pc = 0;
  cpu->fl = 0;
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->RAM, 0, sizeof(cpu->RAM));
}
