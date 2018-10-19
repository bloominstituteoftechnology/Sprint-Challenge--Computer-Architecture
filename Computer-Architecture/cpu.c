#include <stdio.h>  // install libraries
#include <stdlib.h>
#include <string.h> // for memset string library
#include "cpu.h"

// #define DATA_LEN 6

// reading from the ram
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)  // helper function; return the address of the cpu
{
  return cpu->ram[address]; // return whatever the address of the ram; the cpu at this address will return the bytes that's there
}

//write to the ram
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)  // helper function; pass in a value since we are writing to the ram; return the address of the cpu and value
{
  cpu->ram[address] = value; 
}

// Pushes a value on the CPU stack.
void cpu_push(struct cpu *cpu, unsigned char value)
{
  cpu->reg[SP]--; // decrement value 
  cpu_ram_write(cpu, cpu->reg[SP], value);  // call ram write function to push a value on to the stack
  // cpu_ram_write(cpu, cpu->reg[SP], cpu->reg[operandA]);  // call ram write function passing in cpu, address we need to push it to which is stored in the Stack Pointer, value of store
}

// Pops a value on the CPU stack.
unsigned char cpu_pop(struct cpu *cpu)
{
  unsigned char value = cpu_ram_read(cpu, cpu->reg[SP]); // pop copies the value from the address pointed to by SP to the given register.
  // cpu->reg[operandA] = cpu_ram_read(cpu, cpu->reg[SP]); 
  cpu->reg[SP]++; // increment the stack pointer 
  
  // unsigned char value = cpu->ram[cpu, cpu->reg[SP]];  

  return value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)  // cpu load taking in cpu and filename
{
  FILE *fp = fopen(filename, "r");  // open a file; grab the pointer to the file and open it
  // printf("%s\n", filename);

  char line[1024]; // give a buffer or enough memory for each line to scroll the whole line; 

  int address = cpu->pc;
  
/*
  char data[DATA_LEN] = {
    // From print8.ls8
    0b10000010, // LDI R0,8; 1 operand
    0b00000000,
    0b00001000,
    0b01000111, // PRN R0; 2 operand
    0b00000000,
    0b00000001  // HLT; 0 operand
  };

  int address = 0;  // starting address 

  for (int i = 0; i < DATA_LEN; i++) 
  {
    cpu->ram[address++] = data[i];
  }
*/

  // TODO: Replace this with something less hard-coded
  // int address = 0;  // starting address 

  // read the file
  if(fp == NULL)  // check first if the pointer is not Null
  {
    fprintf(stderr, "Cannot open file. %s\n", filename);  // if null, print error
    exit(2);  // exit with code 2
  }

  char *endchar;  // to initialize endchar

  // Read all the lines and store them in RAM
  // use string to line, strlon(), function to read one line at a time
  while(fgets(line, sizeof line, fp) != NULL)
  {
    unsigned char byte = strtol(line, &endchar, 2); // strtol; &endchar to ignore spaces and bad chars

    // ignore empty lines
    if(endchar == line) 
    {
      continue;
    }

    // storing the bytes in our ram
    cpu_ram_write(cpu, address++, byte); 
  }
}

/**
 * ALU
 */
// doing ALU operations; ALU for Arithmetic Logic Unit which is the fundamental building blocks of the cpu
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) 
  {
    case ALU_MUL: // multiply
      // TODO
      // cpu->reg[regA] = cpu->reg[regA] * cpu->reg[regB];  // multiply values in register A and register B and store value in RegA
      cpu->reg[regA] *= cpu->reg[regB];
      break;

    // TODO: implement more ALU ops
    case ALU_ADD: // add      
      cpu->reg[regA] = cpu->reg[regA] + cpu->reg[regB]; 
      // cpu->reg[regA] += cpu->reg[regB]; 
      break;

    default:
      printf("Unknown ALU instruction at %02x: %02x\n", cpu->pc, op);
      exit(3);  // exit with code 3
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) 
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->pc); // instruction register; pass in cpu and the program count er to read off that; read from RAM at the PC; this is the instruction/ opcode

    // unsigned char operandA = cpu_ram_read(cpu, cpu->pc + 1); // openrandA will make 1 movement; reading at the next address of whatever the pc points at
    // unsigned char operandB = cpu_ram_read(cpu, cpu->pc + 2); // openrandB will make 2 movements; reading the next byte and store in operand B

    unsigned char operandA = cpu_ram_read(cpu, (cpu->pc + 1)& MAX_ADDR); // openrandA will make 1 movement; reading at the next address of whatever the pc points at
    unsigned char operandB = cpu_ram_read(cpu, (cpu->pc + 2)& MAX_ADDR); // openrandB will make 2 movements; reading the next byte and store in operand B


    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.

    // printf("TRACE: %02x: %02x\n", cpu->pc, IR);
    // int instruction_set_pc = (IR >> 4) & 1;
    
    switch (IR) { // switch on the IR 

      case PRN: // call print function; print numeric value in the given register
        printf("%d\n", cpu->reg[operandA]); // print whatever is in operandA
        // printf("print 8 manually\n");        
        // cpu->pc += 2;  // move the program counter by 2
        break;
      
      case LDI:   // LDI/load instruction
        // printf("We're about to load.\n");
        cpu->reg[operandA] = operandB; // operandA tells us the register number we need to store the value in; operandB is the value we need to store 
        // cpu->pc += 3; // increment the pc; LDI is 3 bytes
        break;
      
      case HLT: // halt method
        // printf("This is the halt method.\n"); // does not move the program counter
        running = 0;  // then running is equal false or 0
        break;
      
      case MUL: 
        cpu->reg[operandA] *= cpu->reg[operandB]; // mutiplication 
        // alu(cpu, ALU_MUL, operandA, operandB);
        cpu->pc += 3; // increment the pc; LDI is 3 bytes      

        // cpu->reg[regA] = cpu->reg[regA] * cpu->reg[regB];
        
        break;

      case ADD: 
        cpu->reg[operandA] += cpu->reg[operandB]; // mutiplication 
        cpu->pc += 3; // increment the pc; LDI is 3 bytes      
        break;
      
      case PUSH:  // add push function 
        cpu_push(cpu, cpu->reg[operandA]);  
        // cpu->reg[SP]--;  // decrement the value
        // cpu_ram_write(cpu, cpu->reg[SP], cpu->reg[operandA]);  // call ram write function passing in cpu, address we need to push it to which is stored in the Stack Pointer, value of store
        break;
      
      case POP:
        cpu->reg[operandA] = cpu_pop(cpu);  // cpu_pop copies the value from the address pointed to by SP to the given register.
        // cpu->reg[operandA] = cpu_ram_read(cpu, cpu->reg[SP]); 
        // cpu->reg[SP]++; // increment the stack pointer 
        break;
      
      case PRA:
        printf("%c\n", cpu->reg[operandA]);
        break;

      case CALL:  // Call; 
        cpu_push(cpu, cpu->pc + 2); // push the address of the next instructions; call is 2 bytes long; next instruction after the call is pc + 2
        cpu->pc = cpu->reg[operandA];   // set the pc to a certain value
        break;

      case RET: // return 
        cpu->pc = cpu_pop(cpu); // pop the value off the stack and set the pc to it
        break;

// Sprting Challenge: 

      case CMP:   // Compares register A and register B.        
        if (cpu->reg[operandA] == cpu->reg[operandB]) // If they are equal, 
        {
          cpu->fl = 1;  // set the flag to 1.
        } else
        {
          cpu->fl = 0;  // Else set it to 0.
        }
        break;

      case JMP: // Jumps to the address stored in the given register.         
        cpu->pc = cpu->reg[operandA]; // Sets the PC to the address stored in the given register.
        break;

      case JEQ: // If equal (E) flag is true, jump to the address in the given register.
        if (cpu->fl)  
        {
          cpu->pc = cpu->reg[operandA];
        }
        else
        {
          cpu->pc += 2;
        }
        break;

      case JNE:           
        if (!cpu->fl) // If equal (E) flag is clear or false, jump to the address stored in the given register.
        {
          cpu->pc = cpu->reg[operandA];
        }
        else
        {
          cpu->pc += 2;
        }
        break;
      
      default:  // default case; if something wrong happes; will catch anything that it doesn't know what to do
        printf("Unknown instruction at %02x: %02x\n", cpu->pc, IR); // if we get instruction doesn't know, this will tell us where and what it is; tell it the pc counter which it's pointing at
        exit(2);  // exit with code 2
    }

    // 4. Move the PC to the next instruction.
    // cpu->pc += (IR >> 6) + 1;

    int instruction_set_pc = (IR >> 4) & 1; // initialize instruction_set_pc

    if (!instruction_set_pc)  // if not 
    {
      cpu->pc += ((IR >> 6) & 0x3) + 1; // Shifts 6 places to the right.
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the Program Counter (PC) and other special registers
  // cpu->pc = 0;  // initialize/ load instructions at address 0
  cpu->pc = START_OF_PROGRAM_ADDR;  // initialize/ set to start of program address
  cpu->fl = CPU_FLAG;     // flag

  // cpu->reg[SP] = 0xf4;  // setup register; SP points at address F4 if the stack is empty

  cpu->reg[SP] = START_OF_STACK_ADDR; // Initialize the Stack Pointer (SP).
  cpu->reg[IM] = INTERRUPT_MASK;      // Initialize the Other special registers.
  cpu->reg[IS] = INTERRUPTS;

  // TODO: Zero registers and RAM
  memset(cpu->reg, 0, sizeof cpu->reg);  // grab the registers; we fill in the array with zeros
  memset(cpu->ram, 0, sizeof cpu->ram);  // grab the ram; we fill in the array with zeros
}
