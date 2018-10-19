#include "cpu.h"

#define DATA_LEN 6

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char memory_ADDRESS_registry)
{
  return cpu->ram[memory_ADDRESS_registry];
}

void cpu_ram_write(struct cpu *cpu, unsigned char memory_ADDRESS_registry, unsigned char memory_DATA_registry)
{
  cpu->ram[memory_ADDRESS_registry] = memory_DATA_registry;
}

void cpu_push(struct cpu *cpu, unsigned char value)
{
  cpu->reg[STACK_POINTER]--;
  cpu->ram[cpu->reg[STACK_POINTER]] = value;
}

unsigned char cpu_pop(struct cpu *cpu)
{
  unsigned char value = cpu->ram[cpu->reg[STACK_POINTER]];
  cpu->reg[STACK_POINTER]++;
  return value;
}

void cpu_load(char *filename, struct cpu *cpu)
{
  FILE *fp;
  char line[1024];
  unsigned char address = 0x00; // Where programs start getting loaded
  if ((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Cannot open file %s/n", filename);
    exit(2);
  }
  while (fgets(line, sizeof line, fp) != NULL) {
    char *endpointer;
    unsigned char byte = strtoul(line, &endpointer, 2);
    if (endpointer == line) {
      continue;
    }
    cpu_ram_write(cpu, address++, byte);
  }
  fclose(fp);
} 

void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->reg[regA] = cpu->reg[regA] * cpu->reg[regB];
      break;
  }
}

void cpu_run(struct cpu *cpu)
{
  int running = 1; 
  while (running) {
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, (cpu->PC+1) & 0xff);
    unsigned char operandB = cpu_ram_read(cpu, (cpu->PC+2) & 0xff);

    int instruction_set_pc = (IR >> 6) + 1;

    printf("TRACE: --> pc:%02X | ir:%02X | opA:%02X | opB:%02X | reg:", cpu->PC, IR, operandA, operandB);
    for (int i = 0; i < 8; i++) {
      printf(" %02X", cpu->reg[i]);
    }
    printf(" |\n");

    switch(IR)
    {
      case LDI:
        cpu->reg[operandA] = operandB;
        break;

      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        break;

      case HLT:
        running = 0;
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;

      case PUSH:
        cpu_push(cpu, cpu->reg[operandA]);
        break;

      case POP:
        cpu->reg[operandA] = cpu_pop(cpu);
        break;


      default:
        fprintf(stderr, "ERROR: Unknown Instruction\n from cpu_run() in cpu.c\n PC: %02x\n IR: %02X\n", cpu->PC, IR);
        exit(3);
    } 

    cpu->PC += instruction_set_pc;
    
  }
} 

void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->reg, 0, sizeof cpu->reg);

  cpu->reg[STACK_POINTER] = 0xF4; // <-- Where SP is on an empty stack
}
