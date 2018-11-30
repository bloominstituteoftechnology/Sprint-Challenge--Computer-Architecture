#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6

/*general
accessing a value in a register:  cpu->registers[operandA]
*/

/* stack pointer (SP) needs to be 
initialized with some value and set 
as point to a memory location
*/

// *** push function for cpu stack
// parameters: cpu struct and input value
void cpu_push(struct cpu *cpu, unsigned char value)
{
  // decrement the SP
  cpu->registers[SP]--;
  // set the value in the given register to the address indicated by SP
  cpu->ram[cpu->registers[SP]] = value;
}

// *** pop function for cpu stack
// parameter: is the cpu struct
unsigned char cpu_pop(struct cpu *cpu)
{
// copy the value from the address pointed to by SP to the given register
  unsigned char value = cpu->ram[cpu->registers[SP]];
  // increment the SP
  cpu->registers[SP]++;
  // return the value popped
  return value;
}

// Helper functions cpu_ram_read() and cpu_ram_write()
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar){
  return cpu->ram[mar];
}

void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr){
  cpu->ram[mar] = mdr;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

void cpu_load(struct cpu *cpu, char *filename)
{
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   // need to parse an external file
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

  // // TODO: Replace this with something less hard-coded

  FILE *fp = fopen(filename, "r");
  char line[255];
  unsigned char address = 0x00;

  if(fp == NULL) {
    fprintf(stderr, "error opening file %s\n", filename);
    exit(2); //comes from stdlib
  }

  while(fgets(line, sizeof line, fp) != NULL){
    unsigned char b = strtol(line, NULL, 2);
    cpu_ram_write(cpu,address++, b);
  }
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{

  switch (op) {
    case ALU_MUL:
      cpu->registers[regA] *= cpu->registers[regB];
      break;

    // TODO: implement more ALU ops
    case ALU_ADD:
      cpu->registers[regA] += cpu->registers[regB];
      break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  printf("\nCPU running\n");

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu->ram[cpu->PC];
    unsigned char operandA = cpu->ram[(cpu->PC + 1) & 0xFF];
    unsigned char operandB = cpu->ram[(cpu->PC + 2) & 0xFF];

    // 2. switch() over it to decide on a course of action.
    // printf("%i\n", cpu->PC);  // displays every instruction on a new line

    switch(IR) {
      // 3. Do whatever the instruction should do according to the spec.
      case LDI:
      printf("\n\n ....... LDI ...... \n");
      printf("Register A: %d\n", cpu->registers[operandA]);
      printf("Register B: %d\n", cpu->registers[operandB]);
      //set the value of a register to an integer
      cpu->registers[operandA] = operandB;
      cpu->PC += 3;
      break;

      case PRN:
      printf(" \n\n ....... PRN ...... \n");
      //print to the console the decimal integer value stored in given register
      printf(" -------------------------------------------------------Output: %d\n", cpu->registers[operandA]);
      cpu->PC += 2;
      break;

      case HLT:
      printf(" \n\n ....... HLT ...... \n");
      running = 0;
      break;

      case PUSH:
      printf(" \n\n ...... PUSH ...... \n");
      cpu_push(cpu, cpu->registers[operandA]);
      cpu->PC += 2;
      break;
      
      case POP:
      printf(" \n\n ...... POP ...... \n");
      cpu->registers[operandA] = cpu_pop(cpu);
      cpu->PC += 2;
      break;
        
      /* RET - takes us to the instruction after the function call
      pops the value off the top of the stack and stores directly in PC
      then we halt
      */
      case RET:
      printf(" \n\n ...... RET ...... \n");
      cpu->PC = cpu_pop(cpu); //pops the most recent item from the stack
      // cpu->PC += 1;
      break;  

      /* CALL - is two bytes long, so the next instruction must be 
      two bytes down. PC+2 will give the correct address

      i.e. trnsfer the address of the next instruction to the stack
      (skip the instructions in between)

      use cpu_push(cpu, value)

      */
      case CALL:
      printf(" \n\n ...... CALL ...... \n");
      cpu_push(cpu, cpu->PC+2);
      printf("what is operandA: %d\n", cpu->registers[operandA]);
      cpu->PC = cpu->registers[operandA];
      // cpu->PC += 2;
      break;


      case JMP:
      printf(" \n\n ...... JMP ...... \n");
      //Jump to the address stored in the given register.
      cpu->PC = cpu->registers[operandA];
      // don't increment the PC because we have already set the PC value explicitly
      break;
  

      //  CMP Compare the values in two registers.
      case CMP:
      printf(" \n\n ...... CMP ...... \n");
      printf("Register A: %d\n", cpu->registers[operandA]);
      printf("Register B: %d\n", cpu->registers[operandB]);
      printf("Initial value of the Equal flag: %d\n", cpu->flags[0]);
      printf("PC location: %d\n", cpu->PC);
      //  If they are equal, set the Equal `E` flag to 1, otherwise set it to 0. 
      if (cpu->registers[operandA] == cpu->registers[operandB]){
        cpu->flags[0] = 1;
        printf("E flag: %d\n", cpu->flags[0]);
      }
      
      // If registerA is less than registerB, set the Less-than `L` flag to 1, otherwise set it to 0.
      else if (cpu->registers[operandA] < cpu->registers[operandB]){
        cpu->flags[1] = 1;
        printf("L flag: %d\n", cpu->flags[1]);
      }
      //If registerA is greater than registerB, set the Greater-than `G` flag to 1, otherwise set it to 0.
      else {
        cpu->flags[2] = 1;
        printf("G flag: %d\n", cpu->flags[2]);
      } 
      cpu->PC += 3;
      printf("PC location: %d\n", cpu->PC);
      break;
      
      // JEQ - conditional jump - jump when equal
      case JEQ:
      printf(" \n\n ...... JEQ ...... \n");
      printf("Register A: %d\n", cpu->registers[operandA]);
      printf("Register B: %d\n", cpu->registers[operandB]);
      printf("E flag: %d\n", cpu->flags[0]);
      printf("PC location: %d\n", cpu->PC);
      // If `equal` flag[0] is set (true, 1), jump to the address stored in the given register.
      if (cpu->flags[0] == 1){
        cpu->PC = cpu->registers[operandA];
        printf("E flag: %d\n", cpu->flags[0]);
        }
      else{
        cpu->PC += 2; // go to the next instruction
        printf("PC location: %d\n", cpu->PC);
      };
      break;
    

      // JNE - conditional jump - jump when not equal
      case JNE:
      printf(" \n\n ...... JNE ...... \n");
      printf("Register A: %d\n", cpu->registers[operandA]);
      printf("Register B: %d\n", cpu->registers[operandB]);
      printf("E flag: %d\n", cpu->flags[0]);
      printf("PC location: %d\n", cpu->PC);
      // If `E` flag[0] is clear (false, 0), jump to the address stored in the given register. 
      if (cpu->flags[0] == 0){
        cpu->PC = cpu->registers[operandA];
        printf("E flag: %d\n", cpu->flags[0]);
        }
      else{
        cpu->PC += 2;
      };
      break;
      }
      // 4. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  // printf("\nCPU init\n");
  cpu->PC = 0;
  // TODO: Zero registers and RAM
  memset(cpu->ram, 0, sizeof(cpu->ram));
  memset(cpu->registers, 0, sizeof(cpu->registers));
  memset(cpu->flags, 0, sizeof(cpu->flags));

  // Initialize stack pointer
  cpu->registers[SP] = 0xF4;
}

