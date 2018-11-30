#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6
#define SP 7

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char MAR){
  return cpu->ram[MAR];
}

void cpu_ram_write(struct cpu *cpu, unsigned char MAR, unsigned char MDR){
  cpu->ram[MAR] = MDR;
}
/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  int address = 0;

  FILE *fp = fopen(filename, "r");
  char line[256];

  if (fp == NULL){
    printf("Cannot open %s. Invalid file name or path.\n", fp);
    exit(2);
  }

  while(fgets(line, sizeof(line), fp) != NULL){
    if (line[0] == "#" || line[0] == "\n"){
      continue;
    }
    unsigned char command;
    command = strtoul(line, NULL, 2);
    cpu_ram_write(cpu, address, command);
    // printf("%d\n", cpu->ram[address]);
    address++;
  }
  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char operandA, unsigned char operandB)
{
  switch (op) {
    case ALU_MUL:
      cpu->reg[operandA] *= cpu->reg[operandB];
      break;
    case ALU_ADD:
      cpu->reg[operandA] += cpu->reg[operandB];
    // TODO: implement more ALU ops
  }
}

unsigned char pop_handler(struct cpu *cpu, unsigned char operandA){
  cpu->reg[operandA] = cpu_ram_read(cpu, cpu->reg[SP]);
  cpu->reg[SP]++;
  return cpu->reg[operandA];
}

void push_handler(struct cpu *cpu, unsigned char operandA){
  cpu->reg[SP]--; //grows downward, larger address --> smaller address
  cpu_ram_write(cpu, cpu->reg[SP], cpu->reg[operandA]);
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  int i;

  // for(i = 0; i<30; i++){
  //   printf("cpu ram[%d] = %d\n", i, cpu->ram[i]);
  // }

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
      case LDI:
        cpu->reg[operandA] = operandB;
        cpu->PC += move_pc;
        break;
      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        cpu->PC += move_pc;
        break;
      case HLT:
        running = 0;
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        cpu->PC += move_pc;
        break;
      case POP:
        pop_handler(cpu, operandA);
        cpu->PC += move_pc;
        break;
      case PUSH:
        push_handler(cpu, operandA);
        cpu->PC += move_pc;
        break;
      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        cpu->PC += move_pc;
        break;
      case CALL:
        cpu->reg[SP]--;
        cpu_ram_write(cpu, cpu->reg[SP], cpu->PC+2);
        cpu->PC = cpu->reg[operandA];
        break;
      case RET:
        cpu->PC = pop_handler(cpu, operandA);
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
  // TODO: Zero registers and RAM
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));
}

