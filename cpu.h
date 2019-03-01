#ifndef _CPU_H_
#define _CPU_H_


struct cpu {
  unsigned int pc;
  unsigned int flag_equals;
  unsigned int flag_greater_than;
  unsigned int flag_less_than;
  unsigned char reg[8]; 
  unsigned char ram[256];
};


enum alu_operations {
	ALU_MULTIPLIER,
	ALU_ADDITION,
  ALU_CMP_INSTRUCTION
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define HLT  0b00000001
#define PRN  0b01000111
#define MUL  0b10100010
#define PUSH 0b01000101
#define POP  0b01000110
#define ADD  0b10100000
#define CALL 0b01010000
#define RET  0b00010001
#define CMP  0b10100111
#define JEQ  0b01010101
#define JNE  0b01010110
#define JMP  0b01010100

// Function declarations

extern void cpu_load(struct cpu *cpu, char *arg);
extern void cpu_init(struct cpu *cpu);
extern void cpu_runner(struct cpu *cpu);

#endif