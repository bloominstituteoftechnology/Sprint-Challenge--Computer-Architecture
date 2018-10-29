#ifndef _CPU_H_
#define _CPU_H_

// .h extension implies this is a header file (to be included with a 
// preprocessor #include directive.) Contains stuff that is normally deemed
// to be shared with other parts of your code, like function prototypes, 
// #define'd stuff, external declaration for global vars and the like. 


// Holds all information about the CPU
struct cpu {
  // TODO
  // PC (Program Counter)
  unsigned char PC;
  // registers (array)
  unsigned char reg[8];
  // ram (array)
  unsigned char ram[256];
};

// ALU operations
enum alu_op {
	ALU_MUL
	// Add more here
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define PRN  0b01000111
#define HLT  0b00000001
#define MUL  0b10100010
#define JMP  0b01010100
#define PUSH 0b01000101
#define POP  0b01000110 
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
