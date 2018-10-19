#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  unsigned char PC;    // PC the type for a single unsigned byte in C; register the pc

  unsigned char reg[8];  // registers (array) create an array with 8 slots in it; we have to specify unsigned with a char otherwise the compiler will be the one to use which one to use

  unsigned char ram[256];   // ram (array) holds the rest of the memory

  unsigned char MAR; //holds the memory address we're reading or writing

  unsigned char MDR;  //holds the value to write or the value just read

  unsigned char IR;

  unsigned FL; //holds current flag status
};

// ALU operations
enum alu_op {
	ALU_MUL,  //assigned an integer value and defaults to starting at 0
	// Add more here
	ALU_ADD,
	ALU_CMP
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010 //load
#define PRN 0b01000111 //print
#define HLT 0b00000001
#define MUL  0b10100010
#define ADD  0b10100000
#define POP  0b01000110
#define PUSH 0b01000101
#define JMP 0b01010100  //Jump
#define CALL 0b01010000
#define RET 0b00010001
#define AND  0b10101000
#define JEQ 0B01010101  //
#define JNE 0b01010110  //Jump
#define CMP 0b10100111
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
