#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

#define DATA_LEN 6
#define SP 7

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *dir)
{
  FILE *fptr;

  if ((fptr = fopen(dir,"r"))== NULL)
  {
    fprintf(stderr, "File does not exist: %s\n",dir);
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

void cpu_ram_write(struct cpu *cpu, unsigned char value, unsigned char address){
  cpu->ram[address] = value;
}

void cpu_increment(struct cpu *cpu, int num_ops){
	  cpu->PC = cpu->PC + num_ops + 1;
	}

// Stack

void cpu_push(struct cpu *cpu, unsigned char value)
{
  cpu->registers[SP]--;
  cpu_ram_write(cpu,value, cpu->registers[SP]);
  printf("PUSH :%d @ %d\n VALUE: %d\n",cpu_ram_read(cpu,cpu->registers[SP]), cpu->registers[SP], value );
}

unsigned char cpu_pop(struct cpu *cpu){
  if(cpu->registers[SP] == 0xF4){
    fprintf(stderr, "stack is empty");
    exit(1);
  }

  unsigned char value = cpu_ram_read(cpu,cpu->registers[SP]);
  printf("POP :%d @ %d\n",value, cpu->registers[SP] );
  cpu_ram_write(cpu,cpu->registers[SP],0);
  cpu->registers[SP]++;
  
  return value;

}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned int num1, num2;
  switch (op) {
    case ALU_MUL:
      // TODO
      cpu->registers[regA] *= cpu->registers[regB];
    // TODO: implement more ALU ops
    case ALU_ADD:
    num1 = cpu->registers[regA];
    num2 = cpu->registers[regB];

    cpu->registers[regA] = num1 + num2;
    break;
    case ALU_CMP:
    num1 = cpu->registers[regA];
    num2 = cpu->registers[regB];
    if (num1 == num2){
      cpu->FL = 0b00000001;
    } else if (num1 > num2) {
      cpu->FL = 0b00000010;
    } else if (num1 < num2) {
      cpu->FL = 0b00000100;
    }

  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  // Remember Zero is interpreted as false and anything non-zero is interpreted as true.
  int should_increment;
  unsigned char IR;
  unsigned char opA;
  unsigned char opB;

  while (running) {
    
    should_increment = 1;
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    int operands = IR >> 6; 
    // 3. Get the appropriate value(s) of the operands following this instruction
    switch (operands){
      case 1:
        opA = cpu_ram_read(cpu,cpu->PC+1);
      case 2:
        opA = cpu_ram_read(cpu,cpu->PC+1);
        opB = cpu_ram_read(cpu,cpu->PC+2);
    }

    // DEBUG
    printf("TRACE: %02X: %02X %02X %02X %02X\n", cpu->PC, IR, opA, opB, cpu->registers[0]);

    // 4. switch() over it (THE INSTRUCTION) to decide on a course of action.
    switch(IR){
    // 5. Do whatever the instruction should do according to the spec.
      case LDI:
        cpu->registers[opA] = opB;
        break;
      case PRN:
        printf("%d\n", cpu->registers[opA]);
        break;
      case MUL:
        alu(cpu,ALU_MUL,opA,opB);
        break;
      case HLT:
      running = 0;
      break;
      case PUSH:
        cpu_push(cpu,cpu->registers[opA]);
        break;
      case POP:
        cpu->registers[opA] = cpu_pop(cpu);
        break;
      case ADD:
        alu(cpu,ALU_ADD,opA,opB);
        break;
	    case CALL:
        cpu_push(cpu, cpu->PC + operands + 1);
        cpu->PC = cpu->registers[opA];
        should_increment = 0;
        break;
	    case RET:
        cpu->PC = cpu_pop(cpu);
        should_increment = 0;
        break;
      case CMP:
        alu(cpu, ALU_CMP, opA, opB);
        break;
      default:
        fprintf(stderr, "PC %02x: unknown instruction %02x\n", cpu->PC, IR);
        exit(3);
    }
    // 6. Move the PC to the next instruction.
    // cpu->PC = cpu->PC + operands + 1;
    if(should_increment == 1){
	      cpu_increment(cpu,operands);
	    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
   // Initialize the PC and other special registers

  cpu->PC = 0;
  cpu->FL = 0;
  memset(cpu->registers,0,8);
  memset(cpu->ram,0,256);
  
}


