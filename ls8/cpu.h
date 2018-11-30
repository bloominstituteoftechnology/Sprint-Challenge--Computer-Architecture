#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  int PC;
  unsigned char registers[8];
  unsigned char ram[256];
};

// ALU operations
enum alu_op {
	ALU_MUL
	// Add more here
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI 0b10000010
#define PRN 0b01000111
#define HLT 0b00000001
#define MUL 0b10100010
#define PUSH 0b01000101
#define POP 0b01000110
#define CMP 10100111
#define JEQ 01010101
#define JNE 01010110
#define JMP 01010100

// TODO: more instructions here. These can be used in cpu_run().
// Function declarations
extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
