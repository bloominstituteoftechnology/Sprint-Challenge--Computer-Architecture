#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  int PC;  // PC
  unsigned char FL;
  unsigned char registers[8];  // registers (array)
  unsigned char ram[256];  // ram (array)
};

// ALU operations
enum alu_op {
	ALU_MUL,
  ALU_ADD
};

// Instructions
// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.
#define SP 7  // stack pointer
#define IM 5  // stack pointer
#define IS 6  // stack pointer
#define LDI  0b10000010
#define HLT  0b00000001
#define PRN  0b01000111
#define MUL  0b10100010
#define ADD  0b10100000
#define PUSH  0b01000101
#define POP  0b01000110
#define CALL  0b01010000
#define RET  0b00010001
#define ST  0b10000100
#define JMP  0b01010100
#define PRA  0b01001000
#define IRET  0b00010011
#define INT  0b01010010

// Function declarations
extern void cpu_load(struct cpu *cpu, char *file);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
