#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char val)
{
  cpu->ram[index] = val;
}


/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  //Init the file
  FILE *fp = fopen(filename, "r");
	
  char line[1024];

  unsigned char mem = 0x00;

  //Check if the file exits:
  if (fp == NULL){
    fprintf(stderr, "error opening file %s\n", filename);
    exit(2);
  }

  //While there's lines in the file...
	while (fgets(line, sizeof line, fp) != NULL) {
    char *endptr;
		unsigned char machine_code = strtoul(line, &endptr, 2);

    if (endptr == line){
      //we got no numbers
      continue;
    }

    //Write it into Memory
    cpu_ram_write(cpu, mem++, machine_code);
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
      cpu->reg[regA] *= cpu->reg[regB];
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

  unsigned char IR; //instruction register
 
  printf("\nCPU Running....\n");

  while (running) {
    // 1. Get the value of the current instruction (in address PC).

    IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, (cpu->PC+1) & 0xFF);
    unsigned char operandB = cpu_ram_read(cpu, (cpu->PC+2) & 0xFF);


    int i;
    for(i = 0; i < 8; i++) {
      // printf(" %02X", cpu->reg[i]);
    }

    int add_to_pc = (IR >> 6) + 1;
      // 2. switch() over it to decide on a course of action.
    switch (IR) {
      case LDI:
        // LDI
        // 3. Do whatever the instruction should do according to the spec.
        cpu->reg[operandA] = operandB;
        break;
      
      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;

      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        break;

      case PUSH:
        cpu_ram_write(cpu, --cpu->reg[7], cpu->reg[operandA]);
        break;

      case POP:
        cpu->reg[operandA] = cpu_ram_read(cpu,cpu->reg[7]++);
        break;

      case CALL:
        cpu_ram_write(cpu, --cpu->reg[7] , ++cpu->PC);
        cpu->PC = cpu->reg[operandA];
        add_to_pc = 0;
        break;

      case RET:
        cpu->PC = cpu_ram_read(cpu,cpu->reg[7]++);
        break;
      
      case CMP:
        if (cpu->reg[operandA] == cpu->reg[operandB]){
          cpu->flag_reg[0] = 1; // E = Equals = 0th bit
        }else if (cpu->reg[operandA] > cpu->reg[operandB]){
          cpu->flag_reg[1] = 1; // G = Greater than = 1st bit
        }else {
          cpu->flag_reg[2] = 1; // L = Less than = 2nd bit
        }
        break;
      
      case JMP:
        cpu->PC = cpu->reg[operandA];
        add_to_pc = 0;
        break;
      
      case JEQ:
        if (cpu->flag_reg[0] == 1){
          cpu->PC = cpu->reg[operandA];
          add_to_pc = 0;
        }
        break;
      
      case JNE:
        if (cpu->flag_reg[0] == 0){
          cpu->PC = cpu->reg[operandA];
          add_to_pc = 0;
        }
        break;

      case HLT:
        running = 0;
        break;
    }

    // 4. Move the PC to the next instruction.
    cpu->PC += add_to_pc;
  }


}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // Power On State
  // R0-R6 are cleared to 0.
  // R7 is set to 0xF4.
 
  // PC and FL registers are cleared to 0.
  // RAM is cleared to 0.
  
  // Initialize the PC and other special registers
  cpu->PC = 0;

  // // TODO: Zero registers and RAM
  memset(cpu->ram, 0, sizeof(cpu->ram));
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->flag_reg, 0, sizeof(cpu->flag_reg));


  cpu->reg[7] = 0b11110100;
}
