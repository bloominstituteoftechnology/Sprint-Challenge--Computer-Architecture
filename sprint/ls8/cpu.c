#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include "cpu.h"

#define DATA_LEN 6
/*In `cpu.c`, add functions `cpu_ram_read()` and `cpu_ram_write()` that access the
RAM inside the `struct cpu`.*/
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index)
{
 //   printf(" Returning   index %u value %u \n", index, cpu->ram[index]);
      //To READ RAM which is an array of memory addresses so to access memory we need index value
      return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char value) 
{
      //To WRITE IN RAM 
         /* - need to access ram of particular index where to write
            - and value -- data to be written */
  //printf(" Storing  index %u value %u \n", index, value);
      cpu->ram[index] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(char *filename, struct cpu *cpu)
{
      /******  DAY-I ********/
      // char data[DATA_LEN] = {
      //   // From print8.ls8
      //   0b10000010, // LDI R0,8
      //   0b00000000,
      //   0b00001000,
      //   0b01000111, // PRN R0
      //   0b00000000,
      //   0b00000001  // HLT
      // };

      // int address = 0;

      // for (int i = 0; i < DATA_LEN; i++) {
      //   cpu->ram[address++] = data[i];
      // }

      /******** DAY-II to process file code ********/
      // TODO: Replace this with something less hard-coded
      printf("IN CPU_LOAD... %s\n", filename);
      FILE *file_pointer;
      char line[1024]; //buffer
      unsigned char line_number = 0;
      char *pointer;

      file_pointer = fopen(filename, "r");

      if(file_pointer == NULL) {
          fprintf(stderr, "Failed to open %s\n", filename);
          exit(0);
      }
      else {
          while(fgets(line, sizeof(line), file_pointer) != NULL) {
                //You'll have to convert the binary strings to integer values to store in RAM. 
                //The built-in `strtoul()` library function might help you here
                //The strtoul() function converts the initial part of the string in nptr to an unsigned long int
                //strtoul(const char *nptr, char **endptr, register int base)
                unsigned char value = strtol(line, &pointer, 2);
                //write value in RAM
                cpu_ram_write(cpu, line_number++, value);
          }
          fclose(file_pointer);
      }
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
            // TODO
            cpu->reg[regA] *= cpu->reg[regB];
            break;

            // TODO: implement more ALU ops
    case ALU_ADD:
            cpu->reg[regA] += cpu->reg[regB];
            break;
    
    case ALU_SUB:
            cpu->reg[regA] -= cpu->reg[regB];
            break;

    case ALU_DIV:
            cpu->reg[regA] /= cpu->reg[regB];
            break;

    default:
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
        // TODO
        // 1. Get the value of the current instruction (in address PC).
        unsigned char IR = cpu_ram_read(cpu, cpu->PC); //Instruction Register
        //unsigned char IR = cpu_ram_read(cpu, cpu->PC + 1); //for CALL.......
        unsigned char operandA = -1;
        unsigned char operandB = -1;
        // 2. Figure out how many operands this next instruction requires
        // 3. Get the appropriate value(s) of the operands following this instruction
        if( IR != RET) {
            operandA =cpu_ram_read(cpu, (cpu->PC + 1));
        }
        if( IR != CALL &&  IR != RET){
            operandB = cpu_ram_read(cpu, (cpu->PC + 2));
        }
        // printf("command %u operandA %u operandB %u\n", IR, operandA, operandB);
        // 4. switch() over it to decide on a course of action.
        switch (IR)
        {
            case ADD:
                    alu(cpu, ALU_ADD, operandA, operandB);
                    cpu->PC +=3;
                    break;
                
            case MUL:
                    alu(cpu, ALU_MUL, operandA, operandB);
                    cpu->PC +=3;
                    break;

            case POP:
                    cpu->reg[operandA] = cpu_ram_read(cpu, cpu->SP++);
                    cpu->PC +=2;
                    break;

            case PUSH:
                    cpu->SP -= 1;
                    cpu_ram_write(cpu, cpu->SP, cpu->reg[operandA]);    
                    cpu->PC +=2;
                    break;
            
            case CALL: //Calls a subroutine (function) at the address stored in the register.
                    //printf("Current stack loc %u current program counter %u  operatndA %u  %u \n", cpu->SP, cpu->PC, operandA ,cpu->reg[operandA]);
                    cpu_ram_write(cpu,  --cpu->SP,  cpu->PC += 2);
                    cpu->PC = cpu->reg[operandA];
                    break;

            case RET: //Return from subroutine.
                      //Pop the value from the top of the stack and store it in the `PC`.
                    cpu->PC = cpu_ram_read(cpu, cpu->SP);
                    break;

            case DIV:
                    alu(cpu, ALU_DIV, operandA, operandB);
                    cpu->PC += 3;
                    break;
                
            case SUB:
                    alu(cpu, ALU_SUB, operandA, operandB);
                    cpu->PC += 3;
                    break;

            case LDI:
                    cpu->reg[operandA] = operandB;
                    cpu->PC += 3;
                    break;
          
            case PRN:
                    printf("%d\n", cpu->reg[operandA]);
                    cpu->PC += 2;
                    break;
                
            case HLT:
                    running = 0;
                    break;
                
            default:
                    printf("UNEXPECTED INSTRUCTION 0x%2X at 0x%2X\n", IR, cpu->PC);
                    break;
        }
        // 5. Do whatever the instruction should do according to the spec.
        // 6. Move the PC to the next instruction.
    }
}

/**
 * Initialize a CPU struct
 */
/*The `cpu_init()` function takes a pointer to a `struct cpu` and initializes it
as necessary. At first, the PC, registers, and RAM should be cleared to zero.
(`memset()` might help you clear registers and RAM.)

Later on, you might do further initialization here, e.g. setting the initial
value of the stack pointer.*/

void cpu_init(struct cpu *cpu)
{
      // TODO: Initialize the PC and other special registers
      cpu->PC = 0;
      //The C library function void *memset(void *str, int c, size_t n)
      memset(cpu->reg, 0, sizeof(cpu->reg));
      memset(cpu->ram, 0, sizeof(cpu->ram));
      //stack;
      cpu->SP = 254;
}
