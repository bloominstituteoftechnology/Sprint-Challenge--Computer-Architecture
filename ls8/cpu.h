#ifndef _CPU_H_
#define _CPU_H_

#define SP 7 // Stack pointer

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC keeps track of where we are in the program we're executing
	unsigned int PC;
  // registers (array)  are where we push operand values
	unsigned char reg[8];
  // ram (array) holds everything else
	unsigned char ram[256];

	unsigned char equal;
	unsigned char greater;
	unsigned char less;
};

// ALU operations
enum alu_op {
	ALU_MUL,
	// Add more here
	ALU_ADD,
	ALU_SUB,
	ALU_CMP,
};

#define ADDR_PROGRAM_ENTRY 0x00 // When programs start gettign loaded
#define ADDR_EMPTY_STACK 0xF4   // Where SP is on the empty stack

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define HLT  0b00000001
#define PRN  0b01000111
#define MUL  0b10100010
#define PUSH 0b01000101
#define POP  0b01000110
#define CALL 0b01010000
#define RET  0b00010001
#define RET  0b00010001
#define ADD  0b10100000
#define CMP  0b10100111
#define JEQ 0b01010101
#define JMP 0b01010100
#define JNE 0b01010110
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, unsigned char *argv);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
