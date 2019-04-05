#ifndef _CPU_H_
#define _CPU_H_

// setup basic cpu struct
struct cpu
{
    // program counter
    unsigned int PC;
    // FL = flag
    unsigned int FL;
    // R7 is reserved as the stack pointer (SP)
    unsigned int SP;
    // 8 general-purpose 8-bit numeric registers R0 - R7
    unsigned char reg[8];
    /*These registers only hold values between 0-255.
    After performing math on registers in the emulator,
    bitwise-AND the result with 0xFF (255) to keep the
    register values in that range.*/
    unsigned char ram[256];
};

// ALU ops
enum alu_op
{
    ALU_MUL,
    ALU_ADD,
    ALU_CMP,
};

// setup instructions
#define LDI 0b10000010
#define PRN 0b01000111
#define HLT 0b00000001
#define PUSH 0b01000101
#define MUL 0b10100010
#define ADD 0b10100000
#define JMP 0b01010100
//POP register - pop the value at the top of the stack into the given reg.
#define POP 0b01000110
// look into these they are new for the sprint
#define CMP 0b10100111 // Compare the value of regs
#define JEQ 0b01010101
#define JNE 0b01010110
// A lot of these commented out instructions might not be needed
#define CALL 0b01010000
#define RET 0b00010001

// function declarations
// load the instructions for the cpu
extern void cpu_load(struct cpu *cpu, char *filename);
// initialize the cpu (clear out all memory)
extern void cpu_init(struct cpu *cpu);
// run the instructions for the cpu
extern void cpu_run(struct cpu *cpu);

#endif