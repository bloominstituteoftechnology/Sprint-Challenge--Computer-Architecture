#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index){
  return cpu->ram[index];
};
  
void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char new){
  //assign that index above to value
  // return cpu->ram[index] = new;
  cpu->ram[index] = new;
};

void cpu_load(struct cpu *cpu, char *argv)
{
  // printf("argv -> %s\n", argv);
  FILE * fp;
  fp = fopen(argv, "r");
  char str[30];
  // fp = fopen("./somefile.txt", "r");
  if (fp == NULL)
      exit(EXIT_FAILURE);
  int address = 0;
  while( fgets(str, sizeof(str), fp) != NULL )
  {
      unsigned char binary;
      char *endptr;
      binary = strtoul(str, &endptr, 2);
      if (str != endptr) {
        // printf("%d", binary);
        cpu->ram[address++] = binary;
      }
  }
  // printf("cpu at address 1: %d\n\n", cpu_ram_read(cpu, 0));

  fclose(fp);
  // exit(EXIT_SUCCESS);
}

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
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
//, int * regNum
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    int IR = cpu->ram[cpu->PC];//this is the slot in ram

    unsigned int value1 = cpu_ram_read(cpu, cpu->PC+1); //the register index where the second should be stored 
    unsigned int value2 = cpu_ram_read(cpu, cpu->PC+2);//the second number

    //value at register zero   

    // 1. Get the value of the current instruction (in address PC).
    switch(IR){
      case ADD:
        printf("ADD\n");
        alu(cpu, ALU_ADD, value1, value2);
        cpu->PC += 3;
        break;
      case CALL:
            // printf("CALLcurrentpointer %u\n", cpu->PC);
            //value 1 pushed onto stack
            cpu->reg[7]--;
        cpu->ram[cpu->reg[7]] = cpu->PC+2;
            //pc is set to value 1 
            printf("CALL return to line -> %u\n", cpu->PC+3);
        cpu->PC = cpu->reg[value1];
            // printf("CALL3newpointer %u\n", cpu->PC);
        break;
      case CMP:
            printf("CMP %u to %u\n", cpu->reg[value1], cpu->reg[value2]);
        if(cpu->reg[value1] == cpu->reg[value2]){
              printf("%u is = %u\n", cpu->reg[value1], cpu->reg[value2]);
          cpu->FL = 0b00000001;
        } else if (cpu->reg[value1] < cpu->reg[value2]){
              printf("%u is < %u\n", cpu->reg[value1], cpu->reg[value2]);
          cpu->FL = 0b00000100;
        } else {
              printf("%u is > %u\n", cpu->reg[value1], cpu->reg[value2]);
          cpu->FL = 0b00000010;
        }
        cpu->PC+=3;
            // printf("CMPend %u\n", cpu->PC);
        break;
      case DEC:
        cpu->reg[value1]--;
            printf("decrement reg%u to %u\n", value1, cpu->reg[value1]);
        cpu->PC+=2;
        break;
      case HLT:
            printf("\nHalting Program.\n");
        exit(0); //terminates the whole program insetead of exiting switch... exit is kinda oppositeish
        break;
      case INC:
            printf("incrementing reg%u to %u\n", value1, cpu->reg[value1]+1);
        cpu->reg[value1]++;
        cpu->PC+=2;
        break;
      case JEQ:
            printf("JEQ is %u == %u?\n", cpu->FL, 0b00000001);
        if(cpu->FL == 0b00000001){
              printf("yes\n");
          cpu->PC = cpu->reg[value1];
        } else {
              printf("no\n");
          cpu->PC+=2;
        }
            // printf("JEQend %u\n", cpu->PC);
        break;
      case JMP:
            // printf("JMP to line %u\n", cpu->reg[cpu->PC+1]);
            printf("JMP to line %u\n", cpu->reg[value1]);
        cpu->PC = cpu->reg[value1];
        break;
      case LD:
        cpu->reg[value1] = cpu->reg[value2];
            printf("LD loads reg%u with %u\n", value1, cpu->reg[value2]);
        cpu->PC += 3;
        break;
      case LDI:
        cpu->reg[value1] = value2;
        cpu->PC += 3;
            printf("LDI reg%u = %u\n", value1, value2);
            printf("value at reg%u = %c\n", value1, cpu->reg[value1]);
        break;
      case MUL:
            printf("MUL\n");
        alu(cpu, ALU_MUL, value1, value2);
        cpu->PC += 3;
        break;
      case POP:
            printf("POP\n");
        cpu->reg[value1] = cpu->ram[cpu->reg[7]];
        cpu->reg[7]++;
        cpu->PC += 2;
        break;
      case PRA:
            printf("\n------------------------------------PRA --%c--\n", cpu->reg[value1]);
        cpu->PC += 2;
        break;
      case PRN:
            printf("The number printed is %u\n", cpu->reg[value1]);
        cpu->PC += 2;
        break;
      case PUSH:
            printf("PUSH\n");
            // cpu->reg[7]--;// this will return previous value and then change it 
        --cpu->reg[7];//this will do the calculation before a return or a print
        cpu->ram[cpu->reg[7]] = cpu->reg[value1];
        cpu->PC += 2;
        break;
      case RET:
            printf("RET %d\n", cpu->ram[cpu->reg[7]]);
        cpu->PC = cpu->ram[cpu->reg[7]];
        cpu->reg[7]++;
        break;
      default:
        exit(1);
    }
      printf("\n----PC at line %d-----\n", cpu->PC+1);
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  memset(cpu->ram, 0, sizeof(cpu->ram)); // 0 is what it is assigned to. 
  memset(cpu->reg, 0, sizeof(cpu->reg));
}