#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu
{
  unsigned char PC;       // Program Counter
  unsigned char reg[8];   // 8 Registers
  unsigned char ram[256]; // 256 bytes of ram.
  // TODO
  // PC
  // registers (array)
  // ram (array)
};

#define SP 5

// ALU operations
enum alu_op // enum is making a new integer type.
{
  ALU_MUL,
  ALU_ADD,
  // Add more here
};

// Mem locations:
#define ADDR_EMPTY_STACK 0xF4
#define ADDR_PROGRAM_ENTRY 0x00
// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define ADD 0b10100000
#define PRA 0b01001000
#define RET 0b00010001
#define CALL 0b01010000
#define HLT 0b00000001
#define LDI 0b10000010
#define MUL 0b10100010
#define PRN 0b01000111
#define PUSH 0b01000101
#define POP 0b01000110
#define CMP 0b10100111
#define JEQ 0b01010101
#define JNE 0b01010110
#define JMP 0b01010100

// Function declarations

extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
