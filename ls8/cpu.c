#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cpu.h"

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char pos)
{
  return cpu->ram[pos];
}

void cpu_ram_write(struct cpu *cpu, unsigned char pos, unsigned char value)
{
  cpu->ram[pos] = value;
}

void stack_push(struct cpu *cpu, unsigned char value)
{
  cpu->registers[SP]--;
  cpu->ram[cpu->registers[SP]] = value;
}

unsigned char stack_pop(struct cpu *cpu)
{
  unsigned char retVal = cpu->ram[cpu->registers[SP]];
  cpu->registers[SP]++;
  return retVal;
}
/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *path)
{
  FILE *fp;
  char line[8192];
  int address = 0;

  if((fp = fopen(path, "r")) == NULL){
    printf("Error opening file.\n");
    exit(2);
  }

  while(fgets(line, sizeof line, fp) != NULL) {
    char *endptr;
    unsigned char curr = strtol(line, &endptr, 2);

    if(endptr == line){
      continue;
    }

    cpu->ram[address++] = curr;
  }
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->registers[regA] *= cpu->registers[regB];
      break;
    case ALU_ADD:
      cpu->registers[regA] += cpu->registers[regB];
      break;
    case ALU_DIV:
      if(!cpu->registers[regB]){
        printf("Error: Divide by 0. Exiting...\n");
        exit(3);
      }
      cpu->registers[regA] /= cpu->registers[regB];
      break;
    case ALU_MOD:
      if(!cpu->registers[regB]){
        printf("Error: Divide by 0. Exiting...\n");
        exit(3);
      }
      cpu->registers[regA] %= cpu->registers[regB];
      break;
  }
  cpu->registers[regA] = cpu->registers[regA] & 0xFF;
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
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    int IR = cpu->ram[cpu->PC];
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);

    switch(IR)
    {
      case HLT:
        running = 0;
        break;
      case LDI:
        cpu->registers[operandA] = operandB;
        // cpu->PC += 3;
        break;
      case PRN:
        printf("%d\n", cpu->registers[operandA]);
        // cpu->PC += 2;
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        // cpu->PC += 3;
        break;
      case PUSH:
        stack_push(cpu, cpu->registers[operandA]);
        break;
      case POP:
        cpu->registers[operandA] = stack_pop(cpu);
        break;
      case CALL:
        stack_push(cpu, cpu->PC + 2);
        cpu->PC = cpu->registers[operandA];
        break;
      case RET:
        cpu->PC = stack_pop(cpu);
        break;
      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        break;
      case ST:
        cpu_ram_write(cpu, cpu->registers[operandA], cpu->registers[operandB]);
        break;
      case AND:
        cpu->registers[operandA] = cpu->registers[operandA] & cpu->registers[operandB];
        break;
      case DEC:
        cpu->registers[operandA] -= 1;
        break;
      case DIV:
        alu(cpu, ALU_DIV, operandA, operandB);
        break;
      case INC:
        cpu->registers[operandA] += 1;
        break;
      case MOD:
        alu(cpu, ALU_MOD, operandA, operandB);
        break;
      case NOP:
        break;
      case NOT:
        cpu->registers[operandA] = ~cpu->registers[operandA];
        break;
      case OR:
        cpu->registers[operandA] = cpu->registers[operandA] | cpu->registers[operandB];
        break;
      case PRA:
        printf("%c\n", cpu->registers[operandA]);
        break;
      case SHL:
        cpu->registers[operandA] = cpu->registers[operandA] << cpu->registers[operandB];
        break;
      case SHR:
        cpu->registers[operandA] = cpu->registers[operandA] >> cpu->registers[operandB];
        break;
      case CMP:
        cpu->FL = cpu->FL & 0b11111000;
        if(cpu->registers[operandA] == cpu->registers[operandB]){
          cpu->FL = cpu->FL | 0b00000001;
        }

        if(cpu->registers[operandA] < cpu->registers[operandB]){
          cpu->FL = cpu->FL | 0b00000100;
        }

        if(cpu->registers[operandA] > cpu->registers[operandB]){
          cpu->FL = cpu->FL | 0b00000010;
        }
        break;
      case JMP:
        cpu->PC = cpu->registers[operandA];
        break;
      case JEQ:
        if((cpu->FL & 0b00000001)){
          cpu->PC = cpu->registers[operandA];
        }else{
          cpu->PC += (IR >> 6 & 3) + 1;
        }

        break;
      case JNE:
        if(!(cpu->FL & 0b00000001)){
          cpu->PC = cpu->registers[operandA];
        }else{
          cpu->PC += (IR >> 6 & 3) + 1;
        }
        break;
      case XOR:
        cpu->registers[operandA] = cpu->registers[operandA] ^ cpu->registers[operandB];
        break;
      default:
        printf("Command: %d\n", IR);
        printf("Unknown Command. Exiting...\n");
        exit(3);
    }
    if(!(IR >> 4 & 1)){
      cpu->PC += (IR >> 6 & 3) + 1;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  cpu->FL = 0;
  // TODO: Zero registers and RAM
  memset(cpu->registers, 0, sizeof(cpu->registers));
  memset(cpu->ram, 0, sizeof(cpu->ram));
  cpu->registers[SP] = SS;
}
