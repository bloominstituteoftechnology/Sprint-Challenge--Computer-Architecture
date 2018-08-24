#ifndef _CPU_H_
#define _CPU_H_

#define E_FLAG 0x00
#define MAX_ADDRESS 0xFF
#define SP 7
#define IS 6
#define IM 5

// Holds all information about the CPU
enum alu_op {
    ALU_MUL,
    ALU_ADD,
    ALU_DIV,
    ALU_SUB,
    ALU_MOD,
};


struct CPU {

    unsigned char PC;
    unsigned char FL;
    unsigned char reg[8];
    unsigned char ram[MAX_ADDRESS + 1];
};

#define LDI  0b10000010 
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
#define POP 0b01000110
#define PRA 0b01001000
#define PRN 0b01000111
#define PUSH 0b01000101
#define RET 0b00010001
#define SHL 0b10101100
#define SHR 0b10101101
#define ST 0b10000100
#define SUB 0b10100001
#define XOR 0b10101011


extern void cpu_load(char *arg, struct CPU *cpu);
extern void cpu_init(struct CPU *cpu);
extern void cpu_run(struct CPU *cpu);
extern unsigned char cpu_ram_read(struct CPU *cpu, unsigned char index);
extern void cpu_ram_write(struct CPU *cpu, unsigned char index, unsigned char value);

#endif
