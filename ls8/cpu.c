#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//from solution lecture
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
void cpu_load(struct cpu *cpu, char *argv[])
{

//const int DATA_LEN = 6;
//  char data[6] = { // I cannot get this to work with the variable
//    // From print8.ls8
//    0b10000010, // LDI R0,8
//    0b00000000,
//    0b00001000,
//    0b01000111, // PRN R0
//    0b00000000,
//    0b00000001  // HLT
//  };
  FILE *fp = fopen(argv[1], "r"); // https://www.tutorialspoint.com/c_standard_library/c_function_fopen.htm
  int address = 0;
  char buffer[256];
  char *pointer;
  // if file not found, error
  if (fp == NULL)
  {
    fprintf(stderr, "fail\n");
    exit(1);
  }
  while (fgets(buffer, sizeof(buffer), fp) != NULL)
  {
      unsigned char data = strtoul(buffer, &pointer, 2);
      if (pointer == buffer)
      {
          continue;
      }
      cpu->ram[address++] = data;
    // store string in the pointer
  }
  fclose(fp);
//  for (int i = 0; i < DATA_LEN; i++) {
//    cpu->ram[address++] = data[i];
//  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *reg = cpu->registers;
  switch (op) {
    case ALU_MUL:
      // TODO
      reg[regA] *= reg[regB];
      break;
    // TODO: implement more ALU ops
    case ALU_ADD:
      reg[regA] += reg[regB];
      break;
    default:
      break;
  }
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
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    // instruction register IR holds the instruction currently being executed
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);
    // some operations need the two bytes after PC to perform operations
    // so we're creating variables just in case
    //printf("IR %d\n PC %d\n", IR, cpu->PC);
    switch (IR)
    {
        case MUL:
            alu(cpu, ALU_MUL, operandA, operandB);
            cpu->PC += 3;
            break;
        case HLT:
            running = 0;
            break;
        case PRN:
            printf("%d\n", cpu->registers[operandA]);
            cpu->PC += 2;
            break;
        case LDI:
            cpu->registers[operandA] = operandB;
            cpu->PC += 3 ;
        //    printf ("ldi %d\n", cpu->PC);
            break;
        case PUSH:
            cpu->registers[7] -= 1;
            cpu->ram[cpu->registers[7]] = cpu->registers[operandA];
            cpu->PC += 2;
            break;
        case POP:
            cpu->registers[operandA] = cpu->ram[cpu->registers[7]];
            cpu->registers[7] += 1;
            cpu->PC += 2;
            break;
        case ADD:
        //    printf ("what is up with the add?");
            alu(cpu, ALU_ADD, operandA, operandB);
            cpu->PC += 3;
        //    printf ("add %d\n", cpu->PC);
            break;
        case CALL:
            cpu->registers[7] -= 1;
            cpu->ram[cpu->registers[7]] = cpu->PC + 2;
        //    printf ("call1 %d\n", cpu->PC);
            cpu->PC = cpu->registers[operandA];
        //    printf ("call %d\n", cpu->PC);
            break;
        case RET:
            cpu->PC = cpu->ram[cpu->registers[7]];
			cpu->registers[7] += 1;
		//	printf ("ret %d\n", cpu->PC);
            break;
        default:
            fprintf(stderr, "wtf\n");
            exit(2);
            break;
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
  cpu->registers[7] = 0xF4;
  memset(cpu->registers, 0, sizeof(cpu->registers));
  memset(cpu->ram, 0, sizeof(cpu->ram));
// tried doing this a different way that made more sense to me
// https://stackoverflow.com/questions/23778404/clear-a-c-array


//  cpu->registers = (unsigned char) calloc(8, sizeof(unsigned char));
//  cpu->ram = (unsigned char) calloc(256, sizeof(unsigned char));
// I got some help from a classmate on the calloc stuff, but now that I see it, it makes sense
// since calloc sets the allocated memory to zero: https://www.tutorialspoint.com/c_standard_library/c_function_calloc.htm
  // TODO: Zero registers and RAM
}
