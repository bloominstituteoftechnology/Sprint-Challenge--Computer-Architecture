#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
// #include <unistd.h> // For sleep()
#include "cpu.h"
#include "handlers.h"

#define DEBUG 1

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void throwHandsUpAndGiveUp(void)
{
  perror("Error");
  exit(1);
}

void cpu_load(struct cpu *cpu, char *argv[])
{
  FILE *fp = fopen(argv[1], "r");
  char line[255];
  int address = 0;

  printf("\n"); // print a new line so I better distinguish output in terminal

  while(fgets(line, sizeof(line), fp) != NULL)
  {
    line[9] = '\0';
    if (line[0] == '0' || line[0] == '1') {
      #if DEBUG
      printf("WRITTEN TO ADDR %i: \t%s\n", address, line);
      #endif
      cpu->ram[address++] = strtoul(line, NULL, 2);
    }
  }

  #if DEBUG
  // Print what is currently saved to memory
  printf("\n---Current Memory State---\n");
  int length = address;
  int current = 0;

  while (current <= length) {
    printf("RAM ADDR %i:\t%02x\n", current, cpu->ram[current]);
    current++;
  }
  printf("\n");
  #endif
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op)
{
  int regA = cpu_ram_read(cpu, cpu->PC + 1);
  int regB = cpu_ram_read(cpu, cpu->PC + 2);

  switch (op) {
    case ALU_ADD:
      handleALU_ADD(cpu, regA, regB);
      break;
    case ALU_MUL:
      handleALU_MUL(cpu, regA, regB);
      break;
    case ALU_INC:
      handleALU_INC(cpu, regA);
      break;
    case ALU_DEC:
      handleALU_DEC(cpu, regA);
      break;
    case ALU_CMP:
      handleALU_CMP(cpu, regA, regB);
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
  // Time Measurement
  // Src: http://www.cs.loyola.edu/~jglenn/702/S2008/Projects/P3/time.html
  struct timeval start, end;
  gettimeofday(&start, NULL);

  unsigned char IR;

  #if DEBUG
  printf("---Program Execution---\n");
  #endif

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.

    gettimeofday(&end, NULL);
    if ((end.tv_sec - start.tv_sec) >= 1) {
      #if DEBUG
      printf("\nAt least 1 second has elasped.\n");
      #endif
      gettimeofday(&start, NULL);
    }

    IR = cpu_ram_read(cpu, cpu->PC);

    #if DEBUG
    printf("TRACE: %02x: %02x\n", cpu->PC, IR);
    #endif

    switch (IR) {
      case LD:
        handleLD(cpu);
        break;
      case LDI:
        handleLDI(cpu);
        break;
      case ST:
        handleST(cpu);
        break;
      case PRN:
        handlePRN(cpu);
        break;
      case PRA:
        handlePRA(cpu);
        break;
      case ADD:
        alu(cpu, ALU_ADD);
        break;
      case MUL:
        alu(cpu, ALU_MUL);
        break;
      case INC:
        alu(cpu, ALU_INC);
        break;
      case DEC:
        alu(cpu, ALU_DEC);
        break;
      case CMP:
        alu(cpu, ALU_CMP);
        break;
      case PUSH:
        handlePUSH(cpu);
        break;
      case POP:
        handlePOP(cpu);
        break;
      case CALL:
        handleCALL(cpu);
        break;
      case RET:
        handleRET(cpu);
        break;
      case JMP:
        handleJMP(cpu);
        break;
      case JEQ:
        handleJEQ(cpu);
        break;
      case JNE:
        handleJNE(cpu);
        break;
      case HLT:
        running = 0;
        break;
      default:
        fprintf(stderr, "Unknown instruction at %i: %02x\n", cpu->PC, IR);
        exit(2);
    }
  }

  #if DEBUG
  printf("\n---Current Stack State---\n");
  // Print first 16 positions of the stack in memory
  for (int q = 0xF3; q > (0xF3 - 0x08); q--) {
    printf("stack @ addr:%i:\t%i\n", q, cpu_ram_read(cpu, q));
  }
  // Print current stack pointer
  // At 0th place, SP should be 0xF4/244;
  printf("current stack pointer: %i\n", cpu->SP);
  #endif

  free(cpu->registers);
  free(cpu->ram);
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;            // Program counter
  cpu->SP = 0xF4;         // Stack pointer
  cpu->FL = 0b00000000;   // Flags (for CMP)
  // TODO: Zero registers and RAM
  cpu->registers = (unsigned char *) calloc(8, sizeof(unsigned char));
  cpu->ram = (unsigned char *) calloc(256, sizeof(unsigned char));
}