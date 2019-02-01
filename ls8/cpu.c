#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(char *filename, struct cpu *cpu)
{
//char data[DATA_LEN] = {
// From print8.ls8
//  0b10000010, // LDI R0,8
//  0b00000000,
//  0b00001000,
//  0b01000111, // PRN R0
//   0b00000000,
//  0b00000001  // HLT
//  };

 // int address = 0;

 // for (int i = 0; i < DATA_LEN; i++) {
 //   cpu->ram[address++] = data[i];
 // }

 // TODO: Replace this with something less hard-coded
FILE *p;
char line[1024];
int address = ADDR_PROGRAM_ENTRY;


//Opens the sorce file
if ((p=fopen(filename,"r"))==NULL){
  fprintf(stderr, "Cannot open file %s\n",filename);
  exit(2);
}

//READs lines of file and stores them in ram
while (fgets(line,sizeof line, p ) !=NULL){
 // converts string to number
 printf("%s",line);
 char *endchar;
 unsigned char byte = strtol(line, &endchar,2);

 //Ignore lines with no numbers
 if(endchar==line){
   continue;
 }
 //store in ram
 cpu_ram_write(cpu,address++,byte);
}

}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index){
  return cpu->ram[index];
}  

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}
void cpu_push(struct cpu *cpu, unsigned char value){
  cpu->SP--;
  cpu_ram_write(cpu,value,cpu->SP);

}
unsigned char cpu_pop(struct cpu *cpu){
  unsigned char r = cpu_ram_read(cpu,cpu->SP);
  cpu->SP++;
  return r;

}
/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  
unsigned char *ram = cpu->reg;
  switch (op) {
    case ALU_MUL:

   cpu->reg[regA] = ram[regB] * ram[regA] ;

      break;

    // TODO: implement more ALU ops

    case ALU_ADD:
    ram[regA] += ram[regB];
    break;

    case ALU_INC:
    ram[regA]++;
    break;

    case ALU_DEC:
    ram[regA]--;
    break;
    case ALU_CMP:
    cpu->FL = 0;
    if (ram[regA]==ram[regB]){
      //set the equal bit 
      cpu->FL =cpu ->FL | (1<<0);
    } else if (cpu->reg[regA < cpu->reg[regB]]){
      cpu->FL = cpu->FL|(1<<1);
    }else{
      //set 
      cpu->FL = cpu->FL | (1<<2);
    }
    break;

  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    unsigned char operandA = NULL;
    unsigned char operandB = NULL;
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char instruction = cpu_ram_read(cpu, cpu->PC);
  
    
    // 2. Figure out how many operands this next instruction requires
    unsigned int num_operands = instruction >> 6    ;
    

    // 3. Get the appropriate value(s) of the operands following this instruction
   
  if(num_operands==2){
     operandA = cpu_ram_read(cpu,(cpu->PC+1)& 0xff);
     operandB = cpu_ram_read(cpu,(cpu->PC+2)& 0xff);
  }else if(num_operands==1){
     operandA = cpu_ram_read(cpu,(cpu->PC+1)& 0xff);
  }else{}
    
   printf("Trace: %02x:%02x %02x %02x\n",cpu->PC,instruction,operandA,operandB);
    // 4. switch() over it to decide on a course of action.
    switch (instruction)
    {
      case LDI:
      cpu->reg[operandA] = operandB;
      operandA = cpu->reg[operandB];
    
      break;
      case HLT:
      running = 0;
      break;
      case PRN:
      printf("%d\n", cpu -> reg[operandA]);
      break;
      case MUL:
      alu(cpu,ALU_MUL,operandA,operandB);
      break;
      case ADD:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;
      case PUSH:
      cpu_push(cpu, cpu -> reg[operandA]);
      break;
      case POP:
      cpu->reg[operandA] = cpu_pop(cpu);
      break;
      case JEQ:
      //implement If `equal` flag is set (true), jump to the address stored in the given register.
      if(cpu->FL){
        cpu->PC=cpu->reg[operandA&7];
        continue;
            
      }
      break;
      case CMP: 
      //implement
      alu(cpu,ALU_CMP,operandA,operandB);
      break; 
      case JMP:
      cpu->PC=cpu->reg[operandA&7];
      num_operands=0;
      break;
      case JNE:
      if(!cpu->FL){
        cpu->PC=cpu->reg[operandA&7];
        continue;
            
      }
      break;
      default:
      fprintf(stderr,"PC%02x: unknown instruction %02x\n",instruction);
      exit(3);
    }
    // 5. Do whatever the instruction should do according to the spec.

    // 6. Move the PC to the next instruction.
      cpu->PC += num_operands + 1;
  
  }

}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu -> PC = 0;
  cpu -> SP = ADDR_EMPTY_STACK;
 
  //    zero registers and ram
  memset(cpu->reg, 0, sizeof cpu->reg);
  memset(cpu->ram, 0, sizeof cpu->ram);



}
