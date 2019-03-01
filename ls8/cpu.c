#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  // Write value to this location in ram
  cpu->ram[address] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *pathToProgram)
{
//  char data[DATA_LEN] = {
//    // From print8.ls8
//    0b10000010, // LDI R0,8
//    0b00000000,
//    0b00001000,
//    0b01000111, // PRN R0
//    0b00000000,
//    0b00000001  // HLT
//  };
//
//  int address = 0;
//
//  for (int i = 0; i < DATA_LEN; i++) {
//    cpu->ram[address++] = data[i];
//  }

  // TODO: Replace this with something less hard-coded
  
  FILE *filePointer = fopen(pathToProgram, "r");
  
  if (filePointer == NULL) {
    printf("Could not open program: %s\n", pathToProgram);
    exit(2);
  }
  
  int address = 0;
  char line[1000];
  
  while (fgets(line, 1000, filePointer) != NULL) {
    
    if (strlen(line) == 1) {  // 1 for the bracket that indicates the new line
      continue;
    }
    
    char *endPoint;
    unsigned char instruction = strtoul(line, &endPoint, 2) & 0xFF;
    
    if (endPoint == line) {
      continue;
    }
    
//    printf("Read a line: '0x%02X'\n", instruction);
    
//    cpu->ram[address++] = instruction;
//    cpu->ram[address] = instruction;
//    address += 1;
    
    // Use out write to ram function instead
    cpu_ram_write(cpu, address++, instruction);
  }
  
  fclose(filePointer);
  
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->reg[regA] = (cpu->reg[regA] * cpu->reg[regB]) & 0xFF;
      break;
    case ALU_ADD:
      cpu->reg[regA] = (cpu->reg[regA] + cpu->reg[regB]) & 0xFF;
      break;


    // TODO: implement more ALU ops
  }
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void push(struct cpu *cpu, unsigned char registerOfValueToPush)
{
  // R7 is the stack pointer
  // Decrement the 'SP'
  // Copy the value in the given register to the address pointed to by `SP`
  
  // Decrement the size of the stack
  cpu->reg[7]--;
  
  unsigned char valueToPush = cpu->reg[registerOfValueToPush];
  
  cpu_ram_write(cpu, cpu->reg[7], valueToPush);
  
  // Move the PC to the next instruction.
  cpu->pc += 2;
}

void pop(struct cpu *cpu, unsigned char registerToPopValueTo)
{
  // Copy the value from the address pointed to by `SP` to the given register.
  // Increment `SP`
  
  // Get address out of register 7
  unsigned char stackPointerAddress = cpu->reg[7]; 
  // Get the value out of that address
  unsigned char valueToPop = cpu_ram_read(cpu, stackPointerAddress);
  
  cpu->reg[registerToPopValueTo] = valueToPop;
  
  // Increment the size of the stack
  cpu->reg[7]++;
  
  // Move the PC to the next instruction.
  cpu->pc += 2;
}

void call(struct cpu *cpu, unsigned char registerOfAddressToJumpTo)
{
  unsigned char addressToJumpTo = cpu->reg[registerOfAddressToJumpTo];
  
  unsigned char nextInstructionToExecute = cpu->pc + 2;
  
  // make room on the stack to store the address
  cpu->reg[7]--;  // stack grows down
  
  cpu_ram_write(cpu, cpu->reg[7], nextInstructionToExecute);
  
  cpu->pc = addressToJumpTo;
}

void ret(struct cpu *cpu)
{
  // get the return address out of the stack
  unsigned char addressToReturnTo = cpu_ram_read(cpu, cpu->reg[7]);
  
  // increment size of stack, because we pop out of it
  cpu->reg[7]++;
  
  cpu->pc = addressToReturnTo;
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->pc);
    
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operandA = cpu_ram_read(cpu, cpu->pc + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->pc + 2);
    
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    switch (IR) {
      case LDI:
        cpu->reg[operandA] = operandB;
        cpu->pc += 3;
        break;
      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        cpu->pc += 2;
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        cpu->pc += 3;
        break;
      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        cpu->pc += 3;
        break;
      case PUSH:
        push(cpu, operandA);
        break;
      case POP:
        pop(cpu, operandA);
        break;
      case CALL:
        call(cpu, operandA);
        break;
      case RET:
        ret(cpu);
        break;
      case HLT:
        running = 0;
        break;
      default:
        printf("Unexpected instruction 0x%02X at 0x%02X\n", IR, cpu->pc);
        exit(1);
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  
//  cpu->pc = 0;
//  memset(cpu->ram, 0, sizeof(unsigned char)*256);
//  memset(cpu->reg, 0, sizeof(unsigned char)*8);
  
  //Alternative to memset()
  // Init registers
  for (int i = 0; i < 7; i++) {
    cpu->reg[i] = 0;
  }
  cpu->reg[7] = 0xF4;  // Spec says to set R7 to 0xF4
  
  // Init PC
  cpu->pc = 0;
  
  // Init Ram
  memset(cpu->ram, 0, sizeof(cpu->ram));
  
}
