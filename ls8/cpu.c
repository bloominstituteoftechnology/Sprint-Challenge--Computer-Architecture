#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

/**
  Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{

//  * this was working, 
  // const int DATA_LEN = 6;
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   10000010 # LDI R0,8, // LDI R0,8
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
  FILE *fp;
  char buf[1024];
  int address = 0;
  fp = fopen(filename, "r");

  while (fgets(buf, sizeof(buf), fp))
  {
    
    // the endptr is the remaining part of the line after the binary number.
    // i could compare this to buf and if they match skip adding to ram.
    char *endptr;
    unsigned char data = strtol(buf, &endptr, 2);

    // this skips adding to ram if first char is a blank line
    // if(buf[0] == '\r' || buf[0] == '\n'){
    //   continue;
    // };
    // from solution lecture. endptr is the first invalid char, if buf and endptr match, the line is skipped
    if (buf == endptr){
      continue;
    }
    // buf[strlen(buf) - 1] = '\0'; // eat the newline fgets() stores, does not remove if blank lines

    // printf("this is the endptr %s\n",endptr);
    // printf("this is what buf is %s\n==============\n", buf);

 

    
    cpu->ram[address] = data;
    address += 1;
    // printf("this is what data is %d\n", data);
    // printf("this is what address is %d\n", address);
    // printf("this is what buf is %c\n", buf[strlen(buf) - 2]);
    // printf("this is what buf is %s\n==============\n", buf);
  }
  fclose(fp);
}

unsigned char read_cpu_ram(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void write_cpu_ram(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

// this was my solution before solution lecture, this func also worked
// unsigned char read_cpu_ram(struct cpu *cpu, int location)
// {
//   return cpu->ram[location];
// }

/**
 * ALU
 */

void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->reg[regA] *= cpu->reg[regB];
      break;
    case ALU_ADD:
      cpu->reg[regA] += cpu->reg[regB];
      break;

    // TODO: implement more ALU ops
  }
}



void push(struct cpu *cpu, unsigned char regA)
{
  cpu->reg[SP] -= 1;
  // TODO add value of regA to mem at sp
  write_cpu_ram(cpu, cpu->reg[SP], regA);
  // printf("SP = %x\n", cpu->reg[SP]);
}

unsigned char pop(struct cpu *cpu)
{
  // TODO add value inside mem at sp to regA
  unsigned char pop_reg = read_cpu_ram(cpu, cpu->reg[SP]);
  cpu->reg[SP] += 1;
  // printf("pop_reg = %d\n", pop_reg);
  return pop_reg;
}



/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {

    unsigned char IR = read_cpu_ram(cpu, cpu->PC);
    unsigned char operandA = read_cpu_ram(cpu, cpu->PC + 1);
    unsigned char operandB = read_cpu_ram(cpu, cpu->PC + 2);
    int needed_operands = (IR >> 6);
    unsigned char poppen;
    // printf("what PC is = %d\n", cpu->PC);
    switch (IR){
      case LDI:
        cpu->reg[operandA] = operandB;
        break;
      case PRN:
        printf("print inside PRN: %d\n", cpu->reg[operandA]);
        break;
      case MUL:
        // cpu->reg[operandA] *= cpu->reg[operandB];
        alu(cpu, ALU_MUL, operandA, operandB);
        break;
      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        break;
      case PUSH:
        push(cpu, cpu->reg[operandA]);
        break;
      case POP:
        cpu->reg[operandA] = pop(cpu);
        break;
      case HLT:
        running = 0;
        break;
      case CALL:
        push(cpu, (cpu->PC + 1 + needed_operands));
        // printf("this is the operandA in CALL %d\n", cpu->reg[operandA]);
        // printf("this value in CALL being pushed %d\n", (cpu->PC + 1 + needed_operands));
        cpu->PC = cpu->reg[operandA];
        continue;
      case RET:
        poppen = pop(cpu);
        // printf("%d\n", poppen);
        cpu->PC = poppen;
        continue;
    }
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    // printf("what PC is = %d\n", cpu->PC);
    // printf("what is IR %d\n",IR);
    cpu->PC += 1 + needed_operands;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{

  // from thursday lecture. look up memset
  memset(cpu->reg, 0, sizeof cpu->reg);
  memset(cpu->ram, 0, sizeof cpu->ram);
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;

  // TODO: Zero registers and RAM
  cpu->reg[SP] = 0xF4;


  // printf("SP = %x\n", cpu->reg[SP]);
}
