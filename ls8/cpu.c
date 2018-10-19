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


/*
  Flags
  The flags register FL holds the current flags status. These flags can change based on the operands given to the CMP opcode.
  The register is made up of 8 bits. If a particular bit is set, that flag is "true".
  FL bits: 00000LGE

  L Less-than: during a CMP, set to 1 if registerA is less than registerB, zero otherwise.
  G Greater-than: during a CMP, set to 1 if registerA is greater than registerB, zero otherwise.
  E Equal: during a CMP, set to 1 if registerA is equal to registerB, zero otherwise.
*/
      case CMP:
        // Compare the values in two registers.
        if(cpu->reg[operandA & 7] == cpu->reg[operandB & 7]) {
          cpu->FL = 0b00000001;
        }
        else if (cpu->reg[operandA & 7] > cpu->reg[operandB & 7]) {
          cpu->FL = 0b00000010;
        }
        else if (cpu->reg[operandA & 7] < cpu->reg[operandB & 7]) {
          cpu->FL = 0b00000100;
        }
        break;

      case JMP:
        // Jump to the address stored in the given register.
        // Set the `PC` to the address stored in the given register.
        cpu->PC = cpu->reg[operandA & 7];
        instruction_set_pc = 0;
        break;

      case JEQ:
        // If `equal` flag is set (true), jump to the address stored in the given register.
        if (cpu->FL == 0b00000001) {
          cpu->PC = cpu->reg[operandA & 7];
          instruction_set_pc = 0;
        }
        break;

      case JNE:
        // If `E` flag is clear (false, 0), jump to the address stored in the given register.
        if (cpu->FL == 0b00000000) {
          cpu->PC = cpu->reg[operandA & 7];
          instruction_set_pc = 0;
        }
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
  cpu->FL = 0;

  cpu->reg[STACK_POINTER] = 0xF4; // <-- Where SP is on an empty stack
}
