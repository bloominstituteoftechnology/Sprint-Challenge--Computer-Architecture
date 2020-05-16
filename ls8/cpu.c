#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "cpu.h"

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address){
  return cpu->ram[address];
};

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char val){
  cpu->ram[address] = val;
};

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

void cpu_load(struct cpu *cpu, char *path)
{
  FILE *filepointer = fopen(path, "r");

  if (filepointer == NULL)
  {
    printf("Couldn't open the program you entered: %s\n", path);
    exit(2);
  }

  int address = 0;
  char line[1024];
  while (fgets(line, sizeof(line), filepointer) != NULL)
  {
    char *endpoint;
    unsigned char val = strtoul(line, &endpoint, 2) & 0xFF;
    if (endpoint == line)
    {
      continue;
    }
    cpu_ram_write(cpu, address++, val);
  }
  fclose(filepointer);
}
/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *reg = cpu->regs;

  unsigned char valA = cpu->regs[regA];
  unsigned char valB = cpu->regs[regB];

  switch (op)
  {
  case ALU_MUL:
    // TODO
    reg[regA] *= valB;
    break;
  case ALU_ADD:
    reg[regA] += valB;
    break;
    // TODO: implement more ALU ops
  case ALU_CMP:
    if (cpu->regs[regA] == cpu->regs[regB])
    {
      cpu->FL |= 0b00000001;
    }
    if (cpu->regs[regA] < cpu->regs[regB])
    {
      cpu->FL |= 0b00000100;
    }
    if (cpu->regs[regA] > cpu->regs[regB])
    {
      cpu->FL |= 0b00000010;
    }
    break;
  }
}
void push(struct cpu *cpu, unsigned char value)
{
  cpu->regs[7]--;
  cpu_ram_write(cpu, cpu->regs[7], cpu->regs[value]);
  cpu->PC += 2;
}

void pop(struct cpu *cpu, unsigned char value)
{
  cpu->regs[7]++;
  cpu->regs[value] = cpu_ram_read(cpu, cpu->regs[7]);
  cpu->PC += 2;
}


void call(struct cpu *cpu, unsigned char registerOfAddress)
{
  char address = cpu->regs[registerOfAddress];
  char nextInstruction = cpu->PC + 2;

  cpu->regs[7]--;
  cpu_ram_write(cpu, cpu->regs[7], nextInstruction);
  cpu->PC = address;
}

void ret(struct cpu *cpu){
  unsigned char address = cpu_ram_read(cpu, cpu->regs[7]);
  cpu->regs[7]++;
  cpu->PC = address;
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    unsigned char instruction = cpu_ram_read(cpu, cpu->PC);
    unsigned char op0 = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char op1 = cpu_ram_read(cpu, cpu->PC + 2);

    printf("trace: %02X: %02X    %02X   %02X\n", cpu->PC, instruction, op0, op1);

    switch (instruction){
      case LDI:
        cpu->regs[op0] = op1;
        cpu->PC += 3;
        break;
      case PRN:
        printf("%d\n", cpu->regs[op0]);
        cpu->PC += 2;
        break;
      case HLT: //HLT the cpu and exit the emulator
        running = 0;
        break;
      case MUL:
        alu(cpu, ALU_MUL, op0, op1);
        cpu->PC += 3;
        break;
      case ADD:
        alu(cpu, ALU_ADD, op0, op1);
        cpu->PC += 3;
        break;
      case PUSH:
        push(cpu, op0);
        break;
      case POP:
        pop(cpu, op1);
        break;
      case CALL:
        call(cpu, op0);
        break;
      case RET:
        ret(cpu);
        break;
      case CMP:
        alu(cpu, ALU_CMP, op0, op1);
        cpu->PC += 3;
        break;
      case JMP:
        cpu->PC = cpu->regs[op0];
        break;
      case JNE:
        if (!(cpu->FL & 1)){
          cpu->PC = cpu->regs[op0];
        }
        else{
          cpu->PC += 2;
        }
        break;
      case JEQ:
        if (cpu->FL & 1){
          cpu->PC = cpu->regs[op0];
        }
        else{
          cpu->PC += 2;
        }
        break;
      default:
        printf("unexpected instruction 0x%02X at 0x%02X\n", instruction, cpu->PC);
        exit(1);
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
  for(int i = 0; i < 7; i++){
    cpu->regs[i] = 0;
  }
  cpu->regs[7] = 0xF4;

  cpu->PC = 0;
  cpu->FL = 0;
  memset(cpu->ram, 0, sizeof cpu->ram);
}
