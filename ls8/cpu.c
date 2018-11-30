#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DATA_LEN 6


unsigned char cpu_ram_read(struct cpu *cpu)
{
  cpu->MDR =  cpu->ram[cpu->MAR];
  return cpu->MDR;
}

void cpu_ram_write(struct cpu *cpu)
{ 
    if(cpu->MAR < 0xF4){
      cpu->ram[cpu->MAR] = cpu->MDR;
    }   
}

void dec(struct cpu *cpu, unsigned char regA){
  cpu->registers[regA] -= 1; 
}
void incr(struct cpu *cpu, unsigned char regA){
  cpu->registers[regA] += 1; 
}


/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *fileName)
{
  FILE *fp;
  fp = fopen(fileName, "r"); // read mode
 
   if (fp == NULL)
   {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }
  // char data = calloc(8, sizeof(char));
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };

  int address = 0;
  
  char s[1024];

  while(!feof(fp))
{
  fgets(s, sizeof(s), fp); 
      char *split=" ";

    unsigned char binaryencoded = strtol(s, &split, 2);;
    if (split == s) {
      continue;
    }

    cpu->ram[address++] = binaryencoded;
}
  fclose(fp);
}

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
      break;
    case ALU_DIV:
      cpu->registers[regA] = cpu->registers[regA] / cpu->registers[regB];
      break;
    case ALU_SUB:
      cpu->registers[regA] = cpu->registers[regA] - cpu->registers[regB];
      break;
    default:
      printf("Something broke in the alu function");
    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{      

  int running = 1; // True until we get a HLT instruction
  
  while (running) {
    int inc; 
    unsigned char operandA;
    unsigned char operandB;

 
  //   // TODO
  //   // 1. Get the value of the current instruction (in address PC).


      cpu->MAR = cpu->PC;
      cpu->IR = cpu_ram_read(cpu);
      if(cpu->IR >= 128){
        inc =3;

        cpu->MAR = cpu->PC + 1;
        operandA = cpu_ram_read(cpu);

        cpu->MAR = cpu->PC + 2;
        operandB = cpu_ram_read(cpu); 
      }
      else if ((cpu->IR >> 4) & 1){
          cpu->MAR = cpu->PC + 1;
          operandA = cpu_ram_read(cpu);
          inc = 0;

      }
      else if(cpu->IR >= 64){
        inc=2;

        cpu->MAR = cpu->PC + 1;
        operandA = cpu_ram_read(cpu);
      }
      else{
        inc = 1;
      }
  //   // 2. switch() over it to decide on a course of action.
  // printf("!%i\n",cpu->IR);
    switch (cpu->IR){
    case HLT:
      running = 0;
      break;
    case LDI:
      cpu->registers[operandA] = operandB;
      break;
    case PRN:
      printf("%d\n",cpu->registers[operandA]);
      break;
    case MUL:
      alu(cpu,ALU_MUL,operandA,operandB);
      break;
    case ADD:
      alu(cpu,ALU_ADD,operandA,operandB);
      break;
    case DIV:
      alu(cpu,ALU_DIV,operandA,operandB);
      break;
    case SUB:
      alu(cpu,ALU_SUB,operandA,operandB);
      break;
    case PUSH:
      dec(cpu,7);
      cpu->MAR = cpu->registers[7];
      cpu->MDR = cpu->registers[operandA];
      cpu_ram_write(cpu);
      break;
    case POP:
      cpu->MAR = cpu->registers[7];
      cpu_ram_read(cpu);
      cpu->registers[operandA]= cpu->MDR;
      incr(cpu,7);
      break;
    case AND:
      cpu->registers[operandA] = cpu->registers[operandA] & cpu->registers[operandB];
      break;
    case OR:
      cpu->registers[operandA] = cpu->registers[operandA] | cpu->registers[operandB];
      break;
    case XOR:
      cpu->registers[operandA] = cpu->registers[operandA] ^ cpu->registers[operandB];
      break;
    case NOT:
      cpu->registers[operandA] = ~cpu->registers[operandA];
      break;
    case SHL:
      cpu->registers[operandA] = cpu->registers[operandA] << cpu->registers[operandB];
      break;
    case SHR:
      cpu->registers[operandA] = cpu->registers[operandA] >> cpu->registers[operandB];
      break;
    case RET:
      cpu->MAR = cpu->registers[7];
      cpu_ram_read(cpu);
      cpu->PC = cpu->MDR;
      incr(cpu,7);
      break;
    case CALL:
      dec(cpu,7);
      cpu->MAR = cpu->registers[7];
      cpu->MDR = cpu->PC+2;
      cpu_ram_write(cpu);
      cpu->PC = cpu->registers[operandA];
      break;
    case ST:
      cpu->MAR = cpu->registers[operandA];
      cpu->MDR = cpu->registers[operandB];
      cpu_ram_write(cpu);
      break;
    case CMP:
    
      if(cpu->registers[operandA] > cpu->registers[operandB]){
        
        cpu->FL = 2;
      }
      else if(cpu->registers[operandA] == cpu->registers[operandB]){
        cpu->FL = 1;
      }
      else{
        cpu->FL = 4;
      }
      break;
    case IRET:
      
      for(int i = 6; i >= 0; i--)
      {
        cpu->MAR = cpu->registers[7];
        cpu_ram_read(cpu);
        cpu->registers[i] = cpu->MDR;
        incr(cpu,7);
        
      }
      cpu->MAR = cpu->registers[7];
      cpu_ram_read(cpu);
      cpu->FL = cpu->MDR;
      incr(cpu,7);
      cpu->MAR = cpu->registers[7];
      cpu_ram_read(cpu);
      cpu->PC = cpu->MDR;
      incr(cpu,7);
      break;
    case JEQ:
      if(cpu->FL == 1){
        cpu->PC = cpu->registers[operandA];
      }
      else{
        inc +=2;
      }
      break;
    case JGE:
      if(cpu->FL == 1 || cpu->FL ==2){
        cpu->PC = cpu->registers[operandA];
      }else{
        inc +=2;
      }
      break;
    case JGT:
      if( cpu->FL ==2){
        cpu->PC = cpu->registers[operandA];
      }else{
        inc +=2;
      }
      break;
    case JLE:
        if(cpu->FL == 1 || cpu->FL ==4){
          cpu->PC = cpu->registers[operandA];
        }else{
        inc +=2;
      }
        break;
    case JLT:
        if(cpu->FL ==4){
          cpu->PC = cpu->registers[operandA];
        }else{
        inc +=2;
      }
        break;
      case JNE:
        if(cpu->FL !=1){
          cpu->PC = cpu->registers[operandA];
        }else{
        inc +=2;
      }
        break;
      case JMP:
          cpu->PC = cpu->registers[operandA];
        break;
      case LD:
         cpu->MAR = cpu->registers[operandB];
        cpu_ram_read(cpu);
        cpu->registers[operandA] = cpu->MDR;
        break;
      case NOP:
        break;
      case PRA:
        printf("%c",cpu->registers[operandA]);
        break;
      case INC:
        incr(cpu,operandA);
        break;
      case DEC:
        dec(cpu,operandA);
        break;
    default:
      printf("Something went wrong\n");

    }
        cpu->PC +=inc;

  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers

  cpu->PC = 0;
  cpu->FL = 0;
  cpu->registers =calloc(8,sizeof(unsigned char));
  cpu->ram = calloc(256,sizeof(unsigned char));
  // TODO: Zero registers and RAM
  
  cpu->registers[7] = 0xF4;
}



