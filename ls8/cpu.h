#ifndef _CPU_H_
#define _CPU_H_

#define MAX_ADDRESS 0xff // Max address 0xff = 255
#define SP 7 // Stack pointer
#define IS 6 // Interrupt status
#define IM 5 // Interrupt mask

/* // Setting flags to initial 0s
#define EQUAL_FLAG 0x00 // Equal flag (E)
#define LESS_THAN_FLAG 0x00 // Less than flag (L)
#define GREATER_THAN_FLAG 0x00 // Greater than flag (G) */

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  unsigned char PC;

  // registers (array)
  unsigned char reg[8];

// flags
  unsigned char EQUAL_FLAG;
  unsigned char LESS_THAN_FLAG;
  unsigned char GREATER_THAN_FLAG;

  // ram (array)
  unsigned char ram[MAX_ADDRESS + 1];
};

// ALU operations
enum alu_op {
	ALU_MUL,
	// Add more here
  ALU_ADD,
  ALU_DIV,
  ALU_SUB,
  ALU_MOD,
  ALU_CMP,
  ALU_LD,
  ALU_AND,
  ALU_OR,
  ALU_XOR,
  ALU_SHL,
  ALU_SHR,
  ALU_ADDI,
  ALU_LDI,
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
// TODO: more instructions here. These can be used in cpu_run().
#define ADD 0b10100000
#define AND 0b10101000
#define CALL 0b01010000
#define CMP 0b10100111
#define DEC 0b01100110
#define DIV 0b10100011
#define HLT 0b00000001
#define INC 0b01100101
#define INT 0b01010010
#define IRET 0b00010011
#define JEQ 0b01010101
#define JGE 0b01011010
#define JGT 0b01010111
#define JLE 0b01011001
#define JLT 0b01011000
#define JMP 0b01010100
#define JNE 0b01010110
#define LD 0b10000011
#define MOD 0b10100100
#define MUL 0b10100010
#define NOP 0b00000000
#define NOT 0b01101001
#define OR 0b10101010
#define XOR 0b10101011
#define POP 0b01000110
#define PRA 0b01001000
#define PRN 0b01000111
#define PUSH 0b01000101
#define RET 0b00010001
#define SHL 0b10101100
#define SHR 0b10101101
#define ST 0b10000100
#define SUB 0b10100001
#define ADDI 0b10101110


// Function declarations

extern void cpu_load(struct cpu *cpu, char *arg);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

// RAM functions
extern unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index);
extern void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char value);

#endif
