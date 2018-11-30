#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address){
    return cpu-> ram[address]; 
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value){
  cpu-> ram[address] = value; 
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
   FILE *fptr; 
   char line[1024]; 
   unsigned char address = 0; 
   fptr = fopen(filename, "r"); 

   if(fptr == NULL){
     fprintf(stderr, "Error opening file");
     exit(1); 
   }

   while(fgets(line, sizeof line, fptr)!= NULL){
      // if(line[0] == "\n" || line[0] == "#"){
      //   printf("Commented line ignored \n"); 
      //   continue; 
      // }

      unsigned char b; 
      b = strtoul(line, NULL, 2); 
      cpu_ram_write(cpu, address++, b); 
   }
   fclose(fptr); 
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu-> registers[regA] *= cpu-> registers[regB];  // multiply value at both registers
      break;
    case ALU_ADD: 
      cpu-> registers[regA] += cpu-> registers[regB]; 
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

      unsigned char IR = cpu_ram_read(cpu, cpu-> PC); 
      unsigned char operandA = cpu_ram_read(cpu, (cpu-> PC + 1)); 
      unsigned char operandB = cpu_ram_read(cpu, (cpu-> PC + 2)); 

      int add_to_pc = (IR >> 6) + 1; 


      switch(IR){
        case LDI: // opcode for setting value of register to integer
          cpu-> registers[operandA] = operandB; //operands R0 = operandA and 8 = operandB
          break; 
        case PRN: // opcode for printing numeric value at the register
          printf("%d \n", cpu-> registers[operandA]); 
          break; 
        case HLT: // opcode to exit the emulator
          running = 0; 
          break; 
        case MUL: // opcode to multiply values in registers
          alu(cpu, ALU_MUL, operandA, operandB); 
          break; 
        case PUSH: //opcode to push values onto the CPU stack
          // start by initializing the stack pointer at register 7
          // according to the spec it will point to address F4 if current address is empty
          // decrement the stack pointer ram index by one 
          // OperandA is the register which holds the value to push 
          // set the value at the stack pointer index in ram to the value in operandA register
          cpu->registers[7] -= 1; 
          cpu-> ram[cpu->registers[7]] = cpu->registers[operandA]; 
          break; 
        case POP: //opcode to pop values of the CPU stack
          // at the stack pointer take the value and store it in register 0
          // increment the stack pointer 
          // return the popped value 
          cpu->registers[operandA] = cpu-> ram[cpu->registers[7]]; 
          cpu->registers[7]++; 
          break; 
        case ADD: // opcode to add two values in registers
          alu(cpu, ALU_ADD, operandA, operandB); 
          break; 
        case CALL:
          cpu->registers[7] -= 1; //decrement the stack pointer by one
          cpu-> ram[cpu->registers[7]] = cpu-> PC + 2;  // address of next instruction saved in stack, program counter increased 2 bytes
          cpu-> PC = cpu->registers[operandA]; // set Program Counter to the subroutine 
          add_to_pc = 0; 
          break; 
        case RET: 
          cpu-> PC = cpu-> ram[cpu-> registers[7]]; //retrieve the address of the next instruction before the call which should be at the top of the stack and set the program counter to it
          cpu-> registers[7] += 1;  // increment the stack pointer
          add_to_pc = 0; 
          break; 
      }
      
    
    cpu-> PC += add_to_pc; 
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu-> PC = 0; 

  //  function memset copies the character c (an unsigned char) to the first n characters of the string pointed to, by the argument str.
  memset(cpu-> ram, 0, sizeof cpu-> ram); 
  memset(cpu-> registers, 0, sizeof cpu-> registers); 
}
