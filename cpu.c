#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6
#define HLT 0b00000001
#define LDI 0b10000010
#define PRN 0b01000111
#define MUL 0b10100010
#define PUSH 0b01000101
#define POP 0b01000110
#define CALL 0b01010000
#define RET 0b00010001
#define ADD 0b10100000

#define DEBUG 0

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, int data)
{
  cpu->ram[index] = data;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *path)
{

  if (path == NULL)
  {
    path = "./examples/print8.ls8";
  }

  FILE *fileToRead = fopen(path, "r");
  if (fileToRead == NULL)
  {
    printf("error reading file");
    exit(1); // end program execution if file cannot be read.
  }
  char line[256];
  int address = 0;
  while (fgets(line, 256, fileToRead))
  {
    if (line[0] == '0' || line[0] == '1')
    {
      cpu->ram[address++] = strtoul(line, NULL, 2);
    }
  }

  #if DEBUG
  int current = 0;
  printf("address: %d\n", address);
  while (current <= address) 
  {
    printf("RAM ADDR %i:\t%d\n", current++, cpu->ram[current]);
  }
  #endif
}

/**
 * ALU
 */
//
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
    case ALU_MUL:
      cpu->R[regA] *= cpu->R[regB];
      break;

    case ALU_ADD:
      cpu->R[regA] += cpu->R[regB];
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

  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char prn;
    int result;
    int a;
    sleep(1);
    int oppA = cpu_ram_read(cpu, cpu->PC + 1);
    int oppB = cpu_ram_read(cpu, cpu->PC + 2);
    // 2. switch() over it to decide on a course of action.
    // printf("Performing instruction IR: %d\n", IR);
    switch (IR)
    {
      case HLT: // HLT
        #if DEBUG
        printf("halted\n");
        #endif
        running = 0; 
        break;

      case LDI:
        a = cpu->R[oppA] = oppB;
        cpu->PC += 3;
        break;

      case PRN: 
        prn = cpu->R[oppA];
        printf("%d\n", prn);
        cpu->PC += 2;
        break;

      case MUL: 
        alu(cpu, ALU_MUL,oppA,oppB);
        cpu->PC += 3;
        break;

      case PUSH: 
        cpu->SP--;
        cpu->ram[cpu->SP] = cpu->R[oppA];
        cpu->PC += 2;
        // printf("SP in PUSH %d\n", cpu->ram[cpu->SP]);
        break;

      case POP:
        cpu->R[oppA] = cpu->ram[cpu->SP];
        // printf("SP in POP %d\n", cpu->ram[cpu->SP]);
        cpu->SP++;
        cpu->PC += 2;
        break;

      case CALL: 
        cpu->ram[--cpu->SP] = cpu->PC + 2;
        cpu->PC = cpu->R[oppA];
        break;

      case RET: 
        // printf("PC IS POINTING AT\n");
        cpu->PC = cpu->ram[cpu->SP++];
        break;

      case ADD: 
        alu(cpu, ALU_ADD,oppA,oppB);
        cpu->PC = cpu->PC + 3;
        break;

      default:
        printf("whatmeantho? %d\n", IR);
        break;
    }
  }
}
/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers

  // TODO: Zero registers and RAM <-- O_O :eyes:
  for (int i = 0; i <= 7; i++)
  {
    if (i <= 6)
    {
      cpu->R[i] = 0;
    }
    else
    {
      cpu->R[i] = 0xF4;
      cpu->SP = cpu->R[i];
      // printf("this thing worked stack pointer %d\n", cpu->SP);
    }
    //  }
  }
  cpu->PC = 0;
  cpu->FL = 0;

  for (int i = 0; i <= 255; i++)
  {
    cpu->ram[i] = 0;
  }
}