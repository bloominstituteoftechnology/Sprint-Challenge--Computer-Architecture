#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include "stdlib.h"

#define DATA_LEN 6


unsigned char cpu_ram_read(struct cpu *cpu, int storage) {
  return cpu->ram[storage];
}

void cpu_ram_write(struct cpu *cpu, int storage, unsigned char value) {
  cpu->ram[storage] = value;
}
/**
 * Load the binary bytes from a .ls8 source file into a RAM array.
 */
void cpu_load(struct cpu *cpu, char *file)
{
  //char data[DATA_LEN] = {
    // From print8.ls8
    //0b10000010, // LDI R0,8
    //0b00000000,
    //0b00001000,
    //0b01000111, // PRN R0
    //0b00000000,
    //0b00000001  // HLT
  //};

  //int address = 0;

  //for (int i = 0; i < DATA_LEN; i++) {
    //cpu->ram[address++] = data[i];
  //}

  // TODO: Replace this with something less hard-coded
  FILE *fp;
  char line[500];
  int address = 0; 
  fp = fopen(file, "r"); 

  if (fp == NULL){
  printf("Cannot Open File.\n");
  exit(2);
  }

  while(fgets(line, sizeof(line), fp) != NULL) {
  cpu->ram[address] = strtoul(line, NULL, 2); 
  address++; 
  }
    fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu-> registers[regA] *= cpu-> registers[regB];
      break;

    // TODO: implement more ALU ops
      case ALU_ADD:
      cpu->registers[regA] = cpu->registers[regB] + cpu->registers[regA];
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
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC+1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC+2);
    unsigned char move_pc = (IR >> 6) + 1;
      switch(IR) {

      case HLT:
        running = 0;
        break;

      case LDI:
      cpu-> registers[operandA] = operandB;
        break;

      case PRN:
      printf("%d \n", cpu-> registers[operandA]); 
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB); 
        break;

      case POP:
        cpu->registers[operandA] = cpu_ram_read(cpu, cpu->registers[SP]);
        cpu->registers[SP]++;
        break;

      case PUSH:
        cpu->registers[SP]--;
        cpu_ram_write(cpu, cpu->registers[SP], cpu->registers[operandA]);
        break;

      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        break;

      case CALL:
        cpu->registers[SP]--;
        cpu_ram_write(cpu, cpu->registers[SP], cpu->PC + move_pc);
        cpu->PC = cpu->registers[operandA];
        cpu->add_pc = 1;
        break;

      case RET:
        cpu->PC = cpu_ram_read(cpu, cpu->registers[SP]);
        cpu->registers[SP]++;
        cpu->add_pc = 1;
        break;

      case JMP:
        cpu->PC = cpu->registers[operandA];

      case CMP:
        if (cpu->registers[operandA] < cpu->registers[operandB]) {
          cpu->add_pc = 1;
        }

        if (cpu->registers[operandA] == cpu->registers[operandB]) {
          cpu->add_pc = 2;
        }

        if (cpu->registers[operandA] > cpu->registers[operandB]) {
          cpu->add_pc = 4;
        }
        cpu->PC += move_pc;
        break;

      case JEQ:
        if (cpu->add_pc == 1) {
          cpu->PC = cpu->registers[operandA];
        }

        break;

      case JNE:
        if (cpu->add_pc != 1) {
          cpu->PC = cpu->registers[operandA];
        }
        break;
        
     default:
      printf("default\n");
        break;

    }
    if (cpu->add_pc == 0) {
      cpu->PC += move_pc;
    }
    else {
      cpu->add_pc = 0;
      continue;
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
  cpu->registers[SP] = 0xF4;
  cpu->add_pc = 0;
  // TODO: Zero registers and RAM
  memset(cpu->ram, 0, sizeof(cpu->ram));
  memset(cpu->registers, 0, sizeof(cpu->registers));

}

