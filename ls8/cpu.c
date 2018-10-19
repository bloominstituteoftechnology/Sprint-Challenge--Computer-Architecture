#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6


unsigned char  cpu_ram_read(struct cpu *cpu, unsigned char i)
{
  return cpu->ram[i];
}

void cpu_ram_write(struct cpu *cpu, unsigned char i, unsigned char data)
{
  cpu->ram[i] = data;
}
/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fp = fopen(filename, "r");
  char line[1024];
  unsigned char addr= 0x00;

  if (fp == NULL) {
    fprintf(stderr, "eror opening file %s\n", filename);
    exit(2);
  }
  while(fgets(line, sizeof line, fp)!=NULL) {
    unsigned char b = strtoul(line, NULL, 2);

    cpu_ram_write(cpu, addr++, b);
  }
  fclose(fp);

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->registers[regA] *= cpu->registers[regB];
      // TODO
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
  unsigned char IR, operandA, operandB, sp;

  IR = cpu_ram_read(cpu, cpu->pc);
  operandA = cpu_ram_read(cpu, cpu->pc+1);
  operandB = cpu_ram_read(cpu, cpu->pc+2);
  sp = cpu->registers[7];

  int add_to_pc = (IR >> 6) +1;

  while (running) {
    switch(IR){
      case LDI:
        cpu->registers[operandA] = operandB;
        break;

      case PRN:
        printf("%d\n", cpu->registers[operandA]);
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;

      case PUSH:
        sp--;
        cpu->ram[sp] = cpu->registers[operandA & 7];
        break;

      case POP: 
        cpu->registers[operandA & 7] = cpu->ram[sp];
        sp++;
        break;

      case JMP:
        cpu->pc = cpu->registers[operandA & 7];
        add_to_pc=0;
        break;

      case CALL:
        sp--;
        cpu->ram[sp] = cpu->registers[operandA];
        cpu->pc = cpu->registers[operandA &7];
        add_to_pc=0;
        break;

      case RET:
        cpu->pc = cpu->ram[sp];
        sp++;
        break;

      case CMP:
        if(cpu->registers[operandA] < cpu->registers[operandB]) {
          cpu->flags[2] = 1;
        }
        else{
            cpu->flags[2] = 0;
          }
        if(cpu->registers[operandA] > cpu->registers[operandB]) {
          cpu->flags[1] = 1;
        }
        else{
            cpu->flags[1] = 0;
          }
        if(cpu->registers[operandA] == cpu->registers[operandB]) {
          cpu->flags[0] = 1;
        }
        else{
            cpu->flags[0] = 0;
          }
        break;
      
      case JEQ:
        if(cpu->flags[0] == 1){
          cpu->pc = cpu->registers[operandA];
          add_to_pc=0;
        }
        break;

      case JNE:
        if(cpu->flags[0] == 0){
          cpu->pc = cpu->registers[operandA];
          add_to_pc=0;
        }
        break;
        
      case HLT:
        running = 0;
        break;
    }
    cpu->pc += add_to_pc;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
    cpu->pc = 0;
    memset(cpu->ram, 0, sizeof cpu->ram);
    memset(cpu->registers, 0, sizeof cpu->registers);
    memset(cpu->flags, 0, sizeof(cpu->flags));
}

