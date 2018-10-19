#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC Program counter. Index of which instruction we need to execute
  int pc;
  // registers (array)
  unsigned char registers[8];
  // ram (array)
  unsigned char ram[256];
  // Flags
  unsigned char FL;
};

// ALU operations
enum alu_op {
	ALU_MUL,
	// Add more here
  NOTHING
};


// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define PRN  0b01000111
#define HLT  0b00000001
#define MUL  0b10100010
#define PUSH  0b01000101
#define POP 0b01000110
#define ADD 0b10100000
#define CALL 0b01010000 
#define RET 0b00010001
#define ST 0b10000100
#define CMP 0b10100111
#define JMP 0b01010100
#define JEQ 0b01010101
#define JNE 0b01010110

// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char* fileName);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);
extern unsigned char cpu_ram_read(struct cpu *cpu, int mar);
extern void cpu_ram_write(struct cpu *cpu, int mar, unsigned char value);
extern int handle_LDI(struct cpu* cpu, unsigned char regA, unsigned char regB);
extern int handle_PRN(struct cpu* cpu, unsigned char regA, unsigned char regB);
extern int handle_HLT(struct cpu* cpu, unsigned char regA, unsigned char regB);
extern int handle_MUL(struct cpu* cpu, unsigned char regA, unsigned char regB);
extern int handle_PUSH(struct cpu* cpu, unsigned char regA, unsigned char regB);
extern int handle_POP(struct cpu* cpu, unsigned char regA, unsigned char regB);
extern int handle_ADD(struct cpu* cpu, unsigned char regA, unsigned char regB);
extern int handle_CALL(struct cpu* cpu, unsigned char regA, unsigned char regB);
extern int handle_RET(struct cpu* cpu, unsigned char regA, unsigned char regB);
extern int handle_ST(struct cpu* cpu, unsigned char regA, unsigned char regB);
extern int handle_CMP(struct cpu* cpu, unsigned char regA, unsigned char regB);
extern int handle_JMP(struct cpu* cpu, unsigned char regA, unsigned char regB);
extern int handle_JEQ(struct cpu* cpu, unsigned char regA, unsigned char regB);
extern int handle_JNE(struct cpu* cpu, unsigned char regA, unsigned char regB);

#endif
