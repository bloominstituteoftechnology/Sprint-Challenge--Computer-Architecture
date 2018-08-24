#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  int L; // less than flag
  int G; // greater than flag
  int E; // equal flag
  unsigned char PC;
  // registers (array)
  unsigned char registers[8];
  // ram (array)
  unsigned char ram[256];
//   An 8 bit CPU is one that only has 8 wires available for addresses (specifying
// where something is in memory), computations, and instructions. With 8 bits, our
// CPU has a total of 256 bytes of memory and can only compute values up to 255.
// The CPU could support 256 instructions, as well, but we won't need them.
};
#define L_FLAG 0
#define G_FLAG 0
#define E_FLAG 0
#define SP 7

enum alu_op {
  ALU_MUL,
  ALU_ADD
};
// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.
#define CMP  0b10100111
#define JMP  0b01010100
#define JNE  0b01010110
#define JEQ  0b01010101
#define LDI  0b10000010
#define MUL  0b10100010
#define HLT  0b00000001
#define ADD  0b10100000
#define CALL 0b01010000
#define POP  0b01000110
#define PRA  0b01001000
#define PRN  0b01000111
#define PUSH 0b01000101
#define RET  0b00010001
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(char *filename, struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
