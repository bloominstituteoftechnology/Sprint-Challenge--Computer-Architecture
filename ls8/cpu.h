#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  unsigned int PC;//Program Counter, address of the currantly executing instruction
  unsigned char registers[8];//the register is made up 8 bits
  unsigned char ram[256];//The CPU could support 256 instructions.
  unsigned char SP;
  unsigned char FL;
  //The SP points at the value at the top of the stack (most recently pushed), or at
//address `F4` if the stack is empty.
  // registers (array)
  // ram (array)
  // if not signed or unsigned with a char, it's up to the compiler which it uses
  //
};

// ALU operations
enum alu_op {
	ALU_MUL,
  ALU_ADD
  
	// Add more here
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.
#define SP 7
//#define SS 0xF4 //Stack Start
#define AND  0b10101000
#define OR   0b10101010
#define LDI  0b10000010
#define HLT  0b00000001
#define PRN  0b01000111
#define MUL  0b10100010
#define ADD  0b10100000
#define POP  0b01000110
#define PUSH 0b01000101
#define CALL 0b01010000
#define RET  0b00010001
#define ST   0b10000100
#define CMP  0b10100111
#define JMP  0b01010100
#define JNE  0b01010110
#define JEQ  0b01010101
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *argv[] );
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
