#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#define ADDR_EMPTY_STACK 0xF4 // where SP is on the empty stack


// initialize RAM address to start writing to when reading from a file
unsigned int address = 0;

/**
 * Helper function to print out the binary representation of an int
 */
void print_binary(int x)
{
  static char b[9];
  b[0] = '\0';

  int z;
  for (z = 128; z > 0; z >>= 1)
  {
    strcat(b, ((x & z) == z) ? "1" : "0");
  }

  printf("%s\n", b);
}

/**
 * Read from RAM
 */
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

/**
 * Write to RAM
 */
unsigned char cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  return cpu->ram[address] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, int argc, char *argv[])
{
  if (argc < 2)
  {
    fprintf(stderr, "Usage: ./ls8 filename\n");
    exit(1);
  }

  FILE *fp;
  char line[1024];
  char *file = argv[1];

  // open the source file
  if ((fp = fopen(file, "r")) == NULL)
  {
    fprintf(stderr, "Cannot open file %s\n", file);
    exit(1);
  }

  // read the source file line by line
  while (fgets(line, sizeof(line), fp) != NULL)
  {
    char *ptr;
    unsigned char instruction;

    instruction = strtol(line, &ptr, 2);

    // ignore lines from which no numbers were read
    if (ptr == line) continue;

    // add instruction to RAM and increment RAM address
    cpu->ram[address++] = instruction;
  }
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_ADD:
      cpu->reg[regA] += cpu->reg[regB];
      break;
    case ALU_AND:
      cpu->reg[regA] = cpu->reg[regA] & cpu->reg[regB];
      break;
    case ALU_CMP:
      if (cpu->reg[regA] < cpu->reg[regB])      cpu->FL = (cpu->FL & ~7) | 4;
      else if (cpu->reg[regA] > cpu->reg[regB]) cpu->FL = (cpu->FL & ~7) | 2;
      else                                      cpu->FL = (cpu->FL & ~7) | 1;
      break;
    case ALU_DEC:
      cpu->reg[regA]--;
      break;
    case ALU_DIV:
      cpu->reg[regA] /= cpu->reg[regB];
      break;
    case ALU_INC:
      cpu->reg[regA]++;
      break;
    case ALU_MOD:
      cpu->reg[regA] %= cpu->reg[regB];
      break;
    case ALU_MUL:
      cpu->reg[regA] *= cpu->reg[regB];
      break;
    case ALU_NOT:
      cpu->reg[regA] = ~cpu->reg[regA];
      break;
    case ALU_OR:
      cpu->reg[regA] |= cpu->reg[regB];
      break;
    case ALU_SHL:
      cpu->reg[regA] <<= cpu->reg[regB];
      break;
    case ALU_SHR:
      cpu->reg[regA] >>= cpu->reg[regB];
      break;
    case ALU_SUB:
      cpu->reg[regA] -= cpu->reg[regB];
      break;
    case ALU_XOR:
      cpu->reg[regA] ^= cpu->reg[regB];
      break;
    default:
      break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char IR, operandA, operandB;
  unsigned int num_operands, jBits;
  unsigned int interrupts_enabled = 0;
  struct timeval tv;
  unsigned int prev_sec;

  while (running) {
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu->PC);

    // 2. Figure out how many operands this next instruction requires
    num_operands = IR >> 6;

    // 3. Get the appropriate value(s) of the operands following this instruction
    operandA = cpu_ram_read(cpu, (cpu->PC + 1) & 0xFF);
    operandB = cpu_ram_read(cpu, (cpu->PC + 2) & 0xFF);

    gettimeofday(&tv, NULL);
    if (tv.tv_sec != prev_sec)
    {
      cpu->reg[6] |= 1;
      prev_sec = tv.tv_sec;
    }

    if (interrupts_enabled)
    {
      // TODO
    }

    // 4. switch() over it to decide on a course of action.
    switch(IR)
    {
      // 5. Do whatever the instruction should do according to the spec.
      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        cpu->PC += num_operands + 1;
        break;
      case AND:
        alu(cpu, ALU_AND, operandA, operandB);
        cpu->PC += num_operands + 1;
        break;
      case CALL:
        cpu->PC += num_operands + 1;
        cpu_ram_write(cpu, --cpu->SP, cpu->PC);
        cpu->PC = cpu->reg[operandA];
        break;
      case CMP:
        alu(cpu, ALU_CMP, operandA, operandB);
        cpu->PC += num_operands + 1;
        break;
      case DEC:
        alu(cpu, ALU_DEC, operandA, operandB);
        cpu->PC += num_operands + 1;
        break;
      case DIV:
        if (cpu->reg[operandB] == 0)
        {
          printf("Divide by 0 not allowed.\n");
          running = 0;
        }
        else
        {
          alu(cpu, ALU_DIV, operandA, operandB);
          cpu->PC += num_operands + 1;
        }
        break;
      case HLT:
        running = 0;
        break;
      case INC:
        alu(cpu, ALU_INC, operandA, operandB);
        cpu->PC += num_operands + 1;
        break;
      case INT:
        cpu->reg[6] |= cpu->reg[operandA];
        unsigned char maskedInterrupts = cpu->IM & cpu->reg[6];
        for (int i = 0 ; i != 8 ; i++) {
          if ((maskedInterrupts & (1 << i)) != 0)
          {
            interrupts_enabled = 0;
            cpu->reg[6] = 0x00; // Clear the bit in the IS register
            cpu_ram_write(cpu, --cpu->SP, cpu->PC);
            cpu_ram_write(cpu, --cpu->SP, cpu->FL);
            for (int j = 0; j < 7; j++)
            {
              cpu_ram_write(cpu, --cpu->SP, cpu->reg[j]);
            }
            cpu->PC = cpu_ram_read(cpu, 0xF8);
            break;
          }
        }
        cpu->PC += num_operands + 1;
        break;
      case IRET:
        for (int i = 6; i >= 0; i--)
        {
          cpu->reg[i] = cpu_ram_read(cpu, cpu->SP++);
        }
        cpu->FL = cpu_ram_read(cpu, cpu->SP++);
        cpu->PC = cpu_ram_read(cpu, cpu->SP++);
        interrupts_enabled = 1;
        break;
      case JEQ:
        jBits = cpu->FL & (1 << 0);
        if (jBits == 0b00000001)
        {
          cpu->PC = cpu->reg[operandA];
        }
        else
        {
          cpu->PC += num_operands + 1;
        }
        break;
      case JGE:
        jBits = cpu->FL & ((1 << 0) | (1 << 1));
        if (jBits == 0b00000010 || jBits == 0b00000001)
        {
          cpu->PC = cpu->reg[operandA];
        }
        else
        {
          cpu->PC += num_operands + 1;
        }
        break;
      case JGT:
        jBits = cpu->FL & (1 << 1);
        if (jBits == 0b00000010)
        {
          cpu->PC = cpu->reg[operandA];
        }
        else
        {
          cpu->PC += num_operands + 1;
        }
        break;
      case JLE:
        jBits = cpu->FL & ((1 << 0) | (1 << 2));
        if (jBits == 0b00000100 || jBits == 0b00000001)
        {
          cpu->PC = cpu->reg[operandA];
        }
        else
        {
          cpu->PC += num_operands + 1;
        }
        break;
      case JLT:
        jBits = cpu->FL & (1 << 2);
        if (jBits == 0b00000100)
        {
          cpu->PC = cpu->reg[operandA];
        }
        else
        {
          cpu->PC += num_operands + 1;
        }
        break;
      case JMP:
        cpu->PC = cpu->reg[operandA];
        break;
      case JNE:
        jBits = cpu->FL & (1 << 0);
        if (jBits == 0b00000000)
        {
          cpu->PC = cpu->reg[operandA];
        }
        else
        {
          cpu->PC += num_operands + 1;
        }
        break;
      case LD:
        cpu->reg[operandA] = cpu_ram_read(cpu, cpu->reg[operandB]);
        cpu->PC += num_operands + 1;
        break;
      case LDI:
        cpu->reg[operandA] = operandB;
        cpu->PC += num_operands + 1;
        break;
      case MOD:
        if (cpu->reg[operandB] == 0)
        {
          printf("Divide by 0 not allowed.\n");
          running = 0;
        }
        else
        {
          alu(cpu, ALU_MOD, operandA, operandB);
          cpu->PC += num_operands + 1;
        }
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        cpu->PC += num_operands + 1;
        break;
      case NOP:
        cpu->PC += num_operands + 1;
        break;
      case NOT:
        alu(cpu, ALU_NOT, operandA, operandB);
        cpu->PC += num_operands + 1;
        break;
      case OR:
        alu(cpu, ALU_OR, operandA, operandB);
        cpu->PC += num_operands + 1;
        break;
      case POP:
        cpu->reg[operandA] = cpu_ram_read(cpu, cpu->SP++);
        if (cpu->SP == 0x00) cpu->SP = ADDR_EMPTY_STACK;
        cpu->PC += num_operands + 1;
        break;
      case PRA:
        printf("%c\n", cpu->reg[operandA]);
        cpu->PC += num_operands + 1;
        break;
      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        cpu->PC += num_operands + 1;
        break;
      case PUSH:
        if (--cpu->SP <= address) {
          fprintf(stderr, "Stack overflow has occurred.\n");
          exit(1);
        }
        cpu_ram_write(cpu, cpu->SP, cpu->reg[operandA]);
        cpu->PC += num_operands + 1;
        break;
      case RET:
        cpu->PC = cpu_ram_read(cpu, cpu->SP++);
        if (cpu->SP == 0x00) cpu->SP = ADDR_EMPTY_STACK;
        break;
      case SHL:
        alu(cpu, ALU_SHL, operandA, operandB);
        cpu->PC += num_operands + 1;
        break;
      case SHR:
        alu(cpu, ALU_SHR, operandA, operandB);
        cpu->PC += num_operands + 1;
        break;
      case ST:
        cpu_ram_write(cpu, cpu->reg[operandA], cpu->reg[operandB]);
        cpu->PC += num_operands + 1;
        break;
      case SUB:
        alu(cpu, ALU_SUB, operandA, operandB);
        cpu->PC += num_operands + 1;
        break;
      case XOR:
        alu(cpu, ALU_XOR, operandA, operandB);
        cpu->PC += num_operands + 1;
        break;
      default:
        break;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  memset(cpu->reg, 0, 8 * sizeof(cpu->reg[0]));
  memset(cpu->ram, 0, 256 * sizeof(cpu->ram[0]));

  // Initialize the PC and other special registers
  cpu->PC = 0;
  cpu->FL = 0; // 00000LGE
  cpu->IM = cpu->reg[5]; // R5
  cpu->SP = ADDR_EMPTY_STACK; // R7
}
