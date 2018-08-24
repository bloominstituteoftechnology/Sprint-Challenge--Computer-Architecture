#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cpu.h"

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
  char line[1024];
  int address = 0;

  FILE *fp = fopen(filename, "r");
  // printf("filename: %s\n", filename);

  while(fgets(line, sizeof line, fp) != NULL) {

    if(line[strlen(line)-1] != '\n') { // this will check to see if we entered line longer than 1024 bytes in command line
      fprintf(stderr, "Line too long!");
      exit(2);
    }

    char *endchar;
    unsigned char v = strtoul(line, &endchar, 2);

    // skip empty lines in files
    if (line == endchar) { // this would mean that the first invalid character was at the beginning of the line
      continue; // then skip it
    }

    cpu_ram_write(cpu, address++, v);

    // printf("%u\n", v); 
  }


  // const int DATA_LEN = 6;
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };

  // int address = 0;

  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }

  // TODO: Replace this with something less hard-coded

}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->reg[regA] *= cpu->reg[regB]; /* multiply value of register A with value of register B
                                           and store in register A */
      break;

    // TODO: implement more ALU ops
    case ALU_ADD:
      cpu->reg[regA] += cpu->reg[regB];
      break;

  }
}

void push(struct cpu *cpu, unsigned char value, unsigned char *SP) {
  SP--;
  cpu->ram[*SP] = value; // ram at the address pointed at by the SP is equal to passed in value
}

unsigned char pop(struct cpu *cpu, unsigned char *SP) {
  unsigned char popped_value = cpu->ram[*SP];
  SP++;
  return popped_value; // return popped value so it can be stored in a register
}


/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{

  unsigned char *SP; 
  SP = &cpu->reg[7];
  *SP = cpu->ram[0xF4];

  int running = 1; // True until we get a HLT instruction

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    // 1a. Get the values of the operands
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1); 
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);
        
    // printf("TRACE: %02x: %02x\n", cpu->PC, IR);
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    switch(IR) {
      case LDI:
        cpu->reg[operandA] = operandB;
        // cpu->PC += 3; // use bit shift method instead
        break;
      
      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        // cpu->PC += 2; // use bit shift method instead
        break;

      case HLT:
        running = 0; // running equals false
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;

      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        break;

      case RET:
        cpu->PC = pop(cpu, SP);
        break;
      
      case CALL:
        push(cpu, cpu->PC + 2, SP);
        cpu->PC = cpu->reg[operandA];
        break;  

      case PUSH:
        push(cpu, cpu->reg[operandA], SP);
        break;

      case POP:
        cpu->reg[operandA] = pop(cpu, SP);
        break;
    
      case CMP:
        if (cpu->reg[operandA] == cpu->reg[operandB]) {
            cpu->fl = 1;
        } else {
            cpu->fl = 0;
        }
        break;
    
      case JEQ:
        if (cpu->fl)
        {
            cpu->PC = cpu->reg[operandA];
        }
        else
        {
            cpu->PC += 2;
        }
        break;

      case JMP:
        cpu->PC = cpu->reg[operandA];
        break;

      case JNE: // jump if not equal
        if(!cpu->fl) {
            cpu->PC = cpu->reg[operandA];
        } else {
            cpu->PC += 2;
        }
        break;


      default:
        printf("unknown instruction at %02x: %02x\n", cpu->PC, IR);
        exit(2);
    }

    int instruction_sets_the_pc = (IR >> 4) & 1;

    if (!instruction_sets_the_pc)
    {
      // right shift it 6 places
      cpu->PC += ((IR >> 6) & 0x3) + 1;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;

  // TODO: Zero registers and RAM


}
