#include "cpu.h"
#include <stdio.h>
#include <string.h>

#define DATA_LEN 6

void (*branchTable[256])(struct cpu *cpu, unsigned char, unsigned char) = {0};

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *file = fopen(filename, "r");
  char line[1024];

  int address = 0;

  while (fgets(line, sizeof line, file) != NULL)
  {
    if (line[0] == '\n' || line[0] == '#')
    {
      continue;
    }

    cpu->ram[address++] = strtol(line, NULL, 2);
  }

  fclose(file);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    // TODO
    break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{

  while (cpu->running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    unsigned char IR = cpu_ram_read(cpu, cpu->pc);
    unsigned char operandA = cpu_ram_read(cpu, cpu->pc + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->pc + 2);
    

    void (*handler)(struct cpu * cpu, unsigned char, unsigned char);

    handler = branchTable[IR];

    if (handler == NULL)
    {
      printf("Unknown instruction\n");
      return;
    }

    handler(cpu, operandA, operandB);

    

    int changed = (IR >> 4) & 1;
    if (!changed)
    {
      cpu->pc += (IR >> 6) + 1;
    }
  }
}

void handle_LDI(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  cpu->registers[operandA] = operandB;
}

void handle_MUL(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  cpu->registers[operandA] = cpu->registers[operandA] * cpu->registers[operandB];
}
void handle_ADD(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  printf("%d in return");
  cpu->registers[operandA] = cpu->registers[operandA] + cpu->registers[operandB];
}

void handle_PRN(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  printf("%d\n", cpu->registers[operandA]);
}

void handle_HLT(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  cpu->running = 0;
}
void handle_POP(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  unsigned char i = cpu->registers[7];
  // printf("%d\n", i);
  cpu->registers[operandA] = cpu->ram[i];
  i++;
}
void handle_PUSH(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  // unsigned char i = cpu->registers[7];
  unsigned char SP = --cpu->registers[7];
  // i--;
  // cpu->registers[7] = i;
  printf("%02x\n", SP);
  
  // cpu->registers[7]--;
  // printf("%02x\n", cpu->registers[7]);
  // cpu->ram[cpu->registers[7]] = cpu->registers[operandA];
  cpu->ram[SP] = cpu->registers[operandA];
}
void handle_CALL(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  printf("%d in return");
  unsigned char address = cpu->pc +2;

unsigned char SP = --cpu->registers[7];
cpu->ram[SP] = address;

cpu->pc = cpu->registers[operandA];

// Calls a subroutine (function) at the address stored in the register.

// The address of the instruction directly after the CALL instruction is pushed onto the stack.
// The PC is set to the address stored in the given register.
  // pushes address of instructions in operandA, then move cpu->pc to subroutine address
  // cpu->registers[operandA] =
  // cpu->ram[0] = cpu->registers[operandB];
  // handle_PUSH(cpu, cpu->registers[operandB], operandB);
   
}
  
void handle_RET(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
//   RET
// Return from subroutine.
// Pop the value from the top of the stack and store it in the PC.
  // pop ret address off the stack and store on cpu->pc
  printf("%d in return");
   unsigned char SP = cpu->registers[7];
  // printf("%d\n", i);
  cpu->pc = cpu->ram[SP];
  SP++;

}

void handle_JMP(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  // printf("%d in returnjmp\n");
  cpu->pc = cpu->registers[operandA];
  
}
void handle_CMP(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  // printf("%d in returncmp\n");
  // 00000LGE
  // If they are equal, set the Equal E flag to 1, otherwise set it to 0.
if (cpu->registers[operandA] == cpu->registers[operandB]) {
  cpu->fl = 1;
}
else if(cpu->registers[operandA] < cpu->registers[operandB]){
  cpu->fl = 4;
}
else if(cpu->registers[operandA] > cpu->registers[operandB]){
  cpu->fl = 2;
}
// If registerA is less than registerB, set the Less-than L flag to 1, otherwise set it to 0.

// If registerA is greater than registerB, set the Greater-than G flag to 1, otherwise set it to 0.
}
void handle_JNE(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  // If E flag is clear (false, 0), jump to the address stored in the given register.
  // printf("%d in returnjne\n");
  if(cpu->fl == 2 || cpu->fl == 4){
  cpu->pc = cpu->registers[operandA];
  }
  else cpu->pc = cpu->pc += 2;
  
}
void handle_JEQ(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  
  // If equal flag is set (true), jump to the address stored in the given register.
  if(cpu->fl == 1){
    cpu->pc = cpu->registers[operandA];
  }
  else cpu->pc +=2;
  // printf("%d in returnjmp\n");
  // cpu->pc = cpu->registers[operandA];
}

/**
 * Initialize a CPU struct
 */

void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;
  cpu->running = 1;
  cpu->registers[7] = 0xF4;
  cpu->fl = 0;

  // TODO: Zero registers and RAM
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->ram, 0, sizeof cpu->registers);

  branchTable[LDI] = handle_LDI;
  branchTable[MUL] = handle_MUL;
  branchTable[PRN] = handle_PRN;
  branchTable[HLT] = handle_HLT;
  branchTable[POP] = handle_POP;
  branchTable[PUSH] = handle_PUSH;
  branchTable[CALL] = handle_CALL;
  branchTable[RET] = handle_RET;
  branchTable[ADD] = handle_ADD;
  branchTable[JMP] = handle_JMP;
  branchTable[CMP] = handle_CMP;
  branchTable[JNE] = handle_JNE;
  branchTable[JEQ] = handle_JEQ;
}