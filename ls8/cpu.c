#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6
#define SP 7

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *dir)
{
  // open file
  FILE *fptr;

  if ((fptr = fopen(dir,"r"))== NULL){
    fprintf(stderr, "file does not exist: %s\n",dir);
    exit(1);
  }
  //
  char temp[256];
  char *instruction;
 
  unsigned int counter = 0;

  while(fgets(temp,sizeof(temp),fptr) != NULL){
    instruction = strndup(temp,8);
    
    if (instruction[0] == '0' || instruction[0] == '1'){
      cpu->ram[counter] = strtoul(instruction,NULL,2);
      counter++;
    }
  }

  fclose(fptr);
 
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address){
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value){
  cpu->ram[address] = value;
}

void cpu_jump(struct cpu *cpu, unsigned int address){
  cpu->PC = cpu->registers[address];
}

void cpu_increment(struct cpu *cpu, int num_operands){
  cpu->PC = cpu->PC + num_operands + 1;
}

// Stack

void cpu_push(struct cpu *cpu, unsigned char value)
{
  cpu->registers[SP]--;
  cpu_ram_write(cpu,cpu->registers[SP],value);
}

unsigned char cpu_pop(struct cpu *cpu){

  if(cpu->registers[SP] == 0xF4){
    fprintf(stderr, "stack is empty");
    exit(1);
  }
  
  unsigned char value = cpu_ram_read(cpu,cpu->registers[SP]);
  cpu_ram_write(cpu,SP,0);
  cpu->registers[SP]++;
  return value;

}
/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char num1, num2;

  switch (op) {
    case ALU_MUL:
      cpu->registers[regA] *= cpu->registers[regB];
      // cpu_ram_write(cpu,regA,cpu_ram_read(cpu,regA)*cpu_ram_read(cpu,regB));
      break;
    case ALU_CMP:
      num1 = cpu->registers[regA];
      num2 = cpu->registers[regB];

      if (num1 == num2){
        cpu->FL = 1;
      }
      if (num1 > num2){
        cpu->FL = 2;
      }
      if (num1 < num2){
        cpu->FL = 4;
      }
      // printf("FLAG %d\n", cpu->FL);
      break;
    case ALU_ADD:
      num1 = cpu->registers[regA];
      num2 = cpu->registers[regB];

      cpu->registers[regA] = num1 + num2;
      break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char IR, opA, opB;
  while (running) {
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu,cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    unsigned int operands = IR >> 6;
    
    // 3. Get the appropriate value(s) of the operands following this instruction
    switch (operands){
      case 1:
        opA = cpu_ram_read(cpu,cpu->PC+1);
      case 2:
        opA = cpu_ram_read(cpu,cpu->PC+1);
        opB = cpu_ram_read(cpu,cpu->PC+2);
    }
    // 4. switch() over it to decide on a course of action.
    switch(IR){
    // 5. Do whatever the instruction should do according to the spec.
      case LDI:
        cpu->registers[opA] = opB;
        cpu_increment(cpu,operands);
        break;
      case PRN:
        printf("%d\n", cpu->registers[opA]);
        cpu_increment(cpu,operands);
        break;
      case MUL:
        alu(cpu,ALU_MUL,opA,opB);
        cpu_increment(cpu,operands);
        break;
      case PUSH:
        cpu_push(cpu,cpu->registers[opA]);
        cpu_increment(cpu,operands);
        break;
      case POP:
        cpu->registers[opA] = cpu_pop(cpu);
        cpu_increment(cpu,operands);
        break;
      case CMP:
        alu(cpu,ALU_CMP,opA,opB);
        cpu_increment(cpu,operands);
        break;
      case JMP:
        cpu_jump(cpu,opA);
        break;
      case JEQ:
        if (cpu->FL == 1){
          cpu_jump(cpu,opA);
        } else {
        cpu_increment(cpu,operands); 
        }
        break;
      case JNE:
        if (cpu->FL != 1){
          cpu_jump(cpu,opA);
        } else {
          cpu_increment(cpu,operands);
        }
        break;
      case ADD:
        alu(cpu,ALU_ADD,opA,opB);
        cpu_increment(cpu,operands);
        break;
      case HLT:
        running = 0;
        break;
      default:
        fprintf(stderr, "PC %02x: unkown instruction %02x:\n", cpu->PC, IR);
        exit(3);

    }
    // 6. Move the PC to the next instruction.
  }
}


/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  //Initialize the PC and other special registers
  cpu->PC = 0;
  cpu->FL = 0;
  memset(cpu->registers,0,8);
  memset(cpu->ram,0,256);
  cpu->registers[SP] = 0xF4;
}
