#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char MAR)
{
    // MAR: Memory Address Register, holds the memory address we're reading or writing
    return cpu->ram[MAR];
}

void cpu_ram_write(struct cpu *cpu, unsigned char MAR, unsigned char MDR)
{
    // MDR: Memory Data Register, holds the value to write or the value just read
    cpu->ram[MAR] = MDR;
}

/* Loading the binary bytes from a .ls8 source file into a RAM Array */

void cpu_load(struct cpu *cpu, char *filename)
{

    char line[1024];
    int address = 0;

    FILE *fptr = fopen(filename, "r");

    while (fgets(line, sizeof line, fptr) != NULL)
    {

        char *endchar;
        unsigned char v = strtoul(line, &endchar, 2);

        if (line == endchar)
        {
            continue;
        }

        cpu_ram_write(cpu, address++, v);
    }
    fclose(fptr);
}

/* Arithmetic Logic Unit */

void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
    switch (op)
    {
    case ALU_CMP:
        if (cpu->reg[regA] == cpu->reg[regB])
        {
            cpu->flag |= FLAG_EQ;
        }
        else
        {
            cpu->flag &= (~FLAG_GT);
        }
        break;
    }
}

/* Run the CPU */

void cpu_run(struct cpu *cpu)
{

    int running = 1; // True until we get a HLT instruction

    while (running)
    {

        // 3. Do whatever the instruction should do according to the spec.
        // 4. Move the PC to the next instruction.

        // 1. Get the value of the current instruction (in address PC).
        // IR: Instruction Register, contains a copy of the currently executing instruction
        unsigned char IR = cpu_ram_read(cpu, cpu->pc);

        // 2. Retrieve operands (params) for current instruction
        unsigned char operandA = cpu_ram_read(cpu, cpu->pc + 1);
        unsigned char operandB = cpu_ram_read(cpu, cpu->pc + 2);

        // 3. switch() over it to decide on a course of action.
        int int_set_pc = (IR >> 4) & 1; 
        switch (IR)
        {
        case LDI:
            cpu->reg[operandA] = operandB;
            break;
        case PRN:
            printf("%d\n", cpu->reg[operandA]);
            break;
        case CMP:
            alu(cpu, ALU_CMP, operandA, operandB);
            break;
        case JMP:
            cpu->pc = cpu->reg[operandA];
            break;
        case JEQ:
            if (cpu->flag & FLAG_EQ)
            {
                cpu->pc = cpu->reg[operandA];
            }
            else {
                cpu->pc += 2; 
            }
            break;
        case JNE:
            if (!(cpu->flag & FLAG_EQ))
            {
                cpu->pc = cpu->reg[operandA];
            }
            else {
                cpu->pc += 2; 
            }
            break;
        case HLT:
            running = 0;
            break;
        default:
            printf("Unknown insruction at %02x: %02x\n", cpu->pc, IR);
            exit(2);
        }
        if (!int_set_pc) {
            cpu->pc += (IR >> 6) + 1;
        }
    }
}

/* Initialize a CPU struct */

void cpu_init(struct cpu *cpu)
{

    // Zero registers and RAM
    memset(cpu->reg, 0, sizeof cpu->reg);
    memset(cpu->ram, 0, sizeof cpu->ram);

    // Initialize the Program Counter and other Special Registers
    cpu->pc = 0;
    cpu->flag = 0; 
    // The SP points at the value at the top of the stack (most recently pushed), or at address F4 if the stack is empty.
    cpu->reg[SP] = 0xf4;
}
