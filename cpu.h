#ifndef _CPU_H_
#define _CPU_H_

// #define SP 7

// Holds all information about the CPU
struct cpu
{
    // TODO    // PC    // registers (array)    // ram (array)
    unsigned char pc;
    unsigned char reg[8];
    unsigned char ram[256];
    unsigned char fl;
};

// ALU operations
enum alu_op
{
    ALU_MUL,
    // Add more here
    ALU_ADD,
    ALU_CMP
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI 0b10000010
#define MUL 0b10100010
#define HLT 0b00000001
#define PRN 0b01000111
#define PUSH 0b01000101
#define POP 0b01000110
#define JMP 0b01010100
#define CALL 0b01010000
#define RET 0b00010001
#define CMP 0b10100111
#define JEQ 0B01010101
#define JNE 0b01010110
#define ADD 0b10100000
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif