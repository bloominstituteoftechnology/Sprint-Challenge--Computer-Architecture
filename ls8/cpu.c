#include <stdio.h>
#include <string.h>
#include "cpu.h"
#include <stdlib.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, int index) {
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, int index, unsigned char value) {
  cpu->ram[index] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *argv[])
{
  FILE *fp = fopen(argv[1], "r");
  char line [1024];
  unsigned char address = 0;

  if(fp == NULL) {
    fprintf(stderr, "Couldn't open the file %s\n", argv[1]);
    exit(2);
  }

  while(fgets(line, sizeof line, fp) != NULL){
    unsigned char bc = strtoul(line, NULL, 2);

    if(line[0] == '\n' || line[0] == '#'){
      continue;
    }

    cpu_ram_write(cpu, address++, bc);
  }

  fclose(fp);

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
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      cpu->registers[regA] *= cpu->registers[regB];
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


  while (running) {
      unsigned char IR = cpu_ram_read(cpu, cpu->PC); 
      unsigned char operandA = cpu_ram_read(cpu, (cpu->PC + 1)) & 0xff;
      unsigned char operandB = cpu_ram_read(cpu, (cpu->PC + 2)) & 0xff;
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.

    printf("TRACE: %02X | %02X %02X %02X | ", cpu->PC, IR, operandA, operandB);

    for(int i = 0; i < 8; i++) {
      printf("%02X ", cpu->registers[i]);
    }

    printf("\n");

    switch(IR) {
      case LDI:
        printf("LDI\n");
        cpu->registers[operandA] = operandB;
        cpu->PC += 3;
        break;

      case PRN:
        printf("PRN\n");
        printf("%d\n", cpu->registers[operandA]);
        cpu->PC += 2;
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        cpu->PC += 3;
        break;

      case PUSH:
        cpu->registers[7]--;
        cpu_ram_write(cpu, cpu->registers[7], cpu->registers[operandA]);
        cpu->PC += 2;
        break;

      case POP:
        cpu->registers[operandA] = cpu_ram_read(cpu, cpu->registers[7]);
        cpu->registers[7]++;
        cpu->PC += 2;
        break;

      case CALL:
        cpu->registers[7]--;
        cpu_ram_write(cpu, cpu->registers[7], cpu->PC + 2);
        cpu->PC = cpu->registers[operandA];
        cpu->PC += 3;
        break;

      case RET:
        cpu->PC = cpu_ram_read(cpu, cpu->registers[7]);
        cpu->registers[7]++;
        break;

      case CMP:
        printf("CMP\n");
        if(cpu->registers[operandA] == cpu->registers[operandB]){
          cpu->FL = 0b00000001;
        }
        else if(cpu->registers[operandA] < cpu->registers[operandB]){
          cpu->FL = 0b00000100;
        }
        else if(cpu->registers[operandA] > cpu->registers[operandB]){
          cpu->FL = 0b00000010;
        }
        cpu->PC += 3;
        break;

      case JMP:
        printf("JMP\n");
        cpu->PC = cpu->registers[operandA];
        break;

      case JEQ:
        printf("JEQ\n");
        if(cpu->FL == 0b00000001){
          cpu->PC = cpu->registers[operandA];
        }
        else {
          cpu->PC += 2;
        }
        break;

      case JNE:
        printf("JNE\n");
        if(cpu->FL == 0){
          cpu->PC = cpu->registers[operandA];
        }
        else {
          cpu->PC += 2;
        }
        break;

      case HLT:
        printf("Halting...\n");
        cpu->PC += 1;
        running = 0;
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
  // TODO: Zero registers and RAM
  cpu->PC = 0;
  cpu->FL = 0;
  memset(cpu->registers, 0, sizeof(cpu->registers));
  memset(cpu->ram, 0, sizeof(cpu->ram));
}
