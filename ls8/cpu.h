#ifndef _CPU_H_
#define _CPU_H_
#define SP 5

struct cpu
{
  unsigned char FL;
  unsigned char PC;
  unsigned char reg[8];
  unsigned char ram[256];
};

enum alu_op
{
  ALU_MUL,
  ALU_ADD,
  ALU_CMP
};

#define ADDR_PROGRAM_ENTRY 0x00
#define ADDR_EMPTY_STACK 0xF4
#define ADD 0b10100000
#define CALL 0b01010000
#define HLT 0b00000001
#define LDI 0b10000010
#define MUL 0b10100010
#define POP 0b01000110
#define PRA 0b01001000
#define PRN 0b01000111
#define PUSH 0b01000101
#define RET 0b00010001
#define CMP 0b10100111
#define JEQ 0b01010101
#define JMP 0b01010100
#define JNE 0b01010110

extern void cpu_load(char *filename, struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif