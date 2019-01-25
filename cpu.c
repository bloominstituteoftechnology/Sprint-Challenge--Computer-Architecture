#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

/**
 * Helper function to write a value to the specified index in RAM
 */
void cpu_ram_write(struct cpu *cpu, unsigned char val, unsigned char index)
{
  cpu->ram[index] = val;
}

/**
 * Helper function to read a value from the specified index in RAM
 * Returns the read value
 */
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  return cpu->ram[index];
}

/**
 * Push a value on the CPU stack
 */
void cpu_push(struct cpu *cpu, unsigned char val)
{
  cpu->reg[SP]--;

  cpu_ram_write(cpu, val, cpu->reg[SP]);
}

/**
 * Pop a value from the CPU stack
 */
unsigned char cpu_pop(struct cpu *cpu)
{
  unsigned char val = cpu_ram_read(cpu, cpu->reg[SP]);

  cpu->reg[SP]++;

  return val;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(char *filename, struct cpu *cpu)
{
  FILE *fp;
  char line[1024];
  int address = ADDR_PROGRAM_ENTRY;

  // Open the source file
  if ((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Cannot open file %s\n", filename);
    exit(2);
  }

  // Read all the lines and store them in RAM
  while (fgets(line, sizeof line, fp) != NULL) {

    // Convert string to a number
    char *endchar;
    unsigned char byte = strtol(line, &endchar, 2);;

    // Ignore lines from which no numbers were read
    if (endchar == line) {
      continue;
    }

    // Store in ram
    cpu_ram_write(cpu, byte, address++);
  }
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *reg = cpu->reg;

  unsigned char valB = reg[regB];

  switch (op) {
    case ALU_MUL:
      reg[regA] *= valB;
      break;

    case ALU_ADD:
      reg[regA] += valB;
      break;

    case ALU_INC:
      reg[regA]++;
      break;

    case ALU_DEC:
      reg[regA]--;
      break;

    case ALU_CMP:
      if (reg[regA] == valB) {
        // Set the last bit of FL to 1
        cpu->FL = cpu->FL | (1 << 0);
      } else if (reg[regA] > valB) {
        // Set the second-to-last bit of FL to 1
        cpu->FL = cpu->FL | (1 << 1);
      } else {
        // Set the third-to-last bit of FL to 1
        cpu->FL = cpu->FL | (1 << 2);
      }
      break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  // Just so we don't have to type cpu-> every time
  unsigned char *reg = cpu->reg;
  unsigned char *ram = cpu->ram;

  int running = 1; // True until we get a HLT instruction

  while (running) {
    unsigned char operandA = NULL;
    unsigned char operandB = NULL;
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned int num_operands = IR >> 6;

    // &'ing by 0xff is essentially modding by 0xff, or 255
    // This just ensures that we wrap around if we ever reach
    // the end of the 255 bytes of allocated RAM and don't go
    // past it
    if (num_operands == 2) {
      operandA = cpu_ram_read(cpu, (cpu->PC + 1) & 0xff);
      operandB = cpu_ram_read(cpu, (cpu->PC + 2) & 0xff);
    } else if (num_operands == 1) {
      operandA = cpu_ram_read(cpu, (cpu->PC + 1) & 0xff);
    } else {}

    // True if this instruction might set the PC
    // This line is shifting the instruction by 4 bits to access
    // the flag that indicates whether the PC might be set, and
    // then &'ing it to see if the bit is set to 0 or 1
    int instruction_set_pc = (IR >> 4) & 1;

    switch (IR) {

      case LDI:
        reg[operandA] = operandB;
        break;

      case PRN:
        printf("%d\n", reg[operandA]);
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;

      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        break;

      case HLT:
        running = 0;
        break;

      case PRA:
        printf("%c\n", reg[operandA]);
        //printf("%c", reg[operandA]); fflush(stdout); // Without newline
        break;


      case PUSH:
        cpu_push(cpu, reg[operandA]);
        break;

      case POP:
        reg[operandA] = cpu_pop(cpu);
        break;

      case INC:
        alu(cpu, ALU_INC, operandA, operandB);
        break;

      case DEC:
        alu(cpu, ALU_DEC, operandA, operandB); 
        break;

      case CMP:
        alu(cpu, ALU_CMP, operandA, operandB);
        break;

      default:
        fprintf(stderr, "PC %02x: unknown instruction %02x\n", cpu->PC, IR);
        exit(3);
    }

    if (!instruction_set_pc) {
      // Increment PC by the number of arguments that were passed
      // to the instruction we just executed
      // We do this by shifting 6 bits and modding by 4 to access
      // the 1st and 2nd bits of the IR, which indicate how many
      // operands the previous instruction expected
      // Plus 1 because that is the size of the opcode itself
      cpu->PC += num_operands + 1;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  cpu->FL = 0;

  // Zero registers and RAM
  memset(cpu->reg, 0, sizeof cpu->reg);
  memset(cpu->ram, 0, sizeof cpu->ram);

  // Initialize SP
  cpu->reg[SP] = ADDR_EMPTY_STACK;
}