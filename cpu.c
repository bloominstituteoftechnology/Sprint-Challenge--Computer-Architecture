#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 255
#define STACK_POINTER 7  // Define the register reserved for the stack pointer
#define IM 5 // Interrupt mask
#define IS 6 // Interrupt status



// Declare an array of pointers to functions, add functions to array in cpu_init
int (*instructions[DATA_LEN]) (struct cpu * cpu, unsigned char regA, unsigned char regB) = {0};

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char* fileName)
{
  char data[DATA_LEN];

  FILE* file_ptr;

  if((file_ptr = fopen(fileName, "r")) == NULL)
  {
    printf("Error opening file!\nPlease include a relative path with filename\n");
    exit(1);
  }

  int i = 0;
  while(fgets(data, sizeof(data), file_ptr) != NULL)
  {
    char * endptr;

    unsigned char num;
    num = strtoul(data, &endptr, 2);

    if(endptr == data)
    {
      continue;
    }

    cpu_ram_write(cpu, i, num);     // Write binary instructions to ram

    i++;
  }
  
  fclose(file_ptr);
}

/**
 * ALU
 */
int alu(struct cpu *cpu, unsigned char op, unsigned char regA, unsigned char regB)
{
  switch (op) 
  {
      case MUL:
      {
        unsigned char product = cpu->registers[regA] * cpu->registers[regB];
        cpu->registers[regA] = product;
        return 1;
        break;
      }
      case ADD:
      {
        unsigned char sum = cpu->registers[regA] + cpu->registers[regB];
        cpu->registers[regA] = sum;
        return 1;
        break;
      }
        
      default:
        return 1;

  }
}


/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{

  int running = 1; // True until we get a HLT instruction

  while (running) {

    // Get the value of the current instruction (in address PC).
    int pc = cpu->pc;
    unsigned char binary_instruction = cpu_ram_read(cpu, pc);
    unsigned char operandA = cpu_ram_read(cpu, pc + 1);
    unsigned char operandB = cpu_ram_read(cpu, pc + 2);
    
    // Set and check our instruction handler

    int (*handler)(struct cpu*, unsigned char, unsigned char);

    handler = instructions[binary_instruction];

    if(handler == 0)
    {

      printf("Invalid instruction PC: %02X entered, exiting program.\n", pc);
      break;

    }

    // Do whatever the instruction should do according to the spec.
    
    if(((binary_instruction >> 4) & 0x1) != 1)  // If we have an instruction which DOES NOT change the pc (indicated by bit 000x0000)
    {
      
      running = handler(cpu, operandA, operandB);   // Returns 1 if continue, 0 if halt
      cpu->pc += (int) (binary_instruction >> 6) + 1;

    }
    else// If we have an instruction which DOES change the PC
    {

      cpu->pc = handler(cpu, operandA, operandB);   // Returns the pc position to jump to

    }

  }
}

// Instruction Handlers
unsigned char cpu_ram_read(struct cpu *cpu, int mar)
{

  return cpu->ram[mar]; //mar = memory address register

}

void cpu_ram_write(struct cpu *cpu, int mar, unsigned char value)
{

  cpu->ram[mar] = value;
  
}

int handle_LDI(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  cpu->registers[regA] = regB;
  return 1;
  
}

int handle_PRN(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  (void)regB;
  printf("\nValue at register %d is: %d\n", regA, cpu->registers[regA]);
  return 1;
  
}

int handle_MUL(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  alu(cpu, MUL, regA, regB);
  return 1;
  
}

int handle_ADD(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  alu(cpu, ADD, regA, regB);
  return 1;
  
}

int handle_PUSH(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  (void)regB;
  cpu_ram_write(cpu, --cpu->registers[STACK_POINTER], cpu->registers[regA]); // Decrement stack pointer first, then write value in register A to stack (according to spec)
  return 1;
  
}

int handle_POP(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  (void)regB;
  unsigned int stack_val = cpu_ram_read(cpu, cpu->registers[STACK_POINTER]++); // Get the value at stack pointer, then increment
  handle_LDI(cpu, regA, stack_val);               // Set the desired register = popped value
  return 1;

}

int handle_HALT(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  (void)cpu;
  (void)regA;
  (void)regB;
  return 0;

}

int handle_CALL(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  (void)regB;
  // Push address of next instruction to the stack
  cpu_ram_write(cpu, --cpu->registers[STACK_POINTER], cpu->pc + 2);
  return cpu->registers[regA];                                            // return the value in register A (the pc value of the called function)

}

int handle_RET(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  (void)regA;
  (void)regB;
  // Pop pc address of next instruction off of the stack
  unsigned int return_pc = cpu_ram_read(cpu, cpu->registers[STACK_POINTER]++); // Get the value at stack pointer, then increment
  return return_pc;                                            // return the pc value of the address just after the previous call

}

int handle_ST(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  // Store value in registerB in the address stored in registerA.
  cpu_ram_write(cpu, cpu->registers[regA], cpu->registers[regB]);
  return 1;

}

int handle_CMP(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  // Clear the current flags
  cpu->FL &= 0;

  // |= is or equals, sets any 1 to true
  if(cpu->registers[regA] < cpu->registers[regB])
  {
    cpu->FL |= 0b0000100;
  }
  else if(cpu->registers[regA] > cpu->registers[regB])
  {
    cpu->FL |= 0b0000010;
  }
  else if(cpu->registers[regA] == cpu->registers[regB])
  {
    cpu->FL |= 0b0000001;
  }

  return 1;

}

int handle_JMP(struct cpu* cpu, unsigned char regA, unsigned char regB)
{
  
  (void)regB;
  // Jump to the address stored in the given register
  return cpu->registers[regA];

}

int handle_JEQ(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  // If equal flag is set (true), jump to the address stored in the given register
  if((cpu->FL & 1) == 1)
  {
    return handle_JMP(cpu, regA, regB);
  }
  // If not equal, increment pc by 2 and return value
  int move_increment = 2;
  return cpu->pc + move_increment;

}

int handle_JNE(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  // If E flag is clear (false, 0), jump to the address stored in the given register
  if((cpu->FL & 1) != 1)
  {
    return handle_JMP(cpu, regA, regB);
  }
  // If equal, increment pc by 2 and return value
  int move_increment = 2;
  return cpu->pc + move_increment;

}



/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // Place in ram where stack memory begins
  int BEGIN_STACK = 0xF4;

  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;
  cpu->FL = 0;

  // Zero registers and RAM
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->registers, 0, sizeof cpu->registers);

  cpu->registers[STACK_POINTER] = BEGIN_STACK;

  // Assign our functions to the array of function handlers
  instructions[LDI] = handle_LDI;
  instructions[PRN] = handle_PRN;
  instructions[MUL] = handle_MUL;
  instructions[ADD] = handle_ADD;
  instructions[PUSH] = handle_PUSH;
  instructions[POP] = handle_POP;
  instructions[HLT] = handle_HALT;
  instructions[CALL] = handle_CALL;
  instructions[RET] = handle_RET;
  instructions[ST] = handle_ST;
  instructions[CMP] = handle_CMP;
  instructions[JMP] = handle_JMP;
  instructions[JEQ] = handle_JEQ;
  instructions[JNE] = handle_JNE;


}
