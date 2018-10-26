#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char MAR)
{
    return cpu->ram[MAR];
}

void cpu_ram_write(struct cpu *cpu, unsigned char MAR, unsigned char MDR)
{
    cpu->ram[MAR] = MDR;
}

void cpu_load(struct cpu *cpu, char *filename)
{
    FILE *fp = fopen(filename, "r");
    char line[1024];
    int address = 0;

    if (fp == NULL)
    {
        fprintf(stderr, "Failed to open %s\n", filename);
        exit(1);
    }
    while (fgets(line, sizeof line, fp) != NULL)
    {
        char *endchar;
        unsigned char value = strtoul(line, &endchar, 2);
        if (endchar == line)
        {
            continue;
        }
        cpu->ram[address++] = value;
    }
    fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
    // unsigned char valA = cpu->reg[regA];
    // unsigned char valB = cpu->reg[regB];
    // unsigned char *reg = cpu->reg;
    switch (op)
    {
    case ALU_MUL:
        // TODO
        // cpu->reg[regA] *= valB;
        cpu->reg[regA] *= cpu->reg[regB];
        break;
    case ALU_ADD:
        cpu->reg[regA] += cpu->reg[regB];
        break;
    case ALU_CMP:
        if (cpu->reg[regA] == cpu->reg[regB])
        {
            cpu->fl = 0b00000001;
        }
        else
        {
            cpu->fl = 0b00000000;
        }
        break;
    }
}
void trace(struct cpu *cpu, unsigned char IR, unsigned char operandA, unsigned char operandB)
{
    printf("TRACE: %02X | %02X %02X %02X |", cpu->pc, IR, operandA, operandB);
    for (int i = 0; i < 8; i++)
    {
        printf(" %02X", cpu->reg[i]);
    }
    printf("\n");
}
/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
    int running = 1; // True until we get a HLT instruction
    unsigned char IR, operandA, operandB;
    while (running)
    {
        // TODO
        // 1. Get the value of the current instruction (in address PC).
        // 2. switch() over it to decide on a course of action.
        // 3. Do whatever the instruction should do according to the spec.
        // 4. Move the PC to the next instruction.

        IR = cpu_ram_read(cpu, cpu->pc);
        operandA = cpu_ram_read(cpu, (cpu->pc + 1) & 0xff);
        operandB = cpu_ram_read(cpu, (cpu->pc + 2) & 0xff);
        int add_to_pc = (IR >> 6) + 1;
        // printf("Trace: %02X %02X %02X %02X ", cpu->pc, IR, operandA, operandB);
        trace(cpu, IR, operandA, operandB);

        switch (IR)
        {
        case LDI:
            cpu->reg[operandA] = operandB;
            break;
        case PRN:
            printf("%d\n", cpu->reg[operandA & 7]);
            break;
        case MUL:
            alu(cpu, ALU_MUL, operandA, operandB);
            break;
        case PUSH:
            cpu->reg[7]--;
            cpu_ram_write(cpu, cpu->reg[7], cpu->reg[operandA]);
            // cpu->ram[cpu->reg[SP]] = cpu->reg[operandA]; // same as above
            break;
        case POP:
            cpu->reg[operandA & 7] = cpu_ram_read(cpu, cpu->reg[7]);
            cpu->reg[7]++;
            break;
        case JMP:
            cpu->pc = cpu->reg[operandA & 7];
            add_to_pc = 0; // restrict moving to next instruction
            break;
        case CMP:
            alu(cpu, ALU_CMP, operandA, operandB); // alu has all this hardware to do it
            break;
        case ADD:
            alu(cpu, ALU_ADD, operandA, operandB); // alu has all this hardware to do it
            break;
        case JEQ:
            if (cpu->fl == 0b00000001)
            {
                cpu->pc = cpu->reg[operandA & 7];
                add_to_pc = 0;
            }
            // else
            // {
            //     cpu->pc += 2;
            // }
            break;
        case JNE:
            if (cpu->fl == 0b00000000)
            {
                cpu->pc = cpu->reg[operandA & 7];
                add_to_pc = 0;
            }
            // else
            // {
            //     cpu->pc += 2;
            // }
            break;
        case CALL:
            cpu->reg[7]--;
            cpu_ram_write(cpu, cpu->reg[7], cpu->pc + 2);
            cpu->pc = cpu->reg[operandA];
            add_to_pc = 0;
            break;
        case RET:
            cpu->pc = cpu_ram_read(cpu, cpu->reg[7]);
            cpu->reg[7]++;
            add_to_pc = 0;
            break;
        case HLT:
            running = 0;
            break;
        default:
            printf("unknown instruction at %02x: %02x\n", cpu->pc, IR);
            exit(2);
        }
        cpu->pc += add_to_pc;
        cpu->pc &= 0xff;
    }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
    // TODO: Initialize the PC and other special registers
    cpu->pc = 0;
    cpu->fl = 0;

    // TODO: Zero registers and RAM
    memset(cpu->ram, 0, sizeof cpu->ram);
    memset(cpu->reg, 0, sizeof cpu->reg);

    cpu->reg[7] = 0xf4;
}