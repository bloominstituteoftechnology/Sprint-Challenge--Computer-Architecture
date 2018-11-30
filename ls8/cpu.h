#ifndef _CPU_H_
#define _CPU_H_

struct cpu {
  unsigned char reg[8];//Registers 0-7
  unsigned char ram[256];//RAM
  
  unsigned char PC;//Program Counter
  unsigned char FL;//Flags `00000LGE`
  unsigned char IR;//Instruction Register - the currently executing instruction
  unsigned char MAR;//Memory Address Register - the memory address We're reading or writing 
  unsigned char MDR; //Memory Data Register - holds the value to write or the value just read
};

// ALU operations
enum alu_op {
  ALU_MUL,
  ALU_ADD,
  ALU_AND,
  ALU_OR,
  ALU_XOR,
  ALU_NOT,
  ALU_SHL,
  ALU_SHR,
  ALU_MOD
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hexliterals should be used.

#define ADD   0b10100000
#define AND   0b10101000
#define CALL  0b01010000
#define CMP   0b10100111
#define DEC   0b01100110
#define LDI   0b10000010
#define JEQ   0b01010101
#define JMP   0b01010100
#define HLT   0b00000001 
#define INC   0b01100101
#define LD    0b10000011
#define JNE   0b01010110
#define MUL   0b10100010
#define MOD   0b10100100
#define NOT   0b01101001
#define OR    0b10101010
#define PRN   0b01000111
#define PUSH  0b01000101
#define POP   0b01000110
#define PRA   0b01001000
#define RET   0b00010001
#define SHL   0b10101100
#define SHR   0b10101101
#define ST    0b10000100
#define XOR   0b10101011

// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *argv);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);
//, int *regNum

#endif