#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  unsigned char PC;
  unsigned char flag_reg[8];
  unsigned char reg[8];
  unsigned char ram[256];

};

// ALU operations
enum alu_op {
	ALU_MUL,
  ALU_ADD
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define PRN 0b01000111
#define HLT 0b00000001
#define LDI 0b10000010
#define MUL 0b10100010
#define PUSH 0b01000101
#define POP 0b01000110
#define CALL 0b01010000
#define RET 0b00010001
#define ADD 0b10100000
#define CMP 0b10100111
#define JMP 0b01010100
#define JEQ 0b01010101
#define JNE 0b01010110


// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);
#endif