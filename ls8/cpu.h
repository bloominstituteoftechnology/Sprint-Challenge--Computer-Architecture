#ifndef _CPU_H_
#define _CPU_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  unsigned int PC;
  // registers (array)
  unsigned char registers[8];
  // ram (array)
  unsigned char ram[256];
};

// ALU operations
enum alu_op {
	ALU_MUL,
	// Add more here
  ALU_ADD
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define HLT  0b00000001
#define PRN  0b01000111
// TODO: more instructions here. These can be used in cpu_run().
#define MUL  0b10100010
#define PUSH 0b01000101
#define POP  0b01000110
#define CALL 0b01010000
#define RET  0b00010001
#define JMP  0b01010100
#define ADD  0b10100000
#define ST   0b10000100
#define JMP  0b01010100
#define PRA  0b01001000
#define IRET 0b00010011

// Function declarations

extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);
extern unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address);
extern void cpu_ram_write(struct cpu *cpu, unsigned char value, unsigned char address);

#endif
