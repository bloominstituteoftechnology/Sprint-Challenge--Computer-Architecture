#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  unsigned char pc;
  // registers (array)
  unsigned char reg[8];
  // ram (array)
  unsigned char ram[256];
};

#define SP 7

// ALU operations
enum alu_op {
  ALU_MUL,
  ALU_ADD,
};

// Memory Location
#define ADDR_PROGRAM_ENTRY 0x00
#define ADDR_EMPTY_STACK 0xF4

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI 0b10000010
// TODO: more instructions here. These can be used in cpu_run().
#define HLT 0b10000001
#define PRN 0b01000111
#define MUL 0b10100010
#define ADD 0b10100000

// ADD Push and Pop instructions
#define PUSH 0b01000101
#define POP 0b01000110

// ADD CALL and RET
#define CALL 0b01010000
#define RET 0b00010001

// Function declarations

extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

extern unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address);
extern void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value);

extern void cpu_push(struct cpu *cpu, unsigned char val);
extern unsigned char cpu_pop(struct cpu *cpu);

#endif
