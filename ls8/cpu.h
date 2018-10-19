#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu
{
  // TODO
  // PC
  unsigned char PC;
  // registers (array)
  unsigned char reg[8];
  // ram (array)
  unsigned char ram[256];
  unsigned char FL[8];
  
};

// ALU operations
enum alu_op
{
  ALU_MUL,
  // Add more here
  ALU_ADD
};

// Special register values
#define SP 5
// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.
// Memory locations
#define ADDR_PROGRAM_ENTRY 0x00 // Where programs start getting loaded
#define ADDR_EMPTY_STACK 0xF4   // Where SP is on an empty stack

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI 0b10000010
#define LD 0b10000011
#define PRN 0b01000111
#define PRA 0b01001000
#define HLT 0b00000001
#define MUL 0b10100010
#define ADD 0b10100000
#define INC 0b01100101
#define DEC 0b01100110
#define PUSH 0b01000101
#define POP 0b01000110
#define CALL 0b01010000
#define RET 0b00010001
#define CMP 0b10100111
#define JEQ 0b01010101
#define JMP 0b01010100
#define JNE 0b01010110

extern void cpu_load(char *filename, struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
