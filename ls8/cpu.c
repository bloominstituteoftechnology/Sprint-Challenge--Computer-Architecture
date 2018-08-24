#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *my_cpu, char* file_name)
{


  FILE *fp;

  if ((fp = fopen(file_name, "r")) == NULL) {
    printf("You done entered the wrong thing.");
    exit(1);
  }




  char data[256];

  int data_pos = 0;

  while (fgets(data, sizeof(data), fp) != NULL) {
    char *ptr;
    unsigned char byte = strtoul(data, &ptr, 2);
    if (ptr == data) {
      continue;
    }
    my_cpu->ram[data_pos++] = byte;
  }


  fclose(fp);

  // int address = 0;


  // TODO: Replace this with something less hard-coded
}


// Make cpu_ram_read and cpu_ram_write
// they do what they sound like

unsigned char cpu_ram_read(struct cpu *my_cpu) {
  return my_cpu->ram[my_cpu->PC];
}

void cpu_ram_write(struct cpu *my_cpu, int index, unsigned char new_symbol) {
  my_cpu->ram[index] = new_symbol;
}

/**
 * ALU
 */
void alu(struct cpu *my_cpu, enum alu_op op, unsigned char reg_a, unsigned char reg_b)
{
  switch (op) {
    case ALU_MUL:
      my_cpu->registers[reg_a] *= my_cpu->registers[reg_b];
      break;
    case ALU_ADD:
      my_cpu->registers[reg_a] += my_cpu->registers[reg_b];
      break;
    case ALU_CMP: ;
      int result = my_cpu->registers[reg_a] - my_cpu->registers[reg_b];
      if (result == 0) {
        my_cpu->FL = 1;
      }
      else if (result > 0) {
        my_cpu->FL = 2;
      }
      else {
        my_cpu->FL = 4;
      }

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *my_cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {


    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(my_cpu);
    // 2. switch() over it to decide on a course of action.

    //This gives us the number of arguments to expect
    unsigned char num_args = (IR >> 6) & 0b11;

    //We'll hold the arguments in here
    unsigned char args[2];

    //Loop for the expected number of arguments and read from each
    //After incrementing PC to read the appropriate slot
    for (int i = 0; i < num_args; i++) {
      my_cpu->PC++;
      args[i] = cpu_ram_read(my_cpu);
    }

    switch(IR) {
      case LDI:
        my_cpu->registers[args[0]] = args[1];
        my_cpu->PC++;
        break;
      case PRN:
        printf("%d\n", my_cpu->registers[args[0]]);
        my_cpu->PC++;
        break;
      case HLT:
        running = 0;
        break;
      case MUL:
        alu(my_cpu, ALU_MUL, args[0], args[1]);
        my_cpu->PC++;
        break;
      case POP:
        my_cpu->registers[args[0]] = my_cpu->ram[my_cpu->registers[7]++];
        my_cpu->PC++;
        break;
      case PUSH:
        my_cpu->ram[--my_cpu->registers[7]] = my_cpu->registers[args[0]];
        my_cpu->PC++;
        break;
      case CALL:
        my_cpu->ram[--my_cpu->registers[7]] = ++my_cpu->PC;
        my_cpu->PC = my_cpu->registers[args[0]];
        break;
      case RET:
        my_cpu->PC = my_cpu->ram[my_cpu->registers[7]++];
        break;
      case ADD:
        alu(my_cpu, ALU_ADD, args[0], args[1]);
        my_cpu->PC++;
        break;
      case ST:
        my_cpu->ram[my_cpu->registers[args[0]]] = my_cpu->registers[args[1]];
        my_cpu->PC++;
        break;
      case CMP:
        alu(my_cpu, ALU_CMP, args[0], args[1]);
        my_cpu->PC++;
        break;
      case JEQ:
        if ((my_cpu->FL & 0b1) == 0b1) {
          my_cpu->PC = my_cpu->registers[args[0]];
        }
        else {
          my_cpu->PC++;
        }
        break;
      case JMP:
        my_cpu->PC = my_cpu->registers[args[0]];
        break;
      case JNE:
        if ((my_cpu->FL & 0b1) == 0b0) {
          my_cpu->PC = my_cpu->registers[args[0]];
        }
        else {
          my_cpu->PC++;
        }
        break;
      default:
        printf("wut?\n");
        exit(1);
    }

    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *my_cpu)
{
  // TODO: Initialize the PC and other special registers
  // TODO: Zero registers and RAM
  my_cpu->PC = 0b00000000;
  my_cpu->registers = (unsigned char *)calloc(8, sizeof(unsigned char));
  my_cpu->ram = (unsigned char *)calloc(256, sizeof(unsigned char));
  my_cpu->FL = 0b00000000;
}
