#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

#define DATA_LEN 6
#define SP 7

unsigned char cpu_ram_read(struct cpu *cpu, unsigned mar)
{
  return cpu->ram[mar];
}




void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr)
{
  cpu->ram[mar] = mdr;
}

void push(struct cpu *cpu, unsigned char value)
{
  cpu->registers[SP]--;

  cpu_ram_write(cpu, cpu->registers[SP], value);  
}



unsigned char pop(struct cpu *cpu) 
{
  unsigned char value = cpu_ram_read(cpu, cpu->registers[SP]);

  cpu->registers[SP]++;
  return value;
}


void cpu_load(struct cpu *cpu, char *filename)
{


  FILE *fp = fopen(filename, "r");

  if(fp == NULL) {
    fprintf(stderr, "ls8: error opening file: %s\n", filename);
    exit(1);
  }

  char buff[8192];
  int address = 0;

  while (fgets(buff, sizeof buff, fp) != NULL) {
    unsigned char val = strtoul(buff, NULL, 2);
    cpu_ram_write(cpu, address++, val);
  }


  fclose(fp);

  }



  // TODO: Replace this with something less hard-coded

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{

  unsigned char num_A = cpu->registers[regA];
  unsigned char num_B = cpu->registers[regB];
  
  switch (op) {
    case ALU_MUL:
      cpu->registers[regA] = num_A * num_B;
      break;
    case ALU_ADD:
      cpu->registers[regA] = num_A + num_B;
      break;
    case ALU_CMP:
        if(num_A > num_B){
          cpu->FL = 0b00000010;
        } else if (num_A < num_B){
          cpu->FL = 0b00000100;
        } else if(num_A == num_B){
          cpu->FL = 0b00000001;
        }
        break;
    
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
    IR = cpu_ram_read(cpu, cpu->PC);

    unsigned int operands = IR >> 6;
    if (operands == 2) {
      operandA = cpu_ram_read(cpu, cpu->PC+1);
      operandB = cpu_ram_read(cpu, cpu->PC+2);
    } else if (operands == 1) {
      operandA = cpu->ram[cpu->PC + 1];
    }

  
  
    // 2. switch() over it to decide on a course of action.
    switch (IR) {
      case LDI:
        cpu->registers[operandA] = operandB;
        break;
      case PRN:
        printf("%d\n", cpu->registers[operandA]);
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;
      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        break;
      case PUSH:
        push(cpu, cpu->registers[operandA]);

        break;
      case POP:
        cpu->registers[operandA] = pop(cpu);
        break;
      case CALL:
        push(cpu, cpu->PC + 1);
        cpu->PC = cpu->registers[operandA] - 2;
        break;
      case RET:
        cpu->PC = pop(cpu);
        break;
      case CMP:
        alu(cpu, ALU_CMP, operandA, operandB);
        break;
      case JMP:
        cpu->PC = cpu->registers[operandA] - operands -1;
        break;
      case JEQ:
        if (cpu->FL == 0b00000001)
        {
          cpu->PC = cpu->registers[operandA];
        }
        break;
      case JNE:
        if ((cpu->FL & 0b000000001) == 0)
        {
          cpu->PC = cpu->registers[operandA] - operands -1;
        }
        break;
      case HTL:
        exit(1);
        break;
    }
    cpu->PC += operands + 1;
  }
   

    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  for (int i = 0; i < 6; i++)
  {
    cpu->registers[i] = 0;
  }
  cpu->PC = 0;
  cpu->FL = 0;
  cpu->registers[7] = 0xF4;
  // TODO: Zero registers and RAM
  memset(cpu->ram, 0, sizeof(cpu->ram));
  // memset(cpu->registers, 0, sizeof(cpu->registers));
}
