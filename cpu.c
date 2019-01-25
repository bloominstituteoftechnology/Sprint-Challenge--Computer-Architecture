#include "cpu.h"
#include <string.h>
#include <stdio.h>

#define DATA_LEN 6


void cpu_ram_write(struct cpu *cpu, unsigned char value, unsigned char address)
{
  cpu->ram[address] = value;
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{

  int address = 0;

  FILE *fp;
  char line[1024];

  if ((fp = fopen("./sprint.ls8", "r")) == NULL) {
    fprintf(stderr, "Cannot open sprint.ls8\n");
    exit(1);
  }

  while (fgets(line, sizeof(line), fp) != NULL){
    char *ptr;
    unsigned char ret = strtol(line, &ptr, 2);


    if (ptr == line){
      continue;
    }

    cpu->ram[address++] = ret;
  }
}



void push(unsigned char val, struct cpu *cpu)
{
  cpu->SP--;
  cpu_ram_write(cpu, val, cpu->SP);
}

unsigned char pop(struct cpu *cpu)
{
  return cpu_ram_read(cpu, cpu->SP++);
}


void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  unsigned char operandA = NULL;
  unsigned char operandB = NULL;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // unsigned char instruction ...
    // 2. Figure out how many operands this next instruction requires
    unsigned char instruction = cpu_ram_read(cpu, cpu->PC);
    unsigned int num_operands = instruction >> 6;
    // 3. Get the appropriate value(s) of the operands following this instruction

    if (num_operands == 2){
      operandA = cpu_ram_read(cpu, (cpu->PC + 1) & 0xff);
      operandB = cpu_ram_read(cpu, (cpu->PC + 2) & 0xff);
    } else if (num_operands == 1){
      operandA = cpu_ram_read(cpu, (cpu->PC + 1) & 0xff);
    }

    // 4. switch() over it to decide on a course of action.
    switch (instruction) {
      case HLT:
        running = 0;
        break;
      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        break;
      case LDI:
        cpu->reg[operandA] = operandB;
        break;
      case CALL:
        push(cpu->PC + 1, cpu);
        cpu->PC = cpu->reg[operandA] - 2;
        break;
      case RET:
        cpu->PC = pop(cpu);
        break;
      case CMP:
        if (cpu->reg[operandA] == cpu->reg[operandB]){
          cpu->E = 1;
        } else {
          cpu->E = 0;
        }
      break;
      case JNE:
        if (cpu->E == 0){
          cpu->PC = cpu->reg[operandA] - 2;
        }
        break;
      case JEQ:
        if (!(cpu->E == 0)){
          cpu->PC = cpu->reg[operandA] - 2;
        }
        break;
      case JMP:
        cpu->PC = cpu->reg[operandA] - 2;
      break;
      default:
        break;
    }
    cpu->PC += num_operands + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = -1;
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->reg, 0, sizeof cpu->reg);
  cpu->SP = ADDR_EMPTY_STACK;
  cpu->E = 0;
}