#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu
{
  unsigned int pc;
  unsigned char registry[8];
  unsigned char ram[256];
  unsigned char SP;
  unsigned char IP;
};

// ALU operations
enum alu_op {
	ALU_MUL
	// Add more here
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define HLT  0b00000001
#define PRN  0b01000111
#define MUL  0B10100010
#define PUSH 0B01000101
#define POP  0B01000110
#define CALL 0B01010000
#define RET  0B00010001
#define ADD  0b10100000
#define ST   0b10000100
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
