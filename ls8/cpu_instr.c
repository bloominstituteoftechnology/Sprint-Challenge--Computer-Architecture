#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "cpu_instr.h"

// TODO: Try variadic functions to reduce number of unused parameters

/**
 * CPU Instruction: Adds values in regA and regB and stores result in regA
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register A.
 * @param opB {unsigned char} Operand B: register B.
 */
void handle_ADD(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  alu(cpu, ALU_ADD, opA, opB);
}

/**
 * CPU Instruction: Performs a bitwise-AND on regA and regB and stores result in regA
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register A.
 * @param opB {unsigned char} Operand B: register B.
 */
void handle_AND(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  alu(cpu, ALU_AND, opA, opB);
}

/**
 * CPU Instruction: Calls subroutine stored at address in register
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register.
 * @param opB {unsigned char} Operand B: --
 */
void handle_CALL(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  cpu->registers[TMP] = cpu->pc + 1;
  handle_PUSH(cpu, TMP, '\0');
  cpu->pc = cpu->registers[opA];
  get_next_instruction(cpu);
}

/**
 * CPU Instruction: Compares the values in regA and regB and sets flag
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register A.
 * @param opB {unsigned char} Operand B: register B.
 */
void handle_CMP(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  alu(cpu, ALU_CMP, opA, opB);
}

/**
 * CPU Instruction: Decrements the value in regA
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register A.
 * @param opB {unsigned char} Operand B: register B.
 */
void handle_DEC(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  alu(cpu, ALU_DEC, opA, opB);
}

/**
 * CPU Instruction: Divides values in regA and regB and stores result in regA
 * 
 * Issues error and exits program if regB is 0
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register A.
 * @param opB {unsigned char} Operand B: register B.
 */
void handle_DIV(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  if (cpu->registers[opB] == 0)
  {
    printf("error: dividing by zero");
    exit(1);
  }
  alu(cpu, ALU_DIV, opA, opB);
}

/**
 * CPU Instruction: Increments the value in regA
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register A.
 * @param opB {unsigned char} Operand B: register B.
 */
void handle_INC(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  alu(cpu, ALU_INC, opA, opB);
}

/**
 * CPU Instruction: Set interrupt status register for bit in given register
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register.
 * @param opB {unsigned char} Operand B: --
 */
void handle_INT(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  cpu->registers[IS] = cpu->registers[IS] | (1 << cpu->registers[opA]);
}

/**
 * CPU Instruction: 
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: --
 * @param opB {unsigned char} Operand B: --
 */
void handle_IRET(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  handle_POP(cpu, 6, '\0');
  handle_POP(cpu, 5, '\0');
  handle_POP(cpu, 4, '\0');
  handle_POP(cpu, 3, '\0');
  handle_POP(cpu, 2, '\0');
  handle_POP(cpu, 1, '\0');
  handle_POP(cpu, 0, '\0');

  handle_POP(cpu, TMP, '\0');
  cpu->fl = cpu->registers[TMP];
  handle_POP(cpu, TMP, '\0');
  cpu->pc = cpu->registers[TMP];

  handle_POP(cpu, IM, '\0');
}

/**
 * CPU Instruction: Jumps to address in register if equal flag is set
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register.
 * @param opB {unsigned char} Operand B: --
 */
void handle_JEQ(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  if (cpu->fl == E)
    cpu->pc = cpu->registers[opA];
  else
    cpu->pc++;
}

/**
 * CPU Instruction: Jumps to address in register if greater than or equal
 * flag is set
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register.
 * @param opB {unsigned char} Operand B: --
 */
void handle_JGE(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  if (cpu->fl == G || cpu->fl == E)
    cpu->pc = cpu->registers[opA];
  else
    cpu->pc++;
}

/**
 * CPU Instruction: Jumps to address in register if greater than flag is set
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register.
 * @param opB {unsigned char} Operand B: --
 */
void handle_JGT(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  if (cpu->fl == G)
    cpu->pc = cpu->registers[opA];
  else
    cpu->pc++;
}

/**
 * CPU Instruction: Jumps to address in register if less than or equal flag
 * is set
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register.
 * @param opB {unsigned char} Operand B: --
 */
void handle_JLE(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  if (cpu->fl == L || cpu->fl == E)
    cpu->pc = cpu->registers[opA];
  else
    cpu->pc++;
}

/**
 * CPU Instruction: Jumps to address in register if less than flag is set
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register.
 * @param opB {unsigned char} Operand B: --
 */
void handle_JLT(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  if (cpu->fl == L)
    cpu->pc = cpu->registers[opA];
  else
    cpu->pc++;
}

/**
 * CPU Instruction: Jumps to address in register
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register.
 * @param opB {unsigned char} Operand B: --
 */
void handle_JMP(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  cpu->pc = cpu->registers[opA];
}

/**
 * CPU Instruction: Jumps to address in register if equal flag is clear
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register.
 * @param opB {unsigned char} Operand B: --
 */
void handle_JNE(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  if (cpu->fl != E)
    cpu->pc = cpu->registers[opA];
  else
    cpu->pc++;
}

/**
 * CPU Instruction: Sets regA to value of memory address stored in regB
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register A.
 * @param opB {unsigned char} Operand B: register B.
 */
void handle_LD(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  cpu->mar = cpu->registers[opB];
  cpu_ram_read(cpu);
  cpu->registers[opA] = cpu->mdr;
}

/**
 * CPU Instruction: Sets value of register to an integer
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register.
 * @param opB {unsigned char} Operand B: immediate.
 */
void handle_LDI(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  cpu->registers[opA] = opB;
}

/**
 * CPU Instruction: Divides values in regA and regB and stores remainder in regA
 * 
 * Issues error and exits program if regB is 0
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register A.
 * @param opB {unsigned char} Operand B: register B.
 */
void handle_MOD(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  if (cpu->registers[opB] == 0)
  {
    printf("error: dividing by zero");
    exit(1);
  }
  alu(cpu, ALU_MOD, opA, opB);
}

/**
 * CPU Instruction: Multiplies values in regA and regB and stores result in regA
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register.
 * @param opB {unsigned char} Operand B: immediate.
 */
void handle_MUL(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  alu(cpu, ALU_MUL, opA, opB);
}

/**
 * CPU Instruction: No operation
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: --
 * @param opB {unsigned char} Operand B: --
 */
void handle_NOP(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  return;
}

/**
 * CPU Instruction: Performs a bitwise-NOT on regA
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register A.
 * @param opB {unsigned char} Operand B: register B.
 */
void handle_NOT(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  alu(cpu, ALU_NOT, opA, opB);
}

/**
 * CPU Instruction: Performs a bitwise-OR on regA and regB and stores result in regA
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register A.
 * @param opB {unsigned char} Operand B: register B.
 */
void handle_OR(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  alu(cpu, ALU_OR, opA, opB);
}

/**
 * CPU Instruction: Pop first value out of stack and store in regA
 * 
 * TODO: Make use of INC instruction
 * TODO: Do not access ram directly
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register.
 * @param opB {unsigned char} Operand B: --
 */
void handle_POP(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  cpu->registers[opA] = cpu->ram[cpu->registers[SP]];
  cpu->registers[SP] += 1;
}

/**
 * CPU Instruction: Prints alpha character value stored in given register
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register.
 * @param opB {unsigned char} Operand B: --
 */
void handle_PRA(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  printf("%c\n", cpu->registers[opA]);
}

/**
 * CPU Instruction: Prints numeric value stored in given register
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register.
 * @param opB {unsigned char} Operand B: --
 */
void handle_PRN(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  printf("%d\n", cpu->registers[opA]);
}

/**
 * CPU Instruction: Push given value on to stack
 * 
 * TODO: Make use of DEC instruction
 * TODO: Do not access ram directly
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register.
 * @param opB {unsigned char} Operand B: --
 */
void handle_PUSH(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  cpu->registers[SP] -= 1;
  cpu->ram[cpu->registers[SP]] = cpu->registers[opA];
}

/**
 * CPU Instruction: Returns from subroutine using address in stack
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: --
 * @param opB {unsigned char} Operand B: --
 */
void handle_RET(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  handle_POP(cpu, TMP, '\0');
  cpu->pc = cpu->registers[TMP];
  get_next_instruction(cpu);
}

/**
 * CPU Instruction: Shift value in regA to the left by value in regB
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register A.
 * @param opB {unsigned char} Operand B: register B.
 */
void handle_SHL(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  alu(cpu, ALU_SHL, opA, opB);
}

/**
 * CPU Instruction: Shift value in regA to the right by value in regB
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register A.
 * @param opB {unsigned char} Operand B: register B.
 */
void handle_SHR(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  alu(cpu, ALU_SHR, opA, opB);
}

/**
 * CPU Instruction: Stores value in regB to address in regA
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register A
 * @param opB {unsigned char} Operand B: register B
 */
void handle_ST(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  cpu->mar = cpu->registers[opA];
  cpu->mdr = cpu->registers[opB];
  cpu_ram_write(cpu);
}

/**
 * CPU Instruction: Subtracts values in regA and regB and stores result in regA
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register A.
 * @param opB {unsigned char} Operand B: register B.
 */
void handle_SUB(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  alu(cpu, ALU_SUB, opA, opB);
}

/**
 * CPU Instruction: Performs a bitwise-XOR on regA and regB and stores result in regA
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register A.
 * @param opB {unsigned char} Operand B: register B.
 */
void handle_XOR(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  alu(cpu, ALU_XOR, opA, opB);
}

/**
 * Loads CPU instructions into branch table
 * 
 * @param bt {handler*} Pointer to an array of function pointers.
 */
void load_cpu_instructions(handler *bt)
{
  bt[ADD] = handle_ADD;
  bt[AND] = handle_AND;
  bt[CALL] = handle_CALL;
  bt[CMP] = handle_CMP;
  bt[DEC] = handle_DEC;
  bt[DIV] = handle_DIV;
  bt[INC] = handle_INC;
  bt[INT] = handle_INT;
  bt[IRET] = handle_IRET;
  bt[JEQ] = handle_JEQ;
  bt[JGE] = handle_JGE;
  bt[JGT] = handle_JGT;
  bt[JLE] = handle_JLE;
  bt[JLT] = handle_JLT;
  bt[JMP] = handle_JMP;
  bt[JNE] = handle_JNE;
  bt[LD] = handle_LD;
  bt[LDI] = handle_LDI;
  bt[MOD] = handle_MOD;
  bt[MUL] = handle_MUL;
  bt[NOP] = handle_NOP;
  bt[NOT] = handle_NOT;
  bt[OR] = handle_OR;
  bt[POP] = handle_POP;
  bt[PRA] = handle_PRA;
  bt[PRN] = handle_PRN;
  bt[PUSH] = handle_PUSH;
  bt[RET] = handle_RET;
  bt[SHL] = handle_SHL;
  bt[SHR] = handle_SHR;
  bt[ST] = handle_ST;
  bt[SUB] = handle_SUB;
  bt[XOR] = handle_XOR;
}