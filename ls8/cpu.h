#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
typedef struct cpu {
  // TODO
  // PC
  unsigned char pc;
  unsigned char fl;
  // registers (array)
  unsigned char reg[8];
  // ram (array)
  unsigned char ram[256];
} cpu;

//add flag somewhere else below? instructions? 

// ALU operations
enum alu_op {
	ALU_MUL,
  ALU_ADD,
  ALU_CMP,
	// Add more here
};

// Instructions
//memory locations !!!!!!!!!!!!!!!!!  
#define ADD_EMPTY_STACK 0xf4
#define ADD_PROGRAM_ENTRY 0x00


// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define HLT  0b00000001
#define PRN  0b01000111
// TODO: more instructions here. These can be used in cpu_run().
#define MUL  0b10100010
#define ADD  0b10100000
#define PUSH 0b01000101
#define POP  0b01000110
#define CALL 0b01010000
#define RET  0b00010001
#define CMP  0b10100111
#define JMP  0b01010100
#define JEQ  0b01010101
#define JNE  0b01010110

// Function declarations

extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
