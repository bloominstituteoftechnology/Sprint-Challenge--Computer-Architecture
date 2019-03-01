#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  unsigned char pc;
  unsigned char flags;
  // registers (array)
  unsigned char reg[8]; // there are 8 total: R0-R7
  // ram (array)
  unsigned char ram[256];  // total amount of storage for this project
};

// ALU operations
enum alu_op {
	ALU_MUL,
	ALU_ADD
	// Add more here
};

#define CMP_LessThan      0b00000100
#define CMP_GreaterThan   0b00000010
#define CMP_Equal         0b00000001

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define HLT  0b00000001
#define PRN  0b01000111
#define MUL  0b10100010
#define ADD  0b10100000
#define CMP  0b10100111
#define PUSH 0b01000101
#define POP  0b01000110
#define CALL 0b01010000
#define RET  0b00010001
#define JMP  0b01010100
#define JEQ  0b01010101
#define JNE  0b01010110
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *pathToProgram);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
