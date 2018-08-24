#ifndef _CPU_H_ 
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  unsigned char PC; // PC
  unsigned char reg[8]; // registers (array)
  unsigned char ram[256]; // ram (array)
  int fl; // equal flag
};


// ALU operations
enum alu_op {
  ALU_MUL,
  ALU_ADD
};


// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
// TODO: more instructions here. These can be used in cpu_run().
#define PRN  0b01000111
#define HLT  0b00000001
#define MUL  0b10100010
#define ADD  0b10100000
#define PUSH 0b01000101
#define POP  0b01000110
#define RET  0b00010001
#define CALL 0b01010000
#define CMP  0b10100111
#define JEQ  0b01010101
#define JMP  0b01010100
#define JNE  0b01010110

// Function declarations aka Function Prototypes aka forward declarations aka forward references
  // it's like declaring a variable before assigning it a value; This will prevent 
  // compiler from complaining when it sees the functions being called in ls8.c
extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
