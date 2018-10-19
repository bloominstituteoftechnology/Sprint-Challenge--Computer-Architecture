#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

unsigned char cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  return cpu->ram[address] = value;
}



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

  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }

  char pathname[30] = "./examples/";
  char line[1024];
  int address = 0;
  strcat(pathname, filename);
  strcat(pathname, ".ls8");
  FILE *fileptr;
  fileptr = fopen(pathname, "r");

  if (fileptr == NULL)
  {
    puts("couldnt open file");
    exit(0);
  }


  while (fgets(line, sizeof line, fileptr) != NULL)
  {
    if (line[0] == '\n' || line[0] == '#') // if its a new line or a comment, skip it.
    {
      continue;
    }
    unsigned char longline;
    longline = strtoul(line, NULL, 2);
    cpu->ram[address++] = longline;
  }
  fclose(fileptr);  

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
    break;
  case ALU_ADD:
    cpu->registers[regA] += cpu->registers[regB];
    break;
  case ALU_CMP:
        puts("ALU_CMP COMMAND\n");
        if (cpu->registers[regA] == cpu->registers[regB]) {
          cpu->fl = 0b0000001;
        } else if (regA < regB) {
          cpu->fl = 0b0000100;
        } else {
          cpu->fl = 0b000010;
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
  unsigned char i,IR, operandA, operandB;
  char next = 1;

    while (running) {
    IR = cpu_ram_read(cpu, cpu->pc);
    operandA = cpu_ram_read(cpu, (cpu->pc+1));
    operandB = cpu_ram_read(cpu, (cpu->pc+2));
    int shiftIndex = (IR >> 6) + 1;
    int flags;
    printf("TRACE: %02X: %02X %02X %02X\n", cpu->pc, IR, operandA, operandB);
    
    for (int i = 0; i < 8; i++)
    {
      printf(" %02X", cpu->registers[i]);
    }
    printf("\n");

    switch(IR) {
    case LDI:
      cpu->registers[operandA] = operandB;
      break;
    case PRN:
      printf("%d\n", cpu->registers[operandA]);
      break;
    case MUL:
      puts("multiplying");
      alu(cpu, ALU_MUL, operandA, operandB);
      // printf("RAM value at F4: %02X\n", cpu_ram_read(cpu, 0xF4));
      // printf("RAM value at F3: %02X\n", cpu_ram_read(cpu, 0xF3));
      // printf("RAM value at F2: %02X\n", cpu_ram_read(cpu, 0xF2));
      break;
    case CMP:
      puts("(case CMP) - sending to ALU\n");
      alu(cpu, ALU_CMP, operandA, operandB);
      printf("fl value after alu - %d\n", cpu->fl);
      // printf("RAM value at F4: %02X\n", cpu_ram_read(cpu, 0xF4));
      // printf("RAM value at F3: %02X\n", cpu_ram_read(cpu, 0xF3));
      // printf("RAM value at F2: %02X\n", cpu_ram_read(cpu, 0xF2));
      break;
    case HLT:
      puts("halting");
      // printf("RAM value at F4: %02X\n", cpu_ram_read(cpu, 0xF4));
      // printf("RAM value at F3: %02X\n", cpu_ram_read(cpu, 0xF3));
      // printf("RAM value at F2: %02X\n", cpu_ram_read(cpu, 0xF2));
      running = 0;
      break;
    case ADD:
      alu(cpu, ALU_ADD, operandA, operandB);
      break;
    case PUSH:
      i = --cpu->registers[7];
      cpu->ram[i] = cpu->registers[operandA];
      // puts("pushing");
      // printf("RAM value at F4: %02X\n", cpu_ram_read(cpu, 0xF4));
      // printf("RAM value at F3: %02X\n", cpu_ram_read(cpu, 0xF3));
      // printf("RAM value at F2: %02X\n", cpu_ram_read(cpu, 0xF2));
      break;
    case POP:
      // puts("popping");
      i = cpu->registers[7];
      cpu->registers[operandA] = cpu->ram[i];
      cpu->registers[7]++;

      // printf("RAM value at F4: %02X\n", cpu_ram_read(cpu, 0xF4));
      // printf("RAM value at F3: %02X\n", cpu_ram_read(cpu, 0xF3));
      // printf("RAM value at F2: %02X\n", cpu_ram_read(cpu, 0xF2));
      break;
    case JMP:
      cpu->pc = cpu->registers[operandA];
      shiftIndex = 0;
      
      break;
    case JEQ:
    puts("JEQ-----");
      if (cpu->fl == 1) {
        printf("JEQ...jumping to %d\n", operandA);
        cpu->pc = cpu->registers[operandA];
        shiftIndex = 0;
      } else if (cpu->fl == 0b00000010) {
        puts("regA > regB, sorry.\n");
        break;
      } else {
        puts("regA < regB, sorry.\n");
        
        break;
      }
      break;

    case JNE:
      puts("JNE-----");
      printf("%d", cpu->fl);
      if (cpu->fl == 0b00000010 || cpu->fl == 0b00000100) {
        cpu->pc = cpu->registers[operandA];
        shiftIndex = 0;
        break;
      }
        

    default:
      break;
    }
    if (next)
    {
      cpu->pc += shiftIndex;
    }
  
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers

  // TODO: Zero registers and RAM
  cpu->pc = 0;

  memset(cpu->ram, 0, sizeof cpu-> ram);
  memset(cpu->registers, 0, sizeof cpu->registers);
  cpu->registers[7] = 0xF4;
}
