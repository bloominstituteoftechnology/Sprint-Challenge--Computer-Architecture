#ifndef _CPU_H_
#define _CPU_H_


struct cpu
{
  // TODO
  int SP;
  unsigned char PC; 
  unsigned char FL; 

  int R[8];             
  unsigned char ram[256]; 
};

// ALU operations
enum alu_op
{
  ALU_MUL,
  ALU_ADD
  // Add more here
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI 0b10000010
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *path);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
