#include "cpu.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//make helper function to make sure address is in range
unsigned char cpu_ram_read(CPU *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void cpu_ram_write(CPU *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

unsigned char cpu_pop(CPU *cpu)
{
  unsigned char val = cpu_ram_read(cpu, cpu->reg[SP]);
  cpu->reg[SP]++;

  return val;
}

void cpu_push(CPU *cpu, unsigned char val)
{
  cpu->reg[SP]--;
  cpu_ram_write(cpu, cpu->reg[SP], val);
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(CPU *cpu, char *filename)
{
  int address = cpu->pc;
  // get a pointer to the filename
  FILE *fp;
  char line[2000];

  // you are going to open the file using fopen()
  if ((fp = fopen(filename, "r")) == NULL)
  {
    // something wrong so we tell the user
    fprintf(stderr, "We can not open the %s\n", filename);
    exit(2);
  }
  // loop through the file reading the contents using fgets
  while (fgets(line, sizeof line, fp) != NULL)
  {
    char *endchar;

    unsigned char opcode = strtol(line, &endchar, 2);

    // ignore empty lines
    if (endchar == line)
    {
      continue;
    }

    //cpu->ram[address++] = opcode;

    // stroe the opcode in the ram
    cpu_ram_write(cpu, address++, opcode);
  }
}

/**
 * ALU
 */
void alu(CPU *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_ADD:
    cpu->reg[regA] += cpu->reg[regB];
    break;

  case ALU_DIV:
    cpu->reg[regA] /= cpu->reg[regB];
    break;

  case ALU_MOD:
    cpu->reg[regA] = cpu->reg[regA] % cpu->reg[regB];
    break;

  case ALU_MUL:
    cpu->reg[regA] *= cpu->reg[regB];
    break;

  case ALU_SUB:
    cpu->reg[regA] -= cpu->reg[regB];

  default:
    printf("unknown ALU instruction at %02x: %02x\n", cpu->pc, op);
    exit(3);
  }
}

/**
 * Run the CPU
 */
void cpu_run(CPU *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    unsigned char IR = cpu_ram_read(cpu, cpu->pc);

    unsigned char operandA = cpu_ram_read(cpu, (cpu->pc + 1) & MAX_ADDR);
    unsigned char operandB = cpu_ram_read(cpu, (cpu->pc + 2) & MAX_ADDR);
    switch (IR)
    {
    case PRN:
      printf("%d\n", cpu->reg[operandA]);
      break;

    case ADD:
      alu(cpu, ALU_ADD, operandA, operandB);
      break;

    case HLT:
      running = 0;
      break;

    case LDI:
      cpu->reg[operandA] = operandB;
      break;

    case MUT:
      cpu->reg[operandA] *= cpu->reg[operandB];
      break;

    case POP:
      // TODO
      cpu->reg[operandA] = cpu_pop(cpu);
      break;

    case PUSH:
      // TODO
      cpu_push(cpu, cpu->reg[operandA]);
      break;

    default:
      printf("unknown instruction at %02x: %02x\n", cpu->pc, IR);
      exit(2);
    }

    /*
    // Increment the PC register to go to the next instruction. Instructions
    // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
    // instruction byte tells you how many bytes follow the instruction byte
    // for any particular instruction.
    if (
      IR != CALL &&
      IR != INT &&
      IR != IRET &&
      IR != JEQ &&
      IR != JGT &&
      IR != JLT &&
      IR != JMP &&
      IR != JNE &&
      IR != RET
    ){
      // right shift it 6 places
      cpu->pc += (IR >> 6) + 1;
    }
    */
    int instruction_sets_the_pc = (IR >> 4) & 1;

    if (!instruction_sets_the_pc)
    {
      // right shift it 6 places
      cpu->pc += ((IR >> 6) & 0x3) + 1;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(CPU *cpu)
{
  // TODO: Zero registers and RAM
  memset(cpu->reg, 0, sizeof cpu->reg);
  memset(cpu->ram, 0, sizeof cpu->ram);

  // TODO: Initialize the PC and other special registers
  cpu->pc = START_OF_PROGRAM_ADDR;
  cpu->reg[SP] = START_OF_STACK_ADDR;
  cpu->fl = CPU_FLAG;
}