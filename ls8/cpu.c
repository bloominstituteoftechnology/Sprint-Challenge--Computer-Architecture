#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar)
{
  return cpu->ram[mar];
}

void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr)
{
  // write mdr to ram
  cpu->ram[mar] = mdr;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

void cpu_load(char *filename, struct cpu *cpu)
{
  FILE *fp = fopen(filename, "r");

  // buffer
  char line[1024];
  unsigned char address = 0;

  // open source file
  if ((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Cannot open file %s\n", filename);
    exit(2);
  };

  // Read all the lines and store them in ram
  while(fgets(line, sizeof line, fp) != NULL) {

    // converts string to a number
    char *endchar;
    unsigned char byte = strtoul(line, &endchar, 2);

    if (endchar == line)
    {
      continue;
    }
    
    cpu_ram_write(cpu, address++, byte);
  }
  fclose(fp);
}



/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *reg = cpu->reg;
  unsigned char valA = reg[regA];
  unsigned char valB = reg[regB];
  
  switch (op) {
    case ALU_MUL:
      reg[valA] *= valB & 0xFF;
      break;
    case ALU_ADD:
      reg[valA] += valB & 0xFF;
      break;
    case ALU_CMP:
      if (regA == regB)
      {
        cpu->FL = 0b00000001;
      }
      else if (regA >= regB) 
      {
        cpu->FL = 0b00000010;
      }
      else
      {
        cpu->FL = 0b00000100;
      }
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
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    
    // 2. Figure out how many operands this next instruction requires
    
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operand_a = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operand_b = cpu_ram_read(cpu, cpu->PC + 2);

    int next_instruction = 1;
    if (IR & 0x80)
    {
      operand_a = cpu_ram_read(cpu, cpu->PC + 1);
      operand_b = cpu_ram_read(cpu, cpu->PC + 2);

      next_instruction = 3;
    } 
    else if (IR & 0x40)
    {
      // printf("Working?\n");
      operand_a = cpu_ram_read(cpu, cpu->PC + 1);

      next_instruction = 2;
    }

    // print out hex letters, 2 means it'll be two characters long
    // printf("TRACE: %02X   %02X   %02X   %02X\n", cpu->PC, IR, operand_a, operand_b);
    
    // 4. switch() over it to decide on a course of action.
    switch(IR) {
      case LDI:
        cpu->reg[operand_a] = operand_b;
        cpu->PC += 3;
        break;

      case PRN:
        printf("%d\n", cpu->reg[operand_a]);
        cpu->PC += 2;
        break;

      case HLT:
        running = 0;
        break;

      case MUL:
        alu(cpu, ALU_MUL, operand_a, operand_b);
        cpu->PC += 3;
        break;

      case ADD:
        alu(cpu, ALU_ADD, operand_a, operand_b);
        cpu->PC += 3;
        break;

      case PUSH:
        cpu->ram[--cpu->reg[7]] = cpu->reg[operand_a];
        break;

      case POP:
        cpu->reg[operand_a] = cpu->ram[cpu->reg[7]++];
        break;

      case CALL:
        cpu->ram[--cpu->reg[7]] = cpu->PC + next_instruction;
        cpu->PC = cpu->reg[operand_a];
        continue;
      
      // Sprint

      case CMP:
        alu(cpu, ALU_CMP, operand_a, operand_b);
        break;

      case JMP:
        cpu->PC = cpu->reg[operand_a];
        break;

      case JEQ:
        if (cpu->FL) {
          cpu->PC = cpu->reg[operand_a];
          continue;
        }
        break;

      case JNE:
        cpu->PC = cpu->reg[operand_a];
        break;
        
      default:
        printf("unexpected instruction 0x%02X at 0x%02X\n", IR, cpu->PC);
        exit(1);
    }

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
  memset(cpu->reg, 0, 8);
  memset(cpu->ram, 0, 256);

  cpu->reg[7] = 0xF4;
}
