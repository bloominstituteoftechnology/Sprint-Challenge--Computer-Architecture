#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  unsigned char PC; // PC
  unsigned char reg[8]; // registers (array)
  unsigned char ram[256]; // ram (array)
};

// ALU operations
enum alu_op {
	ALU_MUL,
  ALU_ADD
	// Add more here
};

#define SP 5

#define ADDR_PROGRAM_ENTRY 0x00
#define ADDR_EMPTY_STACK 0xF4
// Instructions
  
// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define ADD  0b10100000
#define CALL 0b01010000
#define HLT  0b00000001
#define LDI  0b10000010
#define MUL  0b10100010
#define POP  0b01000110
#define PRA  0b01001000
#define PRN  0b01000111
#define PUSH 0b01000101
#define RET  0b00010001
#define JMP  0b01010100

// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
