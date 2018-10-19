#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>

#define DATA_LEN 6

// Read value from address
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

// Write value to ram at address
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

// pop
unsigned char pop(struct cpu *cpu) 
{
  return cpu_ram_read(cpu, cpu->registers[7]++);
}

// push
void push(struct cpu *cpu, unsigned char val) 
{
  cpu_ram_write(cpu, cpu->registers[7]--, cpu->registers[val]);
}


/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file)
{
  // HARDCODED DATA
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };
  // int address = 0;
  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }

  int address = 0;
  FILE *fp;
	char line[1024];
  fp = fopen(file, "r");

	while (fgets(line, sizeof line, fp) != NULL) {
		// printf("%s", line);

		if (line[0] == '\n' || line[0] == '#') {
			// printf("Ignoring this line.\n");
			continue;
		}

		unsigned char b;
		b = strtoul(line, NULL, 2);

		cpu_ram_write(cpu, address++, b); 
	}

	fclose(fp);
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

    default:
      printf("Invalid ALU instruction");
      exit(3);
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
    unsigned char ir = cpu_ram_read(cpu, cpu->pc);
    // next two bytes
    unsigned char operandA = cpu_ram_read(cpu, cpu->pc+1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->pc+2);

    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    switch (ir) {
      case LDI:
        //printf("Loading\n");
        cpu->registers[operandA] = operandB;
        break;

      case PRN:
        printf("%d\n", cpu->registers[operandA]);
        break;
  
      case HLT:
        //printf("Halting\n");
        running = 0;
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;

      case PUSH:
        //printf("Pushing\n");
        cpu->registers[7]--;
        cpu_ram_write(cpu, cpu->registers[7], cpu->registers[operandA]);
        break;

      case POP:
        //printf("Popping\n");
        cpu->registers[operandA] = cpu_ram_read(cpu, cpu->registers[7]);
        cpu->registers[7]++;
        break;

      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        break;

      case CALL:
        push(cpu, cpu->pc + 2);
        cpu->pc = cpu->registers[operandA];
        break;

      case RET:
        cpu->pc = pop(cpu);
        break;

      case CMP:
        printf("CMP\n");
        if (cpu->registers[operandA] == cpu->registers[operandB]) {
          cpu->fl = 1;
        } else if (cpu->registers[operandA] > cpu->registers[operandB]) {
          cpu->fl = 2;
        } else if (cpu->registers[operandA] < cpu->registers[operandB]) {
          cpu->fl = 4;
        } else {
          cpu->fl = 0;
        }
        break;

      case JMP:
        printf("JMP\n");
        cpu->pc = cpu->registers[operandA];
        break;

      case JEQ:
        printf("JEQ\n");
        if (cpu->fl == 1) {
          cpu->pc = cpu->registers[operandA];
        }
        break;

      case JNE:
        printf("JNE\n");
        if (cpu->fl == 0){
          cpu->pc = cpu->registers[operandA];
        }
        break;

      default:
        printf("Invalid instruction");
        exit(2);
    }

    // 4. Move the PC to the next instruction.
    cpu->pc += (ir >> 6) + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;

  // TODO: Zero registers and RAM
  for (int i = 0; i < 8; i++) {
    cpu->registers[i] = 0;
  }
  for (int i = 0; i < 256; i++) {
    cpu->ram[i] = 0;
  }

  cpu->registers[7] = 0xF4;
  cpu->fl = 0x00;

}
