#include <stdio.h>
#include "cpu.h"
#include <stdlib.h>
#include <string.h>
/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address) {
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value) {
  cpu->ram[address] = value;
}

unsigned char cpu_pop(struct cpu *cpu)
{
  unsigned char val = cpu_ram_read(cpu, cpu->registers[SP]);
  
  cpu->registers[SP]++;

  return val;
}

void cpu_push(struct cpu *cpu, unsigned char val)
{
  cpu->registers[SP]--;
  
  cpu_ram_write(cpu, cpu->registers[SP], val);
}

void cpu_load(char *filename, struct cpu *cpu)
{
  FILE *fp;
  char line[1024];
  int address = 0;

  if ((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Error locating file %s\n", filename);
    exit(1);
  }

    while (fgets(line, sizeof line, fp) != NULL) {
    char *endchar;
    unsigned char byte = strtol(line, &endchar, 2);                


      if (endchar == line) {
            continue;
          }

      cpu->ram[address++] = byte;
    }
}
/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *reg = cpu->registers;

  switch (op) {
    case ALU_MUL:
    reg[regA] *= reg[regB];
      break;
    case ALU_ADD:
    reg[regA] += reg[regB];
      break;

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
    // 1. Get the value of the current instruction (in address PC).
        unsigned char IR = cpu->ram[cpu->PC]; //cpu_ram_read(cpu, cpu->PC)

        unsigned char operandA = cpu->ram[(cpu->PC + 1)]; //cpu_ram_read(cpu, cpu->PC + 1)
        unsigned char operandB = cpu->ram[(cpu->PC + 2)]; //cpu_ram_read(cpu, cpu->PC + 2)
    // 2. switch() over it to decide on a course of action.
        int instruction_set_pc = (IR >> 4) & 1;

        switch(IR) {
          case HLT:
            running = 0;
            break;
        
          case LDI:
           cpu->registers[operandA] = operandB;
            break;

          case PRN:
           printf("%d\n", cpu->registers[operandA]);
            break;

          case MUL:
            alu(cpu, ALU_MUL, operandA, operandB);
            break;
          
          case CMP: // compare values in two registers
            if (cpu->registers[operandA] == cpu->registers[operandB]) {
              cpu->E = 1;
            }
            else if (cpu->registers[operandA] < cpu->registers[operandB]) {
              cpu->L = 1;
            } 
            else if (cpu->registers[operandA] > cpu->registers[operandB]){
              cpu->G = 1;
            }
            else
            {
              cpu->E = 0;
              cpu->G = 0;
              cpu->L = 0;
            }
            break;

          case JMP: // jump to address given
            cpu->PC = cpu->registers[operandA];
            break;

          case JNE: // if E flag == 0, jump to address stored in given register
            if (cpu->E == 0) {
              cpu->PC = cpu->registers[operandA];
            }
            else { // otherwise move to next process
              cpu->PC += 2;
            }
            break;

          case JEQ: // if E flag == 1, jump to address stored in given register
            if (cpu->E == 1){
              cpu->PC = cpu->registers[operandA];
            }
            else {
              cpu->PC += 2; // otherwise move to next process
            }
            break;
          
          case PUSH:
          cpu_push(cpu, cpu->registers[operandA]);
            break;

          case POP:
            cpu->registers[operandA] = cpu_pop(cpu);
            break;

          default:
            printf("unknown instruction at %02x: %02x\n", cpu->PC, IR);
            break;
        }
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.+
    if (!instruction_set_pc) {
      cpu->PC += (IR >> 6) + 1;
    }
 }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  // sp = cpu->registers 7 
  // TODO: Zero registers and RAM
    cpu->PC = 0;

  // Zero registers and RAM
  memset(cpu->registers, 0, sizeof cpu->registers);
  memset(cpu->ram, 0, sizeof cpu->ram);

  // Initialize SP
  cpu->registers[SP] =  0xF4;
  cpu->E = E_FLAG;
}
