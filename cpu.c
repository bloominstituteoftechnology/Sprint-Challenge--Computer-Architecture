#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char MAR )
{
  return cpu->ram[MAR];
}

void cpu_ram_write(struct cpu *cpu,unsigned char MAR, unsigned char MDR)
{
  cpu->ram[MAR] = MDR;
}

void cpu_load(char *filename, struct cpu *cpu)
{
  int RAM_address = 0; // initalizes address counter to 0 to be used as index for where values will be stored within RAM
  FILE *fd; // initializes file descriptor pointer to fd
  char line[1024]; // temp variable to hold each line of the file being read
  int instruction_counter = 0;

  fd = fopen(filename, "r"); // open filename specified in argv and read file.

  if (fd == NULL) {
    printf("Cannot read file."); //print error if file cannot be read
    return;
  }

  while (RAM_address < 256 && fgets(line, sizeof line, fd) != NULL) { // while loop to loop through file and assign each line of file to RAM while loop exits once fgets returns NULL indicating EOF
    char *end_of_byte; //initialize pointer to end of byte
    unsigned char data = strtol(line, &end_of_byte, 2); // method used to only pull in a byte of information, and assign it to data.

    if (end_of_byte == line) { //if data and *line pointer equal one another continue; otherwise first iteration of fgets loads incorrect data.
      instruction_counter++; //increment instruction counter each time data equals *line (it will equal NULL at EOF)
      continue;
    }
    else {
      cpu->ram[RAM_address++] = data; // loads each line of data to a new RAM address location for cpu to read from when performing instructions.
    }
  }
  
  cpu->instruction_counter = instruction_counter; // sets incremented instruction counter to be used to prevent stack from overwriting instructions
  fclose(fd); //close file.
}

/**
 * ALU
 */
// void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
// {
//   switch (op) {
//     case ALU_MUL:
//       // TODO
//       break;

//     // TODO: implement more ALU ops
//   }
// }

void cpu_run(struct cpu *cpu)
{
  unsigned char IR, operandA, operandB, bit; // initializes IR, OpA, and OpB as "bytes"
  // int call;
  int instruction_index; // used to track location within RAM corresponding to sequential instructions.
  int running = 1; // True until we get a HLT instruction or if instruction fails to match switch case

  instruction_index = cpu->pc; // initialize index of instruction

  while (running) {
    IR = cpu_ram_read(cpu, instruction_index); // set IR to current value of instruction index
    operandA = cpu_ram_read(cpu, instruction_index + 1); // sets operandA to the next line below instruction
    operandB = cpu_ram_read(cpu, instruction_index + 2); // sets operandB to the second line below instruction, will only be used if needed in switch statement

    void load_immediate(int opA, int opB) {
      cpu->registers[opA] = opB; // save value in targeted register
    }

    int print(int opA) {
      return printf("%d\n", cpu->registers[opA]); // print value in targeted register
    }

    void multiply(int opA, int opB) {
      cpu->registers[opA] *= cpu->registers[opB]; // take value of register opA and multiply it by value in register opB and assign to register opA
    }

    void push(int opA) {
      cpu->registers[7]--; // decrement stack pointer to next stack address 
      if (cpu->ram[cpu->registers[7]] == cpu->ram[cpu->instruction_counter]) { // checks to insure stack pointer does not point to an address in RAM that an instruction was loaded
        printf("Stack overflow; recontemplate your life (or just inc ram or load a smaller program)");
      } else {
      cpu->ram[cpu->registers[7]] = cpu->registers[opA]; // loads value in register opA to current RAM address of stack
      }
    }

    int pop(int opA) {
      if(cpu->registers[7] == 0xF4) { // check if stack pointer is pointed to intial state
        return printf("Stack empty nothing to pop\n");
      } else {
        cpu->registers[opA] = cpu->ram[cpu->registers[7]]; // set register opA to value of RAM indicated by current stack pointer address
        cpu->registers[7]++; // increment stack pointer (removing access to the top the the stack)
      }
      return cpu->registers[opA]; // returns popped value
    }

    void add(int opA, int opB) {
      cpu->registers[opA] += cpu->registers[opB]; // add values of register opA and register opB and assign them to register opA
    }

    void jump (int opA) {
      instruction_index = cpu->registers[opA];
    }

     switch(IR) { // switch based on IR's instruction

      case LDI: //Set the value of a register to an integer.
        load_immediate(operandA, operandB);
        instruction_index += 3; // increments instruction index to next instruction line
        break; // breaks switch and re-runs loops

      case PRN: // print value of integer saved to register address
        print(operandA);
        instruction_index += 2; // increments instruction index to next instruction line
        break; // breaks switch and re-runs loops

      case MUL:
        multiply(operandA, operandB);
        instruction_index += 3; // increments instruction index to next instruction line
        break;

      case PUSH:
        push(operandA);
        instruction_index += 2; // increments instruction index to next instruction line
        break;

      case POP:
        pop(operandA);
        instruction_index += 2; // increments instruction index to next instruction line
        break;
          
      case HLT:
        running = 0; // kill while loop
        break;
    
      case ADD:
        add(operandA, operandB);
        instruction_index += 3; // increments instruction index to next instruction line
        break;

      case RET:
        instruction_index = cpu->ram[cpu->registers[7]]; // pop value from top of stack (presumably storing the last instruction and storing it to PC to continue running the program)
        cpu->registers[7]++; // increment SP to point to correct space in RAM corresponding to current stack location; effectively popping off stack value.
        break;

      case CALL:
        instruction_index += 2; // increments instruction index to next instruction line
        cpu->registers[7]--; // decrement SP to point to next place in RAM in order to create space to push current instruction
        cpu->ram[cpu->registers[7]] = instruction_index; // push next instruction to stack, continue running current program.
        instruction_index = cpu->registers[operandA]; // set instruction index to subroutine address loaded into passed register
        break;

      case CMP:
        if (cpu->registers[operandA] == cpu->registers[operandB]) {
          bit = (cpu->fl >> 0) & 1U; // sets bit variable to 0000 0001 <--- bit to check if it has been set to 1 already.
          if (bit == 1) { // if bit is 1 it's already set and this if statement need not do anything, increment and break switch case.
            instruction_index += 3;
            break;
          } else {
          cpu->fl ^= 1UL << 0; //if bit is not 1; bitwise operation to switch flag E from 0 to 1
          }

        } else if (cpu->registers[operandA] < cpu->registers[operandB]) {
          cpu->fl ^= 1UL << 2; // if statement is true, switch value of cpu->flag bit
        
        } else if (cpu->registers[operandA] > cpu->registers[operandB]) {
          cpu->fl ^= 1UL << 1; // if statement is true, switch value of cpu->flag bit

        } else {
          printf("CPU registers are not set properly, try again for comparison");
        }
        instruction_index += 3; // increment PC by manipulating instruction_index to next instruction
        break;

      case JMP:
        jump(operandA); //change instruction index to match register passed to JMP instruction
        break;

      case JEQ:
        bit = (cpu->fl >> 0) & 1U; // set bit variable to equal cpu-flag bit E
        if (bit == 1) { // if cpu-flag bit E is true or 1; jump to given register; otherwise increment program and continue.
          jump(operandA);
        } else {
          instruction_index += 2;
        }
        break;

      case JNE:
        bit = (cpu->fl >> 0) & 1U; // set bit variable to equal cpu-flag bit E
        if (bit == 0) { // if cpu-flag bit E is false or 0; jump to given register; otherwise increment program and continue.
          jump(operandA);
        } else {
          instruction_index += 2;
        }
        break;

      default:
       printf("Nothing to run, this is default of switch, current instruction: %u \n", IR);
       running = 0; // kill while loop
     } 
  }
}

void cpu_init(struct cpu *cpu)
{
  cpu->pc = 0; //sets pc to 0
  cpu->fl = 0; //sets flag to 0
  memset(cpu->registers, 0, sizeof cpu->registers); // uses memset to set register to 0
  memset(cpu->ram, 0, sizeof cpu->ram); // uses memset to set ram to 0

  cpu->registers[7] = 0xF4; // initialize the stack pointer to address 0xF4(244) to be used to locate top of stack in RAM
}
