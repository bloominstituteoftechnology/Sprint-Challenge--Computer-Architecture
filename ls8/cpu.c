#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cpu.h"

#define DATA_LEN 6


/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address) {

  return cpu->ram[address]; //recieves cpu and address and returns the value at the address in RAM
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value) {
  cpu->ram[address] = value; 
}

void cpu_load(struct cpu *cpu, char *args[])
{
  /* char data[DATA_LEN] = {
    // From print8.ls8
    0b10000010, // LDI R0,8
    0b00000000,
    0b00001000,
    0b01000111, // PRN R0
    0b00000000,
    0b00000001  // HLT
  }; */

  /* int address = 0;

  for (int i = 0; i < DATA_LEN; i++) {
    cpu->ram[address] = args[i];
    address++;
    printf("Address: %d, %d \n", address, cpu->ram[address]);
  } */

  // TODO: Replace this with something less hard-coded
  FILE *fp;
  char data[1024];
  unsigned char address = 0;
  fp = fopen(args[1], "r");

  if(!fp) {
    printf("Error opening file \n");
    exit(1);
  }
  
  while(fgets(data, sizeof data, fp) != NULL) {
    unsigned char byte = strtol(data, NULL, 2);
    if (data == NULL) {
      continue;
    }
    cpu->ram[address] = byte;
    address++;
  }

  fclose(fp);

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
      printf("%d", cpu->registers[regA]);
      break;
    case ALU_ADD:
      cpu->registers[regA] += cpu->registers[regB];
      break;

    // TODO: implement more ALU ops
  }
}

unsigned char cpu_pop(struct cpu *cpu) {
  unsigned char ret = cpu->ram[cpu->registers[7]];
  cpu->registers[7]++;
  return ret;
}

void cpu_push(struct cpu *cpu, unsigned char val) {
  cpu->registers[7]--; 
  cpu->ram[cpu->registers[7]] = val;
}

 

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  int PC = cpu->PC;
  unsigned char *ram = cpu->ram;
  unsigned char operand1, operand2, instruction, val;
  


  while (running) {
    
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    instruction = ram[PC];
    // 2. Figure out how many operands this next instruction requires
    
    operand1 = cpu_ram_read(cpu, PC+1);  
    operand2 = cpu_ram_read(cpu, PC+2);
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    printf("[");
    for (int i = 0; i < 8; i++){
      printf("(R:%d V:%d S:%d),", i, cpu->registers[i], cpu->ram[cpu->registers[7]]);
    }
    printf("]\n");
    printf("ins: %d | %d %d %d %d \n", instruction, operand1, operand2, cpu_ram_read(cpu, PC+3), cpu_ram_read(cpu, PC+4) );
    switch(instruction){
      
      case HLT:
        running = 0;
        break;
      case LDI:
        cpu->registers[operand1] = operand2;
        printf("LDI: %d \n", cpu->registers[operand1]);
        PC+=3;
        break;
      case PRN: 
        printf("PRN ~ %d \n", cpu->registers[operand1]);
        PC+=2;
        break;
      case MUL:
        printf("MUL ~ %d \n", cpu->registers[operand1]);
        alu(cpu, ALU_MUL, operand1, operand2);
        PC+=3;
        break;
      case PUSH:
        val = cpu->registers[operand1];
        printf("PUSH: (R:%d V:%d) \n", operand1 ,val);
        cpu->registers[7]--;
        cpu->ram[cpu->registers[7]] = val;
        PC+=2;
        break;
      case ADD:
        alu(cpu, ALU_ADD, operand1, operand2);
        PC+=3;
        break;
      case POP:
        printf("POP: (R:%d V:%d) \n", operand1 ,cpu->ram[cpu->registers[7]]);
        cpu->registers[operand1] = cpu->ram[cpu->registers[7]];
        cpu->registers[7]++;
        PC+=2;
        break;
      case CALL:
        printf("CALL\n\n\n");
        cpu_push(cpu, PC+2);
        PC = cpu->registers[operand1];
        break;
      case RET:
        //pop the PC off the stack
        PC = cpu_pop(cpu);
        break;
      case CMP:
        if (cpu->registers[operand1] == cpu->registers[operand2]) {
          cpu->FL = 1;
        } else if (cpu->registers[operand1] > cpu->registers[operand2]) {
          cpu->FL = 2;
        } else {
          cpu->FL = 4;
        } 
        PC+=3;
        break;
      case JMP:
        PC = cpu->registers[operand1];
        break;
      case JEQ:
        if (cpu->FL == 1) {
          PC = cpu->registers[operand1]; 
          break;   
        }
        else{
          PC+=2;
          break;
        }
      case JNE:
        if (cpu->FL != 1) {
          PC = cpu->registers[operand1];
          break;
        }
        else{
          PC+=2;
          break;
        }
        
      }

    }
    
    // 6. Move the PC to the next instruction.
    
  
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  cpu->registers[7] = 0xF4;
  cpu->FL = 0;
  /* cpu->registers = calloc(8, sizeof(unsigned char));
  cpu->ram = calloc(256, sizeof(unsigned char)); */
}
