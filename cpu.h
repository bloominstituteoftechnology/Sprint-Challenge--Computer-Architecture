#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  // registers (array)
  // ram (array)
  int PC;
  int SP;
  int FL;
  unsigned char *registers;
  unsigned char *ram;
};


enum alu_op {
  ALU_MUL,
  ALU_ADD
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define PRN  0b01000111
#define HLT  0b00000001
#define MUL  0b10100010
#define LDI  0b10000010
#define PUS  0b01000101
#define POP  0b01000110
#define CAL  0b01010000
#define RET  0b00010001
#define JMP  0b01010100
#define ADD  0B10100000
#define CMP  0B10100111
#define JEQ  0B01010101
#define JNE  0B01010110
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *argv);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
