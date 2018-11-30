#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  unsigned char PC;
  // registers (array) the number of wires
  unsigned char registers[8];
  // ram (array)
  unsigned char ram[256];
  // set up flags
  unsigned char flags[3]; //not sure about this: equal is flags[0], less is flags[1], greater is flags[2]
};

/* 
  r7 reserved for stack pointer
  F3 start of stack
  initiate value for string pointer
*/

  #define SP 7 // SP is 7 # tells the compiler directly to change instances of SP to the number 7

// ALU operations - arithmetic logic unit, a digital circuit used to perform arithmetic and logic operations
enum alu_op {
  ALU_ADD,
	ALU_MUL,
  ALU_SUB,
  ALU_DIV
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010 //<-this is 130
// TODO: more instructions here. These can be used in cpu_run().
#define ADD 0b10100000 // 160
#define CALL 0b01010000 // 80
#define HLT 0b00000001 // 1
#define PRN 0b01000111 // 71
#define RET 0b00010001 // 17
#define LD 0b10000011 // 131
#define ST 0b10000100 // 132
#define PRA 0b01001000 // 72 
#define IRET 0b00010011 // 19
#define MUL 0b10100010 // 162
#define CMP 0b10100111 // 167
#define JEQ 0b01010101 // 85
#define INC 0b01100101 // 101
#define DEC 0b01100110 // 102
#define DIV 0b10100011 // 163
#define SUB 0b10100001 // 161
#define JNE 0b01010110 // 86
#define POP 0b01000110 // 70
#define PUSH 0b01000101 // 69
#define JMP 0b01010100 // 84
#define CMP 0b10100111 // 167

// Function declarations

extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
