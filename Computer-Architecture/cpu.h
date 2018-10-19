#ifndef _CPU_H_
#define _CPU_H_

#define MAX_ADDR 0xff
#define CPU_FLAG 0x00

// Holds all information about the CPU
typedef struct cpu {
  // TODO
  int fl; // flag 
  // PC
  unsigned char pc; // the type for a single unsigned byte in C; register the pc
  // registers (array)
  unsigned char reg[8]; // create an array with 8 slots in it; we have to specify unsigned with a char otherwise the compiler will be the one to use which one to use
  // ram (array)
  // unsigned char ram[256]; // 256 bytes for the RAM; holds the rest of the memory
  unsigned ram[MAX_ADDR +1];  // set ram to MAX_ADDR
} CPU;

//Other general purpose register names
#define SP 7  // initialize special values for register
#define IS 6
#define IM 5

// ALU operations // Math operations 
enum alu_op { // enum stands for enumerated type
  ALU_MUL,  // MUL stands for multiply; assigned an integer value and defaults to starting at 0
  ALU_ADD,
};

// Memory locations.
#define START_OF_STACK_ADDR 0xf4  // Where Stack Pointer (SP) is on an empty stack.
#define START_OF_PROGRAM_ADDR 0x00 // Where programs start getting loaded.
#define INTERRUPT_MASK 0X00   // Others
#define INTERRUPTS 0X00

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

// for implementing instruction handlers
#define LDI 0b10000010  // load
#define PRN 0b01000111  // print; 
#define HLT 0b00000001  // halt; start the program
#define MUL 0b10100010  // multiply
#define ADD 0b10100000  // add
#define PRA 0b01001000
#define PUSH 0b01000101 // push 
#define POP  0b01000110 // pop  
#define CALL 0b01010000 
#define RET  0b00010001
#define INT  0b01010010 
#define CMP  0b10100111
#define JMP  0b01010100
#define JEQ  0b01010101
#define JNE  0b01010110

// TODO: more instructions here. These can be used in cpu_run().

// Function declarations
// extern void cpu_load(struct cpu *cpu);

// extern: "declare without defining"
extern void cpu_load(struct cpu *cpu, char *filename);  // predefining and instructing the compiler to expect these functions
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

extern unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address);  // declare function for reading from ram
extern void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value); // declare function for writing to ram

#endif
