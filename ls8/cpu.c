#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"


void cpu_push(struct cpu *cpu, unsigned char val)
{
  cpu->registers[SP]--;

  cpu->ram[cpu->registers[SP]] = val;
}

unsigned char cpu_pop(struct cpu *cpu)
{
  unsigned char val = cpu->ram[cpu->registers[SP]];

  cpu->registers[SP]++;

  return val;
}

void cpu_load(char *filename, struct cpu *cpu)
{
  FILE *fp;
  char line[1024];
  int address = ADDR_PROGRAM_ENTRY;


  if ((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Cannot open file %s\n", filename);
    exit(2);
  }


  while (fgets(line, sizeof line, fp) != NULL) {


    char *endchar;
    unsigned char byte = strtol(line, &endchar, 2);;


    if (endchar == line) {
      continue;
    }


    cpu->ram[address++] = byte;
  }
}


void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *registers = cpu->registers;

  unsigned char valB = registers[regB];


  switch (op) {
    case ALU_CMP:
      if (cpu ->registers[regA] == cpu ->registers[regB])
      {
        cpu -> FL = 0b001;
      }
      else if (cpu ->registers[regA] > cpu -> registers[regB])
      {
        cpu -> FL = 0b00000010;
      }
      else {
        cpu -> FL = 0b00000100;
      }
      break;

    case ALU_MUL:
      registers[regA] *= valB;
      break;
    case ALU_ADD:
      registers[regA] += valB;
      break;
  }

}

void cpu_run(struct cpu *cpu)
{

  unsigned char *registers = cpu->registers;
  unsigned char *ram = cpu->ram;

  int running = 1;

  while (running) {
    unsigned char IR = ram[cpu->PC];

    unsigned char operandA = ram[(cpu->PC + 1) & 0xff];
    unsigned char operandB = ram[(cpu->PC + 2) & 0xff];


    int instruction_set_pc = (IR >> 4) & 1;

    switch (IR) {
      case JMP:
        cpu ->PC = cpu ->registers[operandA];
        break;
        case JEQ:
          if((cpu -> FL & 1) == 1){
            cpu -> PC = cpu -> registers[operandA];
          }
          else {
            cpu -> PC += 2;
          }
          break;
        case JNE:
          if((cpu -> FL & 1) != 1)
          {
            cpu -> PC = cpu -> registers[operandA];
          }
          else {
            cpu -> PC += 2;
          }
          break;

      case CMP:
        alu(cpu,ALU_CMP,operandA,operandB);
        break;
      case LDI:
        registers[operandA] = operandB;
        break;

      case PRN:
        printf("%d\n", registers[operandA]);
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;

      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        break;

      case HLT:
        running = 0;
        break;

      case PRA:
        printf("%c\n", registers[operandA]);
        break;
      case CALL:
        cpu_push(cpu, cpu->PC + 2);
        cpu->PC = registers[operandA];
        break;

      case RET:
        cpu->PC = cpu_pop(cpu);
        break;

      case PUSH:
        cpu_push(cpu, registers[operandA]);
        break;

      case POP:
        registers[operandA] = cpu_pop(cpu);
        break;

      default:
        fprintf(stderr, "BAD FILE NAME\n");
        exit(3);
    }

    if (!instruction_set_pc) {
      cpu->PC += ((IR >> 6) & 0x3) + 1;
    }

  }
}


void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  cpu -> FL = 0;

  memset(cpu->registers, 0, sizeof cpu->registers);
  memset(cpu->ram, 0, sizeof cpu->ram);

  cpu->registers[SP] = ADDR_EMPTY_STACK;
}
