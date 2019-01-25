#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DATA_LEN 6
#define SP 7
#define LESS 00000100
#define GREATER 00000010
#define EQUAL 00000001

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  int address = 0; // initialize the ram address we'll use

  FILE *file = fopen(filename, "r"); // open the filename passed in argv

  char data[128];

  if(file == NULL){
    fprintf(stderr, "No file with that filename was found.\n");
    exit(1);
  }

  // for each line in the binary file, push it to the ram 
  while(fgets(data, sizeof(data), file) != NULL){
    // printf("binary: %s", data);
    // convert each binary string into numbers
    char *ptr;
    unsigned char ret = strtol(data, &ptr, 2);

    // ignore lines where no valid binary values are found
    if(ptr == data){
      continue;
    }

  // printf("conversion: %02X\n \n", ret);

  // write the return value to ram
  cpu->ram[address] = ret;
  address++;

  }

  fclose(file);
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}


void cpu_ram_write(struct cpu *cpu, unsigned char value, unsigned char address)
{
  cpu->ram[address] = value;
}


void cpu_push(struct cpu *cpu, unsigned char value){ // abstract push for use in CALL
    cpu->registers[SP]--;  
    cpu->ram[cpu->registers[SP]] = value;
    // printf("Pushed %d to ram address %d.\n", value, cpu->registers[SP]);
  }

unsigned char cpu_pop(struct cpu *cpu){ // abstract pop for use in RET
  unsigned char new_address = cpu->ram[cpu->registers[SP]];
  // cpu->registers[address] = cpu->ram[cpu->registers[SP]];
  // printf("Popping %d from ram address %d.\n", cpu->ram[cpu->registers[SP]], cpu->registers[SP]);
  cpu->registers[SP]++;
  return new_address;
}

unsigned char cpu_compare(unsigned char valA, unsigned char valB){
  unsigned char result;
  if(valA > valB){
    result = GREATER;
    return result;
  } else if (valA < valB){
    result = LESS;
    return result;
  } else if(valA == valB){
    result = EQUAL;
    return result;
  }

  return result = 0;
}

void cpu_jump(struct cpu *cpu, unsigned char operandA){
  // printf("Jumping to register %d\n", operandA);
  cpu->PC = cpu->registers[operandA] - 2;
}

void cpu_jeq(struct cpu *cpu, unsigned char operandA){
  if(cpu->FL == EQUAL){
    // printf("EQUAL true, jumping to register %d\n", operandA);
    cpu_jump(cpu, operandA);
  }
}

void cpu_jne(struct cpu *cpu, unsigned char operandA){
  if(cpu->FL != EQUAL){
    // printf("EQUAL false, jumping to register %d\n", operandA);
    cpu_jump(cpu, operandA);
  }
}

long dec_to_bin(unsigned char value){
  int rem;
  long binary = 0, i = 1;
  
  while(value != 0){
    rem = value % 2;
    value = value/2;
    binary = binary + (rem * i);
    i = i*10;
  }

  return binary;
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      cpu->registers[regA] *= cpu->registers[regB];
      printf("Multiply has finished.\n");
      break;

    case ALU_ADD:
      // printf("Adding %d + %d\n", cpu->registers[regA], cpu->registers[regB]);
      cpu->registers[regA] += cpu->registers[regB];
      // printf("Add has finished.\n");
      break;

    case ALU_SUB:
      break;

    case ALU_DIV:
      break;

    case ALU_INC:
      break;

    case ALU_DEC:
      break;

    case ALU_CMP:
      // printf("ALU compare: %d, %d\n", regA, regB);
      cpu->FL = cpu_compare(regA, regB);
      // printf("FL set to %d\n", cpu->FL);
      break;

    case ALU_AND:
      cpu_push(cpu, (regA & regB));
      break;

    case ALU_OR:
    // push the bitwise operation answer to the stack
      cpu_push(cpu, (regA | regB));
      break;

    case ALU_XOR:
      cpu_push(cpu, (regA ^ regB));
      break;

    default:
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
  

  while (running) {
    // TODO
    unsigned char operandA, operandB; 
    // 1. Get the value of the current instruction (in address PC).
    unsigned char instruction = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    unsigned int num_operands = instruction >> 6;
    // 3. Get the appropriate value(s) of the operands following this instruction

    if(num_operands == 2){
      
      operandA = cpu_ram_read(cpu, (cpu->PC+1) & 0xff),
      operandB = cpu_ram_read(cpu, (cpu->PC+2) & 0xff);
      // printf("Two operands, reg: %d, val: %d\n", operandA, operandB);
    } else if (num_operands == 1){
      operandA = cpu_ram_read(cpu, (cpu->PC+1) & 0xff);
      // printf("One operand, %d\n", operandA);
    }
    // 4. switch() over it to decide on a course of action.

    switch(instruction){

      case HLT:
        printf("\nHLT received at line %d, ending program.\n", cpu->PC);
        running = 0;
        break;

      case PRN:
      // print what is in the specified register
        printf("\nOUTPUT: %d\n", cpu->registers[operandA]);
        break;

      case LDI:
        cpu->registers[operandA] = operandB;
        // printf("LDI: A: %d B: %d\n", operandA, operandB);
        break;

      case ADD:
        // printf("ADD was called to handle %d and %d.\n", cpu->registers[operandA], cpu->registers[operandB]);
        alu(cpu, ALU_ADD, operandA, operandB);
        break;

      case MUL:
        printf("MUL was called.\n");
        alu(cpu, ALU_MUL, operandA, operandB);
        // alu takes in cpu, operation, and value A and B.
        // mult.ls8 does the following: R0 is set to value A. R1 is set to value B. MUL is called on R0 and R1. R0 is printed. 
        // Our answer should be in R0 when print is called.
        break;

      case PUSH: // push the value in the given register on the stack
        // 1. decrement the SP i.e. R7. We decrement because R7 sits at the top of the stack and the stack builds down.
        // 2. copy the value in the given register to the address pointed to by SP

        // OLD:
        // cpu->registers[SP]--;
        // cpu->ram[cpu->registers[SP]] = cpu->registers[operandA];

        // NEW:
        cpu_push(cpu, cpu->registers[operandA]);
        break;

      case POP: // pop the value at the top of the stack into the given register
        // 1. copy the value from the address pointed to by SP i.e. R7 to the given register
        // 2. increment SP

        // OLD:
        // cpu->registers[operandA] = cpu->ram[cpu->registers[SP]];
        // cpu->registers[SP]++;

        // NEW:
        cpu->registers[operandA] = cpu_pop(cpu);
        break;

      // CALL wil push the address of the instruction after it on the stack, then move the PC
      // to the subroutine address
      case CALL:
        // printf("CALL: Pushing return address %d to stack for next value %d\n", cpu->PC+1, operandB);
        cpu_push(cpu, cpu->PC+1);

        // printf("CALL: Jumping to subroutine address %d\n", cpu->registers[operandA] - 2);
        cpu->PC = cpu->registers[operandA] - 2;
        break;
      
      // RET will pop the return address off the stack and store it in the PC
      case RET:
        cpu->PC = cpu_pop(cpu);
        // printf("PC is now set to %d.\n", cpu->PC);
        break;

      case CMP:
        // printf("Compare %d with %d.\n", cpu->registers[operandA], cpu->registers[operandB]);
        alu(cpu, ALU_CMP, cpu->registers[operandA], cpu->registers[operandB]);
        break;

      case JMP:
        // jump the PC to the address stored in the given register
        cpu_jump(cpu, operandA);
        break;

      case JEQ:
        // jump the PC to the given register if cpu->FL is == EQUAL
        cpu_jeq(cpu, operandA);
        break;

      case JNE:
        cpu_jne(cpu, operandA);
        break;

      case AND:
        alu(cpu, ALU_AND, cpu->registers[operandA], cpu->registers[operandB]);
        cpu->registers[operandA] = cpu_pop(cpu);
        break;

      case OR:
        alu(cpu, ALU_OR, cpu->registers[operandA], cpu->registers[operandB]);
        // pop the bitwise answer off the stack
        cpu->registers[operandA] = cpu_pop(cpu);
        break;

      case XOR:
        alu(cpu, ALU_XOR, cpu->registers[operandA], cpu->registers[operandB]);
        cpu->registers[operandA] = cpu_pop(cpu);
        break;

      default:
        break;
    }
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    cpu->PC += num_operands + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{

  // R0-R6 are cleared to 0
  memset(cpu->registers, 0, sizeof(cpu->registers));

  // R7 is set to 0xF4
  cpu->registers[SP] = 0xF4;

  // PC and FL registers are cleared to 0
  cpu->PC = 0;
  cpu->FL = 0;

  // RAM is cleared to 0
  memset(cpu->ram, 0, sizeof(cpu->ram));

  // The program can now be loaded into RAM starting at address 0x00.
  printf("CPU Initilized Successfully!\n");


}
