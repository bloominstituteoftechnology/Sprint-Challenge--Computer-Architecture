#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  int fl;
  unsigned char pc;
  unsigned char reg[8];
  unsigned char RAM[256];
  int sp;
} cpu;


// ALU operations
enum alu_op {
  ALU_MUL,
  ALU_ADD
};

// Instructions
#define ADD  0b10100000
#define LDI  0b10000010
#define PRN  0b01000111
#define HLT  0b00000001
#define MUL  0b10100010
#define POP  0b01000110
#define RET  0b00010001
#define CMP  0b10100111
#define JMP  0b01010100
#define JEQ  0b01010101
#define JNE  0b01010110
#define ST   0b10000100
#define PUSH 0b01000101
#define CALL 0b01010000
// Function declarations

extern void cpu_load(struct cpu *cpu, char *argv[]);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
