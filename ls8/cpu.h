#ifndef _CPU_H_
#define _CPU_H_

struct cpu {
  unsigned char PC;
  unsigned char reg[8];
  unsigned char ram[256];
  unsigned char FL;
};

#define STACK_POINTER 5

enum alu_op {
	ALU_MUL
};


#define LDI  0b10000010
#define PRN  0b01000111
#define HLT  0b00000001
#define MUL  0b10100010
#define PUSH  0b01000101
#define POP  0b01000110
#define CMP  0b10100111
#define JMP  0b01010100
#define JEQ  0b01010101
#define JNE  0b01010110

extern void cpu_load(char *filename, struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
