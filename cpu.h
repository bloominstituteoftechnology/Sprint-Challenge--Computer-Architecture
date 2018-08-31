#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu // implement step 1 so that this struct can be used in cpu.c
{
  unsigned char PC;       // program counter
  unsigned char reg[8];   // allocate memory to registers
  unsigned char ram[256]; // allocate 256 bytes of memory to ram
};

// Special register values
#define SP 5

// ALU operations
enum alu_op
{
  ALU_MUL,
  ALU_ADD
};

// Memory locations
#define ADDR_PROGRAM_ENTRY 0x00 // Where programs start getting loaded
#define ADDR_EMPTY_STACK 0xF4   // Where SP is on an empty stack

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define ADD 0b10100000
#define HLT 0b00000001
#define LDI 0b10000010
#define MUL 0b10100010
#define PRA 0b01001000
#define PRN 0b01000111
#define PUSH 0b01000101
#define POP 0b01000110

// Function declarations

extern void cpu_load(char *filename, struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
