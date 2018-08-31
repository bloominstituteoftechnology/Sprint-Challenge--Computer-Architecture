#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  // the adress tends to be cpu->PC
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char val)
{
  cpu->ram[address] = val;
}

void handleLD(struct cpu *cpu)
{
  // Operands
  int regA = cpu_ram_read(cpu, cpu->PC + 1);
  int regB = cpu_ram_read(cpu, cpu->PC + 2);
  // Operation
  // -- Get mem address from register
  // -- Then retrieve value and assign to regA
  unsigned char mem = cpu->registers[regB];
  cpu->registers[regA] = cpu_ram_read(cpu, mem);
  // PC
  cpu->PC += 3;
}

void handleLDI(struct cpu *cpu)
{
  int regAddr;
  unsigned char val;
  // Get dem operands
  regAddr = cpu_ram_read(cpu, cpu->PC + 1);
  val = cpu_ram_read(cpu, cpu->PC + 2);
  // Do dat operation
  cpu->registers[regAddr] = val;
  // Move dat program counter
  cpu->PC += 3;
}

void handleST(struct cpu *cpu)
{
  // Operands
  int regA = cpu_ram_read(cpu, cpu->PC + 1);
  int regB = cpu_ram_read(cpu, cpu->PC + 2);
  // Operation
  unsigned char val = cpu->registers[regB];
  int mem = cpu->registers[regA];
  cpu_ram_write(cpu, mem, val);
  // PC
  cpu->PC += 3;
}

void handlePRN(struct cpu *cpu)
{
  int addrPRN;
  // Operands
  addrPRN = cpu_ram_read(cpu, cpu->PC + 1);
  // Operation
  printf("%i\n", cpu->registers[addrPRN]);
  // PC
  cpu->PC += 2;
}

void handlePRA(struct cpu *cpu)
{
  // Operand
  int addrPRA = cpu_ram_read(cpu, cpu->PC + 1);
  // Operation
  printf("%c", cpu->registers[addrPRA]);
  // PC
  cpu->PC += 2;
}

void handlePUSH(struct cpu *cpu)
{
  // Get operands
  int regAddr = cpu_ram_read(cpu, cpu->PC + 1);
  // printf("handlePop->regAddr: %i\n", regAddr);
  // Operation
  unsigned char val = cpu->registers[regAddr];
  // printf("val: %i\n", val);
  cpu_ram_write(cpu, --cpu->SP, val);
  // PC
  cpu->PC += 2;
}

void handlePOP(struct cpu *cpu)
{
  // Get operands
  int regAddr = cpu_ram_read(cpu, cpu->PC + 1);
  // printf("handlePop->regAddr: %i\n", regAddr);
  // Operate
  // printf("read result: %i\n", cpu_ram_read(cpu, cpu->SP++));
  cpu->registers[regAddr] = cpu_ram_read(cpu, cpu->SP++);
  // PC
  cpu->PC += 2;
}

void handleCALL(struct cpu *cpu)
{
  // Operand
  int regAddr = cpu_ram_read(cpu, cpu->PC + 1);
  // Operate and PC
  unsigned char nextInstruction = cpu->PC + 2;
  cpu_ram_write(cpu, --cpu->SP, nextInstruction);
  cpu->PC = (int) cpu->registers[regAddr];
}

void handleRET(struct cpu *cpu)
{
  // Operate (and PC. Its operation is to set the PC back).
  cpu->PC = (int) cpu_ram_read(cpu, cpu->SP++);
}

void handleJMP(struct cpu *cpu)
{
  // Operand
  int regAddr = cpu_ram_read(cpu, cpu->PC + 1);
  // Operate and PC
  cpu->PC = (int) cpu->registers[regAddr];
}

void handleJEQ(struct cpu *cpu)
{
  // Operand
  int regAddr = cpu_ram_read(cpu, cpu->PC + 1);
  // Operate
  int prevCMPwasEQL = cpu->FL & 0b1;
  if (prevCMPwasEQL ) cpu->PC = (int) cpu->registers[regAddr];
  else                cpu->PC += 2;
}

void handleJNE(struct cpu *cpu)
{
  // Operand
  int regAddr = cpu_ram_read(cpu, cpu->PC + 1);
  // Operate and PC
  int prevCMPwasEQL =  cpu->FL & 0b1;
  if (!prevCMPwasEQL)  cpu->PC = (int) cpu->registers[regAddr];
  else                 cpu->PC += 2; 
}

/* --- ALU Operations --- */
void handleALU_ADD(struct cpu *cpu, int regA, int regB)
{
  // Operands
  unsigned char termA = cpu->registers[regA];
  unsigned char termB = cpu->registers[regB];
  // Operation
  cpu->registers[regA] = termA + termB;
  // PC
  cpu->PC += 3;
}

void handleALU_MUL(struct cpu *cpu, int regA, int regB)
{
  // Operands
  unsigned char prodA = cpu->registers[regA];
  unsigned char prodB = cpu->registers[regB];
  // Operation
  cpu->registers[regA] = prodA * prodB;
  // PC
  cpu->PC += 3;
}

void handleALU_INC(struct cpu *cpu, int regA)
{
  // Operand
  unsigned char incremented = cpu->registers[regA];
  // Operation
  cpu->registers[regA] = ++incremented;
  // PC
  cpu->PC += 2;
}

void handleALU_DEC(struct cpu *cpu, int regA)
{
  // Operand
  unsigned char decremented = cpu->registers[regA];
  // Operation
  cpu->registers[regA] = --decremented;
  // PC
  cpu-> PC += 2;
}

void handleALU_CMP(struct cpu *cpu, int regA, int regB)
{
  // Operands
  unsigned char a = cpu->registers[regA];
  unsigned char b = cpu->registers[regB];
  // Operation
  cpu->FL = 0b00000000;
  if      (a < b) cpu->FL += 0b100;
  else if (a > b) cpu->FL += 0b10;
  else            cpu->FL += 0b1;
  // PC
  cpu->PC += 3;
}
