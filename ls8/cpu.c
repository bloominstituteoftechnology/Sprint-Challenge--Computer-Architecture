#include <string.h>
#include <stdlib.h>
#include <stdio.h> 

#include "cpu.h"

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  //open file
  FILE *fptr = fopen(filename, "r");

  if (fptr == NULL)
  {
    printf("File %s is not recognized.\n", filename);
    exit(2);
  }

  //read line by line until returns null, ignore blank lines and comments
  char line[1000];
  int address = 0;

  while(fgets(line, sizeof line, fptr) != NULL)
  {
    //convert string to a number. if get an error 
    char *endpointer;
    unsigned char value = strtoul(line, &endpointer, 2);  //needs pointer to a pointer so need & as address of pointer
    //ignore lines where no numbers read
    if (endpointer == line) 
    {
      continue;
    }
    //save data into ram, have to convert binary strings to int values to store in ram with strtoul
    //cpu_ram_write(cpu, address++, value);
    cpu->ram[address++] = value;
  }
  fclose(fptr);

  //original hard code we're refactoring
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
  unsigned char valA = cpu->reg[regA];
  unsigned char valB = cpu->reg[regB];
  unsigned char flag;
  switch (op) {
    case ALU_MUL:
      // TODO
      cpu->reg[regA] = cpu->reg[regA]*cpu->reg[regB];  //reg[regA] *= valB??
      break;
    // TODO: implement more ALU ops
    case ALU_ADD:
      cpu->reg[regA] = cpu->reg[regA]+cpu->reg[regB];
      break;
    case ALU_CMP:  //compare values in two registers w limited subtraction. 
      if (valA < valB)  //if less than flag to 1
      {
        flag = 0b00000100;  
      }
      else if (valA > valB)  //if greater than flag to 1
      {
        flag = 0b00000010;
      }
      else if (valA == valB) //if equal flag to 1
      {
        flag = 0b00000001;
      }
      else
      {
        flag = 0b00000000;  //otherwise set to 0
      }
      cpu->fl = flag;
      break;
  }
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar)
{
  return cpu->ram[mar];
}

//mar is index in ram so set to new value 
void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr)
{
  cpu->ram[mar] =  mdr;
}

void push(struct cpu *cpu, unsigned char val)
{
  //push the value in the given register to top of the stack
  //decrement the sp
  cpu->reg[7]--;
  //copy the value in the given register to the address pointed to by the sp
  //another variable? add sp to struct? 
  cpu_ram_write(cpu, cpu->reg[7], val);
}

unsigned char pop(struct cpu *cpu)
{
  //pop the value at the top of the stack into given register
  //copy the value from the address pointed to by sp to given register
  //put value in register take it out of ram (by reading) get address reg7 and store it in given register
  //unsigned char pop_val = cpu_ram_read(cpu, cpu->reg[7]);
  unsigned char val = cpu->ram[cpu->reg[7]];
  //increment sp
  cpu->reg[7]++; 
  return val;
}

void call(struct cpu *cpu, unsigned char registerAddress)
{
  //push address of instruction on to stack so we can return to where we left off after subroutine executes
  unsigned char address = cpu->reg[registerAddress];
  push(cpu, cpu->pc+2);
  //set program counter to location of subroutine
  cpu->pc = address;
  //pc is set to address stored in given register. jump to that location and execute the instruction in the subroutine.
}

void ret(struct cpu *cpu)
{
  //return from subroutine. pop value from top of stack and store in pc.
  cpu->pc = pop(cpu);
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
    unsigned char operandA = cpu_ram_read(cpu, cpu->pc+1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->pc+2);
    printf("TRACE: %02X: %02X  %02X  %02X \n", cpu->pc, IR, operandA, operandB);
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    //int instruction_set_pc = (IR >> 4);
    switch(IR)
    {
      case LDI:
        //sets value in given register to value specified 
        cpu->reg[operandA] = operandB;
        //increment by 3 since 3 arguments (CP+2)
        cpu->pc += 3;
        break;
      case PRN:
        //print out numerically in given register 
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
        push(cpu, cpu->reg[operandA]);
        cpu->pc += 2;
        break;
      case POP:
        cpu->reg[operandA] = pop(cpu);
        cpu->pc += 2;
        break;
      case CALL:
        //call(cpu, operandA);
        push(cpu, cpu->pc + 2);
        cpu->pc = cpu->reg[operandA];
        //printf("TRACE: 0x%02X\n", cpu->pc);
        break;
      case RET:
        //ret(cpu);
        cpu->pc = pop(cpu);
        break;
      case CMP: 
        alu(cpu, ALU_CMP, operandA, operandB);
        break;
      case JMP:
        //jump to address stored in given register. set pc to address stored in given register. 
        cpu->pc = cpu->reg[operandA];
        break;
      case JEQ:
        //if equal flag set to true jump to address stored at given register.
        if (cpu->fl == 1)
        {
          cpu->pc = cpu->reg[operandA];
        }
        break;
      case JNE:
        //if equal flag is clear/false/0 jump to address stored at given register.
        if ((cpu->fl & 0b00000001) == 0)
        {
          cpu->pc = cpu->reg[operandA];
        }
        break;
      case HLT: 
        running = 0;  //set running to false
        break;
      default:  //program stops if gets instruction it doesnt know
        printf("unexpected instruction: 0x%0X2 at 0x%0X2 \n", IR, cpu->pc);
        exit(1);
    }
    // 6. Move the PC to the next instruction.
    // if (!instruction_set_pc) 
    // {
    //   cpu->pc+= ((IR >> 6) & 0x3) + 1;
    // }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->fl = 0;
  cpu->pc = 0;
  memset(cpu->reg, 0, sizeof cpu->reg);   //R7??????
  cpu->reg[7] = 0xf4; 
  memset(cpu->ram, 0, sizeof cpu->ram);
}
// When the LS-8 is booted, the following steps occur:
// * `R0`-`R6` are cleared to `0`.
// * `R7` is set to `0xF4`.
// * `PC` and `FL` registers are cleared to `0`.
// * RAM is cleared to `0`.