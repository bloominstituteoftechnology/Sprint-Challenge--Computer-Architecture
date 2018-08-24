#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"

//fing cpu run

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

//fing cpu write

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char val)
{
  cpu->ram[address] = val;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

//zoom ended
void cpu_load(struct cpu *cpu, char *argv){
  char* c;
  char buff[255];
  char buffer[255];
  FILE *fp = fopen(argv, "r");
  int address = 0;
  while(1) {
      c = fgets(buff, 9, (FILE*) fp);
      if( feof(fp) ) { 
         break ;
      }
      if((buff[0] == '0' || buff[0] == '1') && (buff[1] == '0' || buff[1] == '1') ){
        //printf("new line :%s\n", c);
        //printf("li :%li\n", strtoul(c, NULL, 2));
        cpu->ram[address++] = strtoul(c, NULL, 2);
      }
   }
   fclose(fp);
}
//boooooooooo
/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB];
      break;
    case ALU_ADD:
      cpu->registers[regA] = cpu->registers[regA] + cpu->registers[regB];
    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char IR;
  unsigned char val;
  int addrLDI;
  int addrPRN;

//what iz life?
  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    //printf("cpu->PC is: %i\n", cpu->PC);
    IR = cpu_ram_read(cpu, cpu->PC);
    //printf("IR is: %d\n", IR);
    // IR 8 bits AA B C DDDD (AABCDDDD)
    unsigned char foo = cpu_ram_read(cpu, cpu->PC+1);
    unsigned char bar = cpu_ram_read(cpu, cpu->PC+2);
    switch (IR) {
      case LDI: // LDI
        // addrLDI = cpu_ram_read(cpu, cpu->PC + 1);
        // val = cpu_ram_read(cpu, cpu->PC + 2);
        // printf("val is: %i\n, addrLDI is: %i\n", val, addrLDI);
        cpu->registers[foo] = bar;
        // printf("cpu->registers[addrLDI] is: %i\n", cpu->registers[addrLDI]);
        cpu->PC += 3;
        //printf("LDI %d\n", IR);
        break;
      case MUL:
        //printf("IR is: %d\n", IR);
        alu(cpu, ALU_MUL, foo, bar);
        cpu->PC += 3;
        break;
      case PRN: // PRN
        //printf("%i\n", foo);
        printf("%i\n", cpu->registers[foo]);
        cpu->PC += 2;
        break;
      case HLT: //HLT
        //printf("boooo\n");
        running = 0;
        break;
      case PUS: 
        //printf("PUS %d\n", foo);
        cpu->SP -= 1;
        cpu_ram_write(cpu, cpu->SP, cpu->registers[foo]);
        cpu->PC += 2;
        //printf("boooo\n");
        //printf("cpu->ram is: %i\n", cpu_ram_read(cpu, cpu->PC+1));
        break;
      case POP: 
        //printf("POP %d\n", foo);
        //printf("foo: %d\n", cpu_ram_read(cpu, cpu->SP));
        if(cpu->SP == 244) {
          fprintf(stderr, "no can do brav!\n");
        }
        cpu->registers[foo] = cpu_ram_read(cpu, cpu->SP++);
        cpu->PC += 2;
        break;
      case ADD:
        alu(cpu, ALU_ADD, foo, bar);
        cpu->PC += 3;
        break;
      case CAL:
         //printf("CAL is: %d\n", IR);
        //printf("CAL1 %d\n", cpu_ram_read(cpu, cpu->PC + 1));
        cpu->SP -= 1; 
        cpu_ram_write(cpu, cpu->SP, (cpu->PC + 2));
        //cpu_ram_write(cpu, cpu->PC, cpu_ram_read(cpu, cpu->PC + 1));
        cpu->PC = cpu->registers[foo];
        //printf("CAL %d\n", cpu->PC);
        break;
      case RET:
       //printf("ret is: %d\n", IR);
        if(cpu->SP == 244) {
          fprintf(stderr, "no can do brav!\n");
        } 
        cpu->PC = cpu_ram_read(cpu, cpu->SP++);
        break;
      case CMP:
        // printf("cmp is: %d\n", IR);
        if(cpu->registers[foo] == cpu->registers[bar]){
          //printf("equal, foo=%d, bar= %d\n",cpu->registers[foo], cpu->registers[bar]);
          cpu->FL = 0B00000001;
        }
        else if(cpu->registers[foo] > cpu->registers[bar]){
          //printf("more foo=%d, bar= %d\n",cpu->registers[foo], cpu->registers[bar]);
          cpu->FL = 0B00000010;
        }
        
        else if(cpu->registers[foo] < cpu->registers[bar])
        {
          //printf(" less foo=%d, bar= %d\n",cpu->registers[foo], cpu->registers[bar]);
          cpu->FL = 0B00000100;
          //printf("fl %d\n", cpu->FL);
        }
        else{
          printf("something's gone terribly wrong!");
        }
        cpu->PC +=3;
        break;
      case JMP:
        // printf("jmp is: %d\n", IR);
        cpu->PC = cpu->registers[foo];
      case JEQ:
        // printf("JEQ is: %d\n", IR);
        // printf("FL is: %d\n ", cpu->FL);
        if(cpu->FL == 1){
          //printf("boo\n");
          cpu->PC = cpu->registers[foo];
        }
        else{
          //printf("boo2\n");
          cpu->PC += 2;
        }
        break;
      case JNE:
        // printf("JNE is: %d\n", IR);
        // printf("FL is: %d\n ", cpu->FL);
        if(cpu->FL == 0B00000000 || cpu->FL == 0B00000100 || cpu->FL == 0B00000010){
          cpu->PC = cpu->registers[foo];
        }
        else{
          cpu->PC += 2;
        }
        break;
    }

  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  cpu->SP = 244;
  // TODO: Zero registers and RAM
  cpu->registers = (unsigned char *) calloc(8, sizeof(unsigned char));
  cpu->ram = (unsigned char *) calloc(256, sizeof(unsigned char));
}
