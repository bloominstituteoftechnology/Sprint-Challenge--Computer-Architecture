#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  // registers (array)
  // ram (array)
  unsigned char PC;
  unsigned char registers[8];
  unsigned char ram[256];
  unsigned char FL;
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
// TODO: more instructions here. These can be used in cpu_run().
#define HLT  0b00000001
#define PRN  0b01000111
#define MUL  0b10100010
#define PUSH 0b01000101
#define POP  0b01000110
#define CMP  0b10100111
#define JMP  0b01010100
#define JEQ  0b01010101
#define JNE  0b01010110
#define SP 7
// Function declarations

extern void cpu_load(struct cpu *cpu, char *argv[]);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
