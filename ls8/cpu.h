#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // PC
	unsigned char PC;
  // registers (array)
	unsigned char reg[8];
  // ram (array)
	unsigned char ram[256];
	// flag register
	unsigned char FL;
	// stack pointer
	unsigned char SP;
};

// ALU operations
enum alu_op {
	ALU_MUL,
	ALU_ADD,
	ALU_CMP,
	ALU_AND,
	ALU_OR,
	ALU_XOR,
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

// CPU Instructions
#define HLT  0b00000001
#define LDI  0b10000010
#define PUSH 0b01000101
#define POP  0b01000110
#define PRN  0b01000111

// Set PC
#define RET  0b00010001
#define CALL 0b01010000
// -- Jump and Jump-If
#define JMP  0b01010100
#define JEQ  0b01010101
#define JNE  0b01010110
#define JGT  0b01010111
#define JLT  0b01011000
#define JLE  0b01011001
#define JGE  0b01011010

// ALU Instructions
#define ADD  0b10100000
#define MUL  0b10100010
#define CMP  0b10100111
#define AND  0b10101000
#define OR   0b10101010
#define XOR  0b10101011

// Function declarations

extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
