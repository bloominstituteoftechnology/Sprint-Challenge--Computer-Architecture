#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cpu.h"

#define DATA_LEN 6
#define SP 7
#define IS
unsigned char cpu_ram_read( struct cpu *cpu, unsigned char address){
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value){
  cpu->ram[address]=value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
//void cpu_load(struct cpu *cpu, char *argv[])
// {
//   char data[DATA_LEN] = {
//     // From print8.ls8
//     0b10000010, // LDI R0,8
//     0b00000000,
//     0b00001000,
//     0b01000111, // PRN R0
//     0b00000000,
//     0b00000001  // HLT
//   };

//   int address = 0;

//   for (int i = 0; i < DATA_LEN; i++) {
//     cpu->ram[address++] = data[i];
//   }

  // TODO: Replace this with something less hard-coded
//}

void cpu_load(struct cpu *cpu, char *argv[])
{
  FILE *fp;
  char data[1024];
  int address=0;  //unsigned char address=[0];
  fp=fopen(argv[1], "r");

  if(!fp){
    printf("Error opening file \n");
    exit(1);
  }

  while(fgets(data, sizeof(data), fp) !=NULL){
    char *endptr;
    unsigned char byte = strtol(data, &endptr, 2);
    if (endptr==data){
      continue;
    }
    cpu->ram[address++]=byte;
  }
  fclose(fp);
}

/**
 * ALU arithmetic logic unit, 
 //It represents the fundamental building block of the central processing unit (CPU) of a computer.
 *///https://study.com/academy/lesson/arithmetic-logic-unit-alu-definition-design-function.html
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      cpu->registers[regA]*=cpu->registers[regB];
      break;
      case ALU_ADD:
      cpu->registers[regA] += cpu->registers[regB];
      break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */

 unsigned char cpu_pop(struct cpu *cpu){
   unsigned char ret = cpu->ram[cpu->registers[SP]];
   cpu->registers[SP]++;
   return ret;
 }

 void cpu_push(struct cpu *cpu, unsigned char val){
   cpu->registers[SP]--;
   cpu->ram[cpu->registers[SP]]=val;
 }


void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char current =cpu_ram_read(cpu, cpu->PC);
    unsigned char param1 = cpu_ram_read(cpu, cpu->PC+1);
    unsigned char param2 = cpu_ram_read(cpu, cpu->PC+2); 
    
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    
//     interrupts = cpu.reg[IM] & cpu.reg[IS];
//     for (int i = 0; i < 8; i++) {
//   // Right shift interrupts down by i, then mask with 1 to see if that bit was set
//   int interrupt_happened = ((interrupts >> i) & 1) == 1;

//   // ...
// }
    //LDI, PRN,MUL,ADD, PUSH, CALL, POP, RET, HLT
    int pc_increment=(current>>6)+1;//7
    switch(current){
      //LDI Set the value of a register to an integer.
      case LDI:
        cpu->registers[param1] = param2;
        break;
      //PRN Print numeric value stored in the given register
      case PRN:
        printf("%d \n", cpu->registers[param1]);
        break;
      
      
      //MUL multiply the values in two registers together and store the result in registerA
      case MUL:
        alu(cpu, ALU_MUL, param1, param2);
        printf("%d\n", param1);
        break;
      //Add the value in two registers and store the result in registerA.
      case ADD:
        alu(cpu, ALU_ADD, param1, param2);
        break;
      
      //Push the value in the given register on the stack.
      //1. Decrement the `SP`.
      //2. Copy the value in the given register to the address pointed to by
       // `SP`.
      case PUSH:
        cpu_push(cpu, cpu->registers[param1]);
        break;
      
      //POP the value at the top of the stack into the given register.
      //1. Copy the value from the address pointed to by `SP` to the given register.
      //2. Increment `SP`.
      case POP:
        cpu->registers[param1] = cpu_pop(cpu);
        break;
      //CALLs a function,
//1. The address of the ***instruction*** _directly after_ `CALL` is
  // pushed onto the stack. This allows us to 
//return to where we left off when the subroutine finishes executing.
//The PC is set to the address stored in the given register.
//We jump to that location in RAM and execute the first instruction in the subroutine

      case CALL:
        cpu_push(cpu, cpu->PC+2);
        cpu->PC=cpu->registers[param1];
        pc_increment =0;
        break;

      //RET Return from subroutine.
      //Pop the value from the top of the stack and 
      //store it in the `PC`.
      case RET:
        cpu->PC = cpu_pop(cpu);
        pc_increment=0;
        break;

      case ST:
        cpu->registers[param1] =param2;
        pc_increment++ ;
        break;

        case CMP:
        if (cpu->registers[param1] == cpu->registers[param2]) {
          cpu->FL = 1;
        } else if (cpu->registers[param1] > cpu->registers[param2]) {
          cpu->FL = 2;
        } else {
          cpu->FL = 4;
        } 
        break;
        
        case JMP:
        cpu->PC = cpu->registers[param1];
        pc_increment=0;
        break;

        case JEQ:
        if(cpu->FL == 1) {
          cpu->PC= cpu->registers[param1];
          pc_increment=0;
        }
        break;

        case JNE:
        if(cpu->FL !=1){
          cpu->PC = cpu->registers[param1];
          pc_increment =0;
        }
        break;
        //HLT halt the CPU (and exit the emulator).
      case HLT:
        running =0;
        break;


      default:
      printf("hello, you are not done with this func yet\n");
      exit(1);
    }
    cpu->PC +=pc_increment;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)//takes a pointer to a struct cpu 
{
  // TODO: Initialize the PC and other special registers
  cpu->PC =0;//at first, the PC, registers and RAM should be cleared 0
  cpu->registers[SP] =0xF4;//The value of the key pressed is stored in address `0xF4`
  cpu->FL = 0;
  memset(cpu->ram, 0, sizeof cpu->ram);//clear registers and RAM
  memset(cpu->registers, 0, sizeof cpu->registers);
}
