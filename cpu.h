#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  unsigned int PC;
  unsigned int SP;
  unsigned int E;
  // registers (array)
  unsigned char reg[8];
  // ram (array)
  unsigned char ram[256];
};

	#define ADDR_PROGRAM_ENTRY 0x00 //when the program is loading
	#define ADDR_EMPTY_STACK 0XF4		//when SP is on the empty stack

 // Inventory what is here
 // Implement struct cpu in cpu.h
 // Add RAM functions cpu_ram_read and cpu_ram_write
 // Implement cpu_init()
 // Implement the core of cpu_run()
 // Implement the HLT instruction handler
 // Add the LDI instruction
 // Add the PRN instruction

// ALU operations
// enum alu_op {
// 	ALU_MUL
// 	// Add more here
// };

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define HLT  0b00000001
#define PRN  0b01000111
#define MUL  0b10100010
#define PUSH 0b01000101
#define POP  0b01000110
#define CALL 0b01010000
#define RET  0b00010001
#define ADD  0b10100000
#define JMP  0b01010100
#define JNE  0b01010110 
#define JEQ  0b01010101
#define CMP  0b10100111

// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
