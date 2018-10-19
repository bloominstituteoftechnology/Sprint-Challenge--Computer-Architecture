#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
	unsigned char PC;
	unsigned char IR;
	unsigned char MAR;
	unsigned char MDR;
	unsigned char FL;
	unsigned char registers[8];
	unsigned char ram[256];
};

// ALU operations
enum alu_op {
	ALU_MUL,
	ALU_CMP
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI 0b10000010
#define PRN 0b01000111
#define HLT 0b00000001
#define MUL 0b10100010
#define CMP 0b10100111
#define JMP 0b01010100
#define JEQ 0B01010101
#define JNE 0b01010110

// Function declarations

extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
