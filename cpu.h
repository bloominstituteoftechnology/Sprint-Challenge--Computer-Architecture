#ifndef _CPU_H_
#define _CPU_H_
#define SP 7  //The SP points at the value at the top of the stack (most recently pushed)

// Holds all information about the CPU
struct cpu {
  
  unsigned int PC;// PC (program counter)
  
  unsigned char reg[8]; // registers (array)
    //holds the data that CPU is currently processing | PC / IR / MAR / MDR
    //we'll execute code that stores the value 8 in a register
 
  unsigned char ram[256];  // ram (array)
    //holds program instruction and data that the program requires for execution 
    //The LS-8 has 8-bit addressing, so can address 256 bytes of RAM total.
  
  // ------- SPRINT ---------
  unsigned int FL; //The flags register `FL` 
  //holds the current flags status. These flags can change based on the operands given to the `CMP` opcode.
  
}; 

// ALU operations
enum alu_op {
	ALU_MUL,
  ALU_ADD,
	// ------- SPRINT ---------
  ALU_CMP
};

// Instructions
// These can be used in cpu_run() as cases 
// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define HLT  0b00000001
#define PRN  0b01000111
#define MUL  0b10100010
#define PUSH 0b01000101
#define POP  0b01000110
#define CALL 0b01010000
#define RET  0b00010001
#define ADD  0b10100000

// ------- SPRINT ---------
#define CMP  0b10100111
#define JMP  0b01010100
#define JEQ  0b01010101
#define JNE  0b01010110


// Function declarations

extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);
extern unsigned char cpu_ram_read(struct cpu *cpu,unsigned char mar);
extern void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr);

#endif
