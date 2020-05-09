#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
    return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
    cpu->ram[address] = value;
}
void cpu_load(struct cpu *cpu, char *file)
{
  FILE *fp; 
  char line[1024];

  if ((fp = fopen(file, "r")) == NULL)
  {
    fprintf(stderr, "File not found.\n");
    exit(1);
  }

  int address = 0;

  fp = fopen(file, "r");
  while(fgets(line, sizeof(line), fp) != NULL){
    char *endptr; 
    unsigned char value;
    value = strtoul(line, &endptr, 2);

    if(endptr == line){
      continue; 
    }

    cpu->ram[address++] = value;
  }
}

void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_ADD:
      cpu->registers[regA] += cpu->registers[regB];
      break;
    case ALU_AND:
      cpu->registers[regA] = cpu->registers[regA] & cpu->registers[regB];
      break;
    case ALU_CMP:
      if (cpu->registers[regA] == cpu->registers[regB])
      {
        cpu->E = 1;
        break;
      }
      else if (cpu->registers[regA] > cpu->registers[regB])
      {
        cpu->G = 1;
        break;
      }
      else
      {
        cpu->L = 1;
        break;
      }
    case ALU_MOD:
      cpu->registers[regA] %= cpu->registers[regB];
      break;
    case ALU_MUL:
      cpu->registers[regA] *= cpu->registers[regB];
      break;
    case ALU_NOT:
      cpu->registers[regA] = ~cpu->registers[regA];
      break;
    case ALU_OR:
      cpu->registers[regA] = cpu->registers[regA] | cpu->registers[regB];
      break;
    case ALU_SHL:
      cpu->registers[regA] = cpu->registers[regA] << cpu->registers[regB];
      break;
    case ALU_SHR:
      cpu->registers[regA] = cpu->registers[regA] >> cpu->registers[regB];
      break;
    case ALU_XOR:
      cpu->registers[regA] = cpu->registers[regA] ^ cpu->registers[regB];
      break;
  }
}

void cpu_run(struct cpu *cpu)
{
  int running = 1; 
  
  while (running) {
    unsigned char ir = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA;
    unsigned char operandB;
    int next_line = 1;

    if (ir & 0x80)
    {
      operandA = cpu->ram[(cpu->PC + 1) & 0xff];
      operandB = cpu->ram[(cpu->PC + 2) & 0xff];
      next_line = 3;
    }
    else if (ir & 0x40)
    {
      operandA = cpu->ram[(cpu->PC + 1) & 0xff];
      next_line = 2;
    }

    switch (ir)
    {
      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        break;
      case AND:
        alu(cpu, ALU_AND, operandA, operandB);
        break;
      case CALL:
        cpu->ram[--cpu->registers[7]] = cpu->PC + next_line;
        cpu->PC = cpu->registers[operandA];
        continue;
      case RET:
        cpu->PC = cpu->ram[cpu->registers[7]++];
        continue;
      case CMP:
        alu(cpu, ALU_CMP, operandA, operandB);
        break;
      case HLT:
        running = 0;
        break;
      case JEQ:
        if (cpu->E == 1)
        {
          cpu->PC = cpu->registers[operandA];
          continue;
        }
        break;
      case JMP:
        cpu->PC = cpu->registers[operandA];
        continue;
      case JNE:
        if (cpu->E != 1)
        {
          cpu->PC = cpu->registers[operandA];
          continue;
        }
        break;
      case LDI:
        cpu->registers[operandA] = operandB;
        break;
      case MOD:
        if (cpu->registers[operandB] == 0)
        {
          printf("Dividing by zero is not allowed.\n");
          running = 0;
        }
        else
        {
          alu(cpu, ALU_MOD, operandA, operandB);
        }
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;
      case NOT:
        alu(cpu, ALU_AND, operandA, operandB);
        break;
      case OR:
        alu(cpu, ALU_AND, operandA, operandB);
        break;
      case POP:
        cpu->registers[operandA] = cpu_ram_read(cpu, cpu->registers[7]);
        cpu->ram[cpu->registers[7]++] = 0x00;
        break;
      case PRN:
        printf("%d\n", cpu->registers[operandA]);
        break;
      case PUSH:
        cpu_ram_write(cpu, --cpu->registers[7], cpu->registers[operandA]);
        break;
      case SHL:
        alu(cpu, ALU_SHL, operandA, operandB);
        break;
      case SHR:
        alu(cpu, ALU_SHR, operandA, operandB);
        break;
      case ST:
        cpu_ram_write(cpu, operandA, cpu->registers[operandB]);
        break;
      case XOR:
        alu(cpu, ALU_AND, operandA, operandB);
        break;
      default:
        break;
    }
    cpu->PC = cpu->PC + next_line;
  }
}

void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  cpu->FL = 0;
  cpu->E = 0;
  cpu->L = 0;
  cpu->G = 0;
  memset(cpu->ram, 0, 8 * sizeof(unsigned char));
  memset(cpu->registers, 0, 256 * sizeof(unsigned char));
  cpu->registers[7] = 0xF4;
}