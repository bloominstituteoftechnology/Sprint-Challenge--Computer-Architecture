#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu
{
  unsigned char PC;
  unsigned char FL;
  unsigned char registers[8];
  unsigned char ram[256];
};

// ALU operations
enum alu_op
{
  ALU_ADD,
  ALU_AND,
  ALU_CMP,
  ALU_MOD,
  ALU_MUL,
  ALU_NOT,
  ALU_OR,
  ALU_SHL,
  ALU_SHR,
  ALU_XOR
  // Add more here
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define ADD 0b10100000
#define AND 0b10101000
#define CALL 0b01010000
#define CMP 0b10100111
#define HLT 0b00000001
#define JEQ 0b01010101
#define JMP 0b01010100
#define JNE 0b01010110
#define LDI 0b10000010
#define MOD 0b10100100
#define MUL 0b10100010
#define NOT 0b01101001
#define OR 0b10101010
#define POP 0b01000110
#define PRN 0b01000111
#define PUSH 0b01000101
#define RET 0b000010001
#define SHL 0b10101100
#define SHR 0b10101101
#define XOR 0b10101011
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *file);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
