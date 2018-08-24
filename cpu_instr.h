#ifndef _CPU_INSTRUCTIONS_H_
#define _CPU_INSTRUCTIONS_H_

// Instructions
#define ADD 0b10100000
#define SUB 0b10100001
#define MUL 0b10100010
#define DIV 0b10100011
#define MOD 0b10100100
#define INC 0b01100101
#define DEC 0b01100110
#define CMP 0b10100111
#define AND 0b10101000
#define NOT 0b01101001
#define OR 0b10101010
#define XOR 0b10101011
#define SHL 0b10101100
#define SHR 0b10101101
#define CALL 0b01010000
#define RET 0b00010001
#define INT 0b01010010
#define IRET 0b00010011
#define JMP 0b01010100
#define JEQ 0b01010101
#define JNE 0b01010110
#define JGT 0b01010111
#define JLT 0b01011000
#define JLE 0b01011001
#define JGE 0b01011010
#define NOP 0b00000000
#define HLT 0b00000001
#define LDI 0b10000010
#define LD 0b10000011
#define ST 0b10000100
#define PUSH 0b01000101
#define POP 0b01000110
#define PRN 0b01000111
#define PRA 0b01001000

typedef void (*handler)(struct cpu *, unsigned char, unsigned char);

// Function declarations
extern void load_cpu_instructions(handler *bt);
extern void handle_ADD(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_AND(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_CALL(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_CMP(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_DEC(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_DIV(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_INC(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_INT(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_IRET(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_JEQ(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_JGE(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_JGT(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_JLE(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_JLT(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_JMP(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_JNE(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_LD(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_LDI(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_MOD(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_MUL(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_NOP(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_NOT(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_OR(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_POP(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_PRA(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_PRN(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_PUSH(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_RET(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_SHL(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_SHR(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_ST(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_SUB(struct cpu *cpu, unsigned char opA, unsigned char opB);
extern void handle_XOR(struct cpu *cpu, unsigned char opA, unsigned char opB);

#endif