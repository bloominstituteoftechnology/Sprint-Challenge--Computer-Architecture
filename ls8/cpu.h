#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  unsigned char PC;
  unsigned char registers[8];
  unsigned char ram[256];
  unsigned char stack_p;
  unsigned char FL;
};

// ALU operations
enum alu_op {
	ALU_MUL,
    ALU_ADD,

	// Add more here
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define HLT  0b00000001
#define PRN  0b01000111
#define MUL  0b10100010
// TODO: more instructions here. These can be used in cpu_run().
#define ADD  0b10100000
#define POP  0b01000110
#define PUSH 0b01000101
#define CALL 0b01010000
#define RET  0b00010001
#define ST   0b10000100
// SPRINT
#define CMP  0b10100111
#define JMP  0b01010100
#define JNE  0b01010110
#define JEQ  0b01010101

// Function declarations

extern void cpu_load(struct cpu *cpu, char*argv[]);  
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
