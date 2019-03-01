#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu
{
  // TODO
  // PC = program counter;
  unsigned int PC;
  // FL = flag
  unsigned int FL;
  // registers (array)
  unsigned char reg[8];
  // ram (array)
  unsigned char ram[256];
};

// ALU operations
enum alu_op
{
  ALU_MUL,
  ALU_ADD,
  ALU_CMP
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI 0b10000010
#define HLT 0b00000001
#define PRN 0b01000111
#define MUL 0b10100010
#define PUSH 0b01000101
#define POP 0b01000110
#define CALL 0b01010000
#define RET 0b00010001
#define ADD 0b10100000

// compare the values in 2 registers
#define CMP 0b10100111
// jump to the address stored in the given register
#define JMP 0b01010100
// If equal flag is set (true), jump to the address stored
// in the given register
#define JEQ 0b01010101
// If E flag is clear (false, 0), jump to the address stored
// in the given register.
#define JNE 0b01010110

// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
