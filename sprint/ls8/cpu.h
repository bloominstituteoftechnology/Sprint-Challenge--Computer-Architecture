#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
      // TODO
      // The type for a single unsigned byte in C is : (unsigned char x;)

      // PC = program counter...address of current instruction
      unsigned char PC;
      
      // registers (array)
      /* 8 registers
          - R5 is reserved for interrupt mask
          - R6 is reserved for interrupt status
          - R7 is reserved for stack pointer
      */
      unsigned char reg[8];

      // ram (array)
      unsigned char ram[256]; //8-bit cpu .. 2^8 = 256 
      //stack 
      unsigned char SP;

};

// ALU operations
enum alu_op {
	  ALU_MUL,
	  // Add more here
    ALU_ADD,
    ALU_SUB,
    ALU_DIV
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define HLT  0b00000001
#define PRN  0b01000111
// TODO: more instructions here. These can be used in cpu_run().
#define ADD  0b10100000
#define MUL  0b10100010
#define SUB  0b10100001
#define DIV  0b10100011
//for stack...
#define POP  0b01000110
#define PUSH 0b01000101

//CALL 01010000 00000rrr
//RET  0001 0001
#define CALL 0b01010000 //01010000
#define RET  0b00010001

//FOR  SPRINT
#define CMP 0b10100111
#define JMP 0b01010100
#define JEQ 0b01010101
#define JNE 0b01010110
#define MOD 0b10100100
#define AND 0b10101000

// Function declarations

extern void cpu_load(char *filename, struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
