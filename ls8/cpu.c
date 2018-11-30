// header file to make access to CPU/ALU functions:
#include "cpu.h"
// include other stuff:
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */


// add args to cpuload to use FILE I/O:
void cpu_load(struct cpu *cpu, char *argv[])
{
        FILE *fp;
        // make data into a big array
        char data[1024];
        //initialize memory address:
        unsigned char address = 0;
        //open file as read only to input instructions the examples are the second elelmet of the argv array
        fp = fopen(argv[1], "r");
        //check if the file doesn't exist.
        if (fp == NULL) {
                perror("file can't be opened");
                exit(1);
        }

        while(fgets(data, sizeof(data), fp) != NULL) {
                //convert data to long unsigned integer
                unsigned char byte = strtoul(data, NULL, 2);

                if(data == NULL) {
                        continue;
                }
                cpu->ram[address++] = byte;
        }
        fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
        unsigned char *regist = cpu->regist;
        switch (op) {
        case ALU_MUL:
                // notice this overwrites whatevr registeer regA is as the product:
                regist[regA] *= regist[regB];
                break;

        // TODO: implement more ALU ops
        case ALU_ADD:
                regist[regA] += regist[regB];
                break;

        case ALU_SUB:
                regist[regA] -= regist[regB];
                break;

        case ALU_DIV:
                regist[regA] /= regist[regB];
                break;

        case ALU_MOD:
                regist[regA] = regist[regA]%regist[regB];
                break;
        }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
        //declare struct components:
        //unsigned char *ram = cpu->ram;
        unsigned char *regist = cpu->regist;
        unsigned char PC = cpu->PC;

        int running = 1; // True until we get a HLT instruction
        while (running) {
                // TODO
                // 1. Get the value of the current instruction (in address PC).
                // Instruction register set to RAM address at program counter
                unsigned char IR = cpu_ram_read(cpu, PC);
                // first operand is the next intsuction in mem
                unsigned char operandA = cpu_ram_read(cpu, (PC + 1));
                unsigned char operandB = cpu_ram_read(cpu, (PC + 2));
                // shift for moving PC along must be past the reserved registers bitwise right shift by 6 bits from the current PC. initialize past 7.
                int shift = ((IR >> 6)) + 1;


                // 2. switch() over it to decide on a course of action. use case based on instruction inpuit through args
                // 3. Do whatever the instruction should do according to the spec. in the middle of each case


                switch (IR) {
                case HLT:
                        running = 0;
                        shift = 0;
                        break;

                case LDI:
                        regist[operandA] = operandB;
                        // PC += shift;
                        break;

                case PRN:
                        printf("%d \n", regist[operandA]);
                        // PC += shift;
                        break;

                case MUL:
                        alu(cpu, ALU_MUL, operandA, operandB);
                        // PC += shift;
                        break;

                case ADD:
                        alu(cpu, ALU_ADD, operandA, operandB);
                        // PC += shift;
                        break;

                case SUB:
                        alu(cpu, ALU_SUB, operandA, operandB);
                        // PC += shift;
                        break;

                case DIV:
                        alu(cpu, ALU_DIV, operandA, operandB);
                        // PC += shift;
                        break;

                case MOD:
                        alu(cpu, ALU_MOD, operandA, operandB);
                        // PC += shift;
                        break;

                case POP:
                        regist[operandA] = cpu_ram_read(cpu, regist[SP]++);
                        // PC += shift;
                        break;

                case PUSH:
                        cpu_ram_write(cpu, --regist[SP], regist[operandA]);
                        // PC += shift;
                        break;

                case CALL:
                        regist[SP] = regist[SP-1];
                        // store the next instruction into the ram index of the current stack.
                        cpu_ram_write(cpu, regist[SP], PC + 2);
                        PC = regist[operandA];
                        shift = 0;
                        break;

                case RET:
                        PC = cpu_ram_read(cpu, regist[SP]++);
                        shift = 0;
                        break;

                default:
                        printf("incorret instuction %02x at %02x\n", PC, IR );
                        exit(2);
                }
                // 4. Move the PC to the next instruction. done with incemeneting shift var
                // set PC
                PC += shift;
        }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
        // TODO: Initialize the PC and other special registers
        cpu->PC = 0x00;
        cpu->regist[SP] = 0xF4;
        //cpu->ram = calloc(256, sizeof(unsigned char));
        // TODO: Zero registers and RAM
        memset(cpu->regist, 0, sizeof(cpu->regist));
        memset(cpu->ram, 0, sizeof(cpu->ram));
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index){
        // unsigned because we need a return for the read
        return cpu->ram[index];
}
void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char value){
        // assign value to array element at given index.
        cpu->ram[index] = value;
}
