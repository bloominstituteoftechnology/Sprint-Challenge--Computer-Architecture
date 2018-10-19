#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG 0

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(char *filename, struct cpu *cpu)
{
  FILE *fp; 
  char line[256]; 
  int counter = 0; 

  if ((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Cannot open file %s\n", filename);
    exit(1);
  }
    #if DEBUG
     printf("\n**********Lines from file:***********\n");
     #endif

    while (fgets(line, sizeof(line), fp) != NULL) { 
      char *ptr;
      unsigned char byte = strtoul(line, &ptr, 2);
      if (ptr == line) {
        continue;
      }
      cpu->ram[counter++] = byte; 
      
      #if DEBUG
      printf("Value of line: %s", line);
      #endif
    }
   
    #if DEBUG
    printf("\n*******RAM in Load*******\n");
    for (unsigned long i = 0; i < 256; i++) {
      printf("cpu->ram[%lu] = %u\n", i, cpu->ram[i]);
    }
    #endif
  // TODO: Replace this with something less hard-coded
}

unsigned char cpu_ram_read(struct cpu *cpu, int index){
  return cpu->ram[index];
}
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}


/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *reg = cpu->reg;
  unsigned char valB = reg[regB];
  switch (op) {
    case ALU_ADD:
      reg[regA] += valB;
      break;
    case ALU_AND:
      reg[regA] = reg[regA] & reg[regB];
      break;
    case ALU_CMP:
      #if DEBUG
      printf("FL is %d\n", cpu->FL);
      #endif
      if ((regA - regB) < 0){
        cpu->FL = 4;
      }
      else if ((regA - regB) == 0){
        cpu->FL = 1;
      }
      else if ((regA - regB) > 0){
        cpu->FL = 2;
      }
      else {
        cpu->FL = 0;
      }
      #if DEBUG
      printf("FL is %d\n", cpu->FL);
      #endif
      break;
    case ALU_DEC:
      reg[regA] = reg[regA] - 1;
      break;
    case ALU_DIV:
      reg[regA] = reg[regA] / reg[regB];
      break;
    case ALU_INC:
      reg[regA] = reg[regA] + 1;
      break;
    case ALU_MOD:
      reg[regA] = reg[regA] % reg[regB];
      break;
    case ALU_MUL:
      reg[regA] *= valB;
      break;
    case ALU_NOT:
      reg[regA] = ~reg[regA];
      break;
    case ALU_OR:
      reg[regA] = reg[regA] | reg[regA];
      break;
    case ALU_SHL:
      reg[regA] = reg[regA] << reg[regB];
      break;
    case ALU_SHR:
      reg[regA] = reg[regA] >> reg[regB];
      break;
    case ALU_SUB:
      reg[regA] = reg[regA] - reg[regB];
      break;
    case ALU_XOR:
      reg[regA] = reg[regA] ^ reg[regB];
      break;
    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  unsigned char *reg = cpu->reg;
  int running = 1; 
  
  while (running) {
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    int difference = ((IR >> 6) & 0b11) + 1;
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC+1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC+2);
    unsigned char maskedInterrupts = cpu->reg[IM] & cpu->reg[IS];
    unsigned char interrupts = 1;

    while (interrupts) {
      for (int i = 0; i < 8; i++) {
        if (((maskedInterrupts >> i) & 1) == 1){
          interrupts = 0;
          cpu->reg[IS] &= ~cpu->reg[IM];
          cpu_ram_write(cpu, --cpu->reg[SP], cpu->PC);
          cpu_ram_write(cpu, --cpu->reg[SP], cpu->FL);
          for (int r = 0; r < 7; r++) {
            cpu_ram_write(cpu, --cpu->reg[SP], cpu->reg[r]);
          }
          cpu->PC = cpu->ram[0xF8 + i]; // Because interrupt vector starts at F8
          break;
        }
      }
      interrupts = 0;
    }
    switch(IR)
    {
      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        cpu->PC+=difference;
        break;
      case CALL:
        #if DEBUG
        printf("Value before call: %d\n", cpu->PC);
        #endif
        cpu_ram_write(cpu, --cpu->reg[SP], cpu->PC+=difference);
        #if DEBUG
        printf("Value of PC in register: %d\n", cpu->PC);
        #endif
        cpu->PC = cpu->reg[operandA];
        #if DEBUG
        printf("Value of PC after call: %d\n", cpu->PC);
        #endif
        break;
      case CMP:
        alu(cpu, ALU_CMP, cpu->reg[operandA], cpu->reg[operandB]);
        cpu->PC+=difference;
        break;
      case HLT:
        running = 0;
        break;
      case JEQ:
        if ((cpu->FL & 0b1) == 0b1){
          cpu->PC = cpu->reg[operandA];
        } else {
          cpu->PC+=difference;
        }
        break;
      case JMP:
        cpu->PC = cpu->reg[operandA];
        break;
      case JNE:
        if ((cpu->FL & 0b1) == 0b0){
          cpu->PC = cpu->reg[operandA];
        } else {
          cpu->PC+=difference;
        }
        break;
      case LDI:
        reg[operandA] = operandB;
        cpu->PC+=difference;
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        cpu->PC+=difference;
        break;
      case POP:
        reg[operandA] = cpu->ram[cpu->reg[SP]++];
        cpu->PC+=difference;
        break;
      case PRN:
        printf("%d\n", reg[operandA]);
        cpu->PC+=difference;
        break;
      case PUSH:
        cpu_ram_write(cpu, --cpu->reg[SP], reg[operandA]);
        cpu->PC+=difference;
        break;
      default:
        printf("Unknown instruction at %02x: %02x\n", cpu->PC, IR);
        exit(2);
    }

    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
  }
  #if DEBUG
    printf("\n********RAM in run********\n");
      for (unsigned long i = 0; i < 256; i++) {
      printf("cpu->ram[%lu] = %u\n", i, cpu->ram[i]);
    }
    printf("\n*******End of debugger********\n\n");
  #endif
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  cpu->IR = 0;
  cpu->reg = (unsigned char *) calloc(8, sizeof(unsigned char));
  cpu->ram = (unsigned char *) calloc(256, sizeof(unsigned char));
  cpu->reg[7] = 0xF4;
  cpu->FL = 00000000;
  // TODO: Initialize the PC and other special registers

  // TODO: Zero registers and RAM
}