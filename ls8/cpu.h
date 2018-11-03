#ifndef _CPU_H_
#define _CPU_H_

// all the information about the cpu
struct cpu {
  unsigned char PC; // our program counter
  unsigned char registers[8]; // our array of register addresses, from 0-7
  unsigned char RAM[256]; // our RAM array
  unsigned char FL; // our flag for the CMP 
};

// Special register values
// this is our stack pointer
#define SP 5

// ALU operations
enum alu_op {
  ALU_MUL, 
  ALU_ADD
};

// memory locations
#define ADDR_PROGRAM_ENTRY 0x00  // where programs start being loaded
#define ADDR_EMPTY_STACK 0xF4 // Spot where SP is on an empty stack

// binary literals
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
#define MULT2PRINT 0b00011000
#define CMP  0b10100111
#define JMP  0b01010100
#define JEQ  0b01010101
#define JNE  0b01010110


extern void cpu_load(char *filename, struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif