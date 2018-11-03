#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar)
{
  return cpu->ram[mar];
}

void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr)
{
  cpu->ram[mar] = mdr;
}

void cpu_load(char *filename, struct cpu *cpu)
{

  FILE *fp;
  int address = 0;
  char line[1024];

  // open file
  fp = fopen(filename, "r");
  if (fp == NULL)
  {
    printf("INVALID FILE NAME.\n");
    return;
  }

  while (address < 256 && fgets(line, sizeof line, fp) != NULL) //not at eof
  {
    char *end_of_instr;
    unsigned char *data;
    // in a line, save the numeric data (byte/instruction/register/value) in data, ignoring non numeric characters
    data = strtol(line, &end_of_instr, 2);

    // if data != 0, save
    //data can be 0 IF no byte data OR byte = 0
    if (line != end_of_instr) //use pointer to first non-numeric character
    {
      //only write instructions, not comments
      cpu->ram[address] = data;
      address++;
    }
    // else ignore
  }
  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    cpu->registers[regA] *= cpu->registers[regB];
    cpu->PC += 3;
    break;
  case ALU_ADD:
    cpu->registers[regA] += cpu->registers[regB];
    cpu->PC += 3;
    break;
  case ALU_CMP:
    if (cpu->registers[regA] == cpu->registers[regB]) {
      int mask = 00000001;
      printf("%d", cpu->FL);
      cpu->FL = cpu->FL & mask;
      printf("%d", cpu->FL);
      // E flag 1
    } else if (cpu->registers[regA] < cpu->registers[regB]) {
      int mask = 00000100;
      printf("%d", cpu->FL);
      cpu->FL = cpu->FL & mask;
      printf("%d", cpu->FL);
      // L flag 1
    } else {
      // G flag 1
      int mask = 00000010;
      printf("%d", cpu->FL);
      cpu->FL = cpu->FL & mask;
      printf("%d", cpu->FL);
    }
    cpu->PC += 3;
    break;  
  }

}

int cpu_push(struct cpu *cpu, unsigned char regA) {
  cpu->registers[SP]--;
  cpu->ram[cpu->registers[SP]] = cpu->registers[regA];
  cpu->PC += 2;
}

int cpu_pop(struct cpu *cpu, unsigned char regA) {
  cpu->registers[regA] = cpu->ram[cpu->registers[SP]];
  cpu->registers[SP]++;
  cpu->PC += 2;
  printf("%d\n", cpu->registers[regA]);
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction or default case

  while (running)
  {
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);

    switch (IR)
    {
    case ADD:
      alu(cpu, ALU_ADD, operandA, operandB); 
      break; 
    case CALL:
      cpu->PC += 2;
      cpu->registers[SP]--;
      cpu->ram[cpu->registers[SP]] = cpu->PC;
      cpu->PC = cpu->registers[operandA];
      break;  
    case CMP:
      alu(cpu, ALU_CMP, operandA, operandB);  
      break;
    case HLT:
      running = 0;
      cpu->PC += 1;
      break;
    case JEQ:
      if (cpu-> FL == 00000001) {
        JMP;
        cpu->FL += 2;
      }
      //if E is 1, jump to address in register
      break; 
    case JMP:
      cpu->PC = cpu->registers[operandA];
      break;  
    case JNE:
      if(cpu->FL == 00000000) {
        JMP;
        cpu->FL += 2;
      }
      //if E is 0, jump to address in register
      break;   
    case LDI:
      cpu->registers[operandA] = operandB;
      cpu->PC += 3;
      break;
    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      break; 
    case PRN:
      printf("%d\n", cpu->registers[operandA]);
      cpu->PC += 2;
      break;
    case POP:
      //if sp is R7, empty stack
      cpu_pop(cpu, operandA);
      break;  
    case PUSH:
      //cpu->ram[address] = data;
      cpu_push(cpu, operandA);
      break;
    case RET:
      cpu->PC = cpu->ram[cpu->registers[SP]];
      cpu->registers[SP]++;
      break;  
    default:
      running = 0;
      printf("%s\n", "THIS SHOULDN'T PRINT. I MUST FIX MY CODE.");
    }
  }
}

void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  cpu->FL = 0;
  memset(cpu->ram, 0, sizeof cpu->ram);             //8
  memset(cpu->registers, 0, sizeof cpu->registers); //256
  // TODO: Zero registers and RAM
  cpu->registers[SP]= ADDR_EMPTY_STACK; //244
}
