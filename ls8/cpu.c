#include "cpu.h"
#include "string.h"
#include "stdio.h"
#include <stdlib.h>

#define ADDR_EMPTY_STACK 0xF4

cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  return cpu->ram[index];
}

cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char value)
{
  cpu->ram[index] = value;
  return 0;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *argv[])
{

  FILE *fp; //creates file variable we will place opened file in, fp=filepointer
  fp = fopen(argv[1], "r");
  if (fp == NULL)
  {
    perror("Error opening file"); //if fp == NULL something went wrong, filepath or otherwise
  }
  char string[256];          //Where instuctions will be placed;
  unsigned char address = 0; //starting address
  //You'll have to convert the binary strings to integer values to store in RAM. The
  //built-in `strtoul()` library function might help you here.
  while (fgets(string, sizeof(string), fp) != NULL)
  {
    // if(string[0] != '#'){
    //unsigned long int strtoul(const char *str, char **endptr, int base)
    unsigned long int data = strtoul(string, NULL, 2);
    cpu->ram[address] = data;
    address++;
    // }
  }
  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{

  switch (op)
  {
  case ALU_MUL:
    cpu->reg[regA] *= cpu->reg[regB];
    // return regA;
    break;
  case ALU_CMP:
    if (cpu->reg[regA] == cpu->reg[regB])
    {
      cpu->FL = (cpu->FL & ~7) | 1;
    }
    else if (cpu->reg[regA] <= cpu->reg[regB])
    {
      cpu->FL = (cpu->FL & ~7) | 2;
    }
    else
    {
      cpu->FL = (cpu->FL & ~7) | 4;
    }
    break;
    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  unsigned int IR; //variable that will storevalue of current instruction

  while (running)
  {
    // IR is the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu->PC); //gets PC value

    // numOperands figures out how many operands this next instruction requires by looking at instruction code
    unsigned int numOperands = IR >> 6; //Nuumber of operations in instruction
 
 
    int operandA = cpu_ram_read(cpu, (cpu->PC + 1) & 0xff); //gets next byte past PC
    int operandB = cpu_ram_read(cpu, (cpu->PC + 2) & 0xff); //gets bye after next past PC 

    printf("CPU Run Values: cpu->PC:%02x operandA:%02x operandB:%02x IR:%02x \n", cpu->PC, operandA, operandB, IR);
   
    switch (IR)
    {
    case LDI:
      cpu->reg[operandA] = operandB; //takes value of operandA and sets it to operand b
      break;
    case PRN:
      printf("PRN: %d \n", cpu->reg[operandA]);
      break;
    case HLT:
      running = 0;
      break;
    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;
    case PUSH:
      cpu->SP--;
      cpu->reg[operandA] = cpu_ram_write(cpu, --cpu->SP, cpu->reg[operandA]);
      break;
    case POP:
      cpu->reg[operandA] = cpu_ram_read(cpu, cpu->SP);
      cpu->SP++;
      break;
    case CALL:
      //The address of the ***instruction*** _directly after_ `CALL` is pushed onto the stack.
      cpu_ram_write(cpu, cpu->reg[7], ++cpu->PC);
      cpu->PC = cpu->reg[operandA];
      break;
    case RET:
      cpu->PC = cpu_ram_read(cpu, cpu->reg[7]);
      break;
    case CMP:
      alu(cpu, ALU_CMP, operandA, operandB);
      break;
    case JMP:
      cpu->PC = operandA; 
      break;  
    case JNE:
      if((cpu->FL & (1<<0)) == 0){
      cpu->PC = cpu->reg[operandA];
      }
      break;
    case JEQ:
      //  flagbits = cpu->FL & (1<<0);
      if((cpu->FL & (1<<0)) == 1){
      cpu->PC = cpu->reg[operandA];
    }
      break; 
    } 
    //Moves the PC to the next instruction after current completed.
    cpu->PC += numOperands + 1  ;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0; //Sets position counter to 0

  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->reg, 0, sizeof cpu->reg);
  cpu->SP = ADDR_EMPTY_STACK;
  cpu->FL = 0;
}
