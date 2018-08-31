#ifndef _CPU_H_
#define _CPU_H_

// Holds all the information about the CPU
struct cpu {
    // PC-Program Counter
    unsigned char pc;
    // registers (array)
    unsigned char reg[8];
    // RAM (array)
    unsigned char ram[256];
};

// special register values
// SP = STACK POINTER
#define SP 5

// ALU operations
enum alu_op {
    ALU_MUL,
    ALU_ADD
};

// Location in Memory
#define ADDR_PROGRAM_ENTRY 0x00 // where programs start getting loaded
#define ADDR_EMPTY_STACK 0xF4 // where SP is on an empty stack

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI 0b10000010
#define ADD  0b10100000
#define CALL 0b01010000
#define HLT  0b00000001
#define MUL  0b10100010
#define POP  0b01000110
#define PRA  0b01001000
#define PRN  0b01000111
#define PUSH 0b01000101
#define RET  0b00010001

// Function declarations

extern void cpu_load(char *filename, struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

// extern unsigned char cpu_ram_read(struct cpu *cpu, unsigned char MAR);
// extern unsigned char cpu_ram_read(struct cpu *cpu, unsigned char MAR, unsigned char MDR)

#endif
