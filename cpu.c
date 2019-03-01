#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

// Reads logic stored in the ram
char cpu_ram_reader(struct cpu *cpu, unsigned char addr)
{
  return cpu->ram[addr];
}

void cpu_ram_writer(struct cpu *cpu, unsigned char value, unsigned char addr)
{
  cpu->ram[addr] = value;
}

void cpu_loader(struct cpu *cpu, char *arg)
{
  int addr = 0;
  FILE *f = fopen(arg, "r");
  char str[256];

  if (f == NULL)
  {
    printf("Invalid file\n");
    exit(1);
  }
  while (fgets(str, 256, f) != NULL)
  {

    char *end_pointer;
    unsigned char val = strtoul(str, &end_pointer, 2);
    
    if (end_pointer == str)
    {
      continue;
    }

    cpu_ram_writer(cpu, val, addr++);

  }
  // Close the cpu instruction
  fclose(f);

}


void alu(struct cpu *cpu, enum alu_operations op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MULTIPLIER:
    cpu->reg[regA] = cpu->reg[regA] * cpu->reg[regB];
    break;
  case ALU_ADDITION:
    cpu->reg[regA] = cpu->reg[regA] + cpu->reg[regB];
    break;
  case ALU_CMP_INSTRUCTION:
    if (regA == regB)
    {
      cpu->flag_equals = 1;
    }
    else
    {
      cpu->flag_equals = 0;
    }

    if (regA < regB)
    {
      cpu->flag_less_than = 1;
    }
    else
    {
      cpu->flag_less_than = 0;
    }

    if (regA > regB)
    {
      cpu->flag_greater_than = 1;
    }
    else
    {
      cpu->flag_greater_than = 0;
    }
    cpu->pc += 3;
    break;
  }
}


void ldi(struct cpu *cpu)
{
  unsigned char operand_a = cpu_ram_reader(cpu, cpu->pc + 1);
  unsigned char operand_b = cpu_ram_reader(cpu, cpu->pc + 2);
  cpu->reg[operand_a] = operand_b;
  cpu->pc += 3;
}

// PRN stage
void prn(struct cpu *cpu)
{
  unsigned char operand_a = cpu_ram_reader(cpu, cpu->pc + 1);
  unsigned char print_value = cpu->reg[operand_a];

  printf("%u \n", print_value);

  cpu->pc += 2;
}


void mul(struct cpu *cpu)
{
  unsigned char operand_a = cpu_ram_reader(cpu, cpu->pc + 1);
  unsigned char operand_b = cpu_ram_reader(cpu, cpu->pc + 2);
  alu(cpu, ALU_MULTIPLIER, operand_a, operand_b);
  cpu->pc += 3;
}


void push(struct cpu *cpu)
{

  unsigned char operand_a = cpu_ram_reader(cpu, cpu->pc + 1);

  cpu->reg[7]--;

  cpu->ram[cpu->reg[7]] = cpu->reg[operand_a];

  cpu->pc += 2;
}

void pop(struct cpu *cpu)
{
  // note: stack decrements memory address as things are added to it, 
  // like having gravity reversed
  unsigned char operand_a = cpu_ram_reader(cpu, cpu->pc + 1);

  cpu->reg[operand_a] = cpu->ram[cpu->reg[7]];

  cpu->reg[7]++;

  cpu->pc += 2;
}


void add(struct cpu *cpu)
{

  unsigned char operand_a = cpu_ram_reader(cpu, cpu->pc + 1);
  unsigned char operand_b = cpu_ram_reader(cpu, cpu->pc + 2);

  alu(cpu, ALU_ADDITION, operand_a, operand_b);
  cpu->pc += 3;

}


void call(struct cpu *cpu)
{

  unsigned char operand_a = cpu_ram_reader(cpu, cpu->pc + 1);
  unsigned char subroutine_location = cpu->reg[operand_a];
  unsigned char location_info = cpu->pc + 2;

  cpu->reg[7]--;
  cpu->ram[cpu->reg[7]] = location_info;
  cpu->pc = subroutine_location;
}

void routine_instruction(struct cpu *cpu)
{

  cpu->pc = cpu->ram[cpu->reg[7]];

  cpu->reg[7]++;
}


void cmp(struct cpu *cpu)
{
  unsigned char operand_a = cpu_ram_reader(cpu, cpu->pc + 1);
  unsigned char operand_b = cpu_ram_reader(cpu, cpu->pc + 2);
  unsigned char reg_a = cpu->reg[operand_a];
  unsigned char reg_b = cpu->reg[operand_b];
  alu(cpu, ALU_CMP_INSTRUCTION, reg_a, reg_b);
}


void jeq(struct cpu *cpu)
{
  unsigned char operand_a = cpu_ram_reader(cpu, cpu->pc + 1);
  if (cpu->flag_equals == 1)
  {
    cpu->pc = cpu->reg[operand_a];
  }
  else
  {
    cpu->pc += 2;
  }
}

void jne(struct cpu *cpu)
{
  unsigned char operand_a = cpu_ram_reader(cpu, cpu->pc + 1);
  if (cpu->flag_equals == 0)
  {
    cpu->pc = cpu->reg[operand_a];
  }
  else
  {
    cpu->pc += 2;
  }
}

void jmp(struct cpu *cpu)
{
  unsigned char operand_a = cpu_ram_reader(cpu, cpu->pc + 1);
  cpu->pc = cpu->reg[operand_a];
}

void cpu_runner(struct cpu *cpu)
{
  int running = 1;

  while (running)
  {
    unsigned char c_instr = cpu_ram_reader(cpu, cpu->pc);
    switch (c_instr)
    {
    case LDI:
      ldi(cpu);
      break;
    case PRN:
      prn(cpu);
      break;
    case MUL:
      mul(cpu);
      break;
    case PUSH:
      push(cpu);
      break;
    case POP:
      pop(cpu);
      break;
    case ADD:
      add(cpu);
      break;
    case CALL:
      call(cpu);
      break;
    case RET:
      routine_instruction(cpu);
      break;
    case CMP:
      cmp(cpu);
      break;
    case JEQ:
      jeq(cpu);
      break;
    case JNE:
      jne(cpu);
      break;
    case JMP:
      jmp(cpu);
      break;
    case HLT:
      running = 0;
      break;
    default:
      break;
    }
  }
}

void cpu_init(struct cpu *cpu)
{
  cpu->pc = 0;
  cpu->flag_equals = 0;
  cpu->flag_less_than = 0;
  cpu->flag_greater_than = 0;
  memset(cpu->ram, 0, 256 * sizeof(unsigned char));
  memset(cpu->reg, 0, 8 * sizeof(unsigned char));
} 