#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char * argv[])
{
  FILE *fp;
  char *location = argv[1];
  char temp[256];
  fp = fopen(location, "r");
  int counter = 0;
  if(fp == NULL) {
      perror("Error opening file");
      exit(1);
  } else {
    while(fgets(temp, sizeof(temp), fp) != NULL) {
      unsigned char data = strtol(temp, NULL, 2);
      
      cpu_ram_write(cpu, data, counter++);
    }
    fclose(fp);

    
  }
  

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->registers[regA] = (int)cpu->registers[regA] * (int)cpu->registers[regB];
      break;
    case ALU_ADD:
      cpu->registers[regA] = (int)cpu->registers[regA] + (int)cpu->registers[regB];
      break;
    case ALU_CMP:
      if((int)cpu->registers[regA] < (int)cpu->registers[regB]) {
        cpu->FL = 4;
      } else if ((int)cpu->registers[regA] > (int)cpu->registers[regB]) {
        cpu->FL = 2;
      } else {
        cpu->FL = 1;
      }
      
      break;
    // TODO: implement more ALU ops
  }
}
unsigned char cpu_ram_read(struct cpu *cpu, int index) {
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, char data, int index) {
  cpu->ram[index] = data;
}

void handle_LDI(struct cpu *cpu, unsigned char operandA, unsigned char operandB) {
  cpu->registers[operandA] = operandB;
 
}

void handle_PRN(struct cpu *cpu, unsigned char operandA) {
  printf("Numeric Value = %i\n", cpu->registers[operandA]);
  
}

int handle_HLT() {
  return 0;
}

void handle_PUSH(struct cpu *cpu, unsigned char operandA) {
  cpu->registers[7]--;
  *cpu->registers[7] = cpu->registers[operandA];
  
}

void handle_POP(struct cpu *cpu, unsigned char operandA) {
  cpu->registers[operandA] = *cpu->registers[7];
  cpu->registers[7]++;
}

void handle_JEQ(struct cpu *cpu, unsigned char operandA) {
  
  if(cpu->FL == 1) {
    
    cpu->PC = (int)cpu->registers[operandA];
  }
}

void handle_JNE(struct cpu *cpu, unsigned char operandA) {
  if(cpu->FL != 1) {
    cpu->PC = cpu->registers[operandA];
    
  }
}

void handle_JMP(struct cpu *cpu, unsigned char operandA) {
  
  cpu->PC = (int)cpu->registers[operandA];
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  while (running) {
    // TODO
    
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);
    unsigned int numop = IR >> 6;
    unsigned int test;
    cpu->PC += numop + 1;
    switch(IR) {
      case(LDI):
        handle_LDI(cpu, operandA, operandB);
        break;
      case(PRN):
        handle_PRN(cpu, operandA);
        break;
      case(HLT):
        running = handle_HLT();
        break;
      case(MUL):
        alu(cpu, ALU_MUL,operandA, operandB);
        break;
      case(PUSH):
        handle_PUSH(cpu, operandA);
        break;
      case(POP):
        handle_POP(cpu, operandA);
        break;
      case(CALL):
        cpu->registers[7]--;
        cpu->registers[7] = cpu->PC;
        cpu->PC = cpu->registers[operandA];
        break;
      case(RET):    
        test = (int)cpu->registers[7];
        cpu->PC = test; 
        cpu->registers[7]++;
        break;
      case(ADD):
        alu(cpu, ALU_ADD, operandA, operandB);
        break;
      case(CMP):
        alu(cpu, ALU_CMP, operandA, operandB);
        break;
      case(JEQ):
        handle_JEQ(cpu, operandA);
        break;
      case(JNE):
        handle_JNE(cpu, operandA);
        break;
      case(JMP):
        handle_JMP(cpu, operandA);
        break;
      
    }
    
    
    
    
    
    
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
  }
}



/**
 * Initialize a CPU struct
 */

void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  cpu->FL= 0;
  memset(cpu->registers, 0, sizeof(cpu->registers));
  memset(cpu->ram, 0, sizeof(cpu->ram));
  cpu->registers[7] = &cpu->ram[255];
  // TODO: Initialize the PC and other special registers
}

/*
### CALL register

`CALL register`

Calls a subroutine (function) at the address stored in the register.

1. The address of the ***instruction*** _directly after_ `CALL` is
   pushed onto the stack. This allows us to return to where we left off when the subroutine finishes executing.
2. The PC is set to the address stored in the given register. We jump to that location in RAM and execute the first instruction in the subroutine. The PC can move forward or backwards from its current location.

Machine code:
```
01010000 00000rrr
50 0r
```


### RET

`RET`

Return from subroutine.

Pop the value from the top of the stack and store it in the `PC`.

Machine Code:
```
00010001
11
```
*/