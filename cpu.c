
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

///////////////////////////////////////////////////////////
//////////////////// CPU Functions ////////////////////////
///////////////////////////////////////////////////////////

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar)
{
    return cpu->ram[mar];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char mdr)
{
    cpu->ram[index] = mdr;
}

void cpu_load(struct cpu *cpu, char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "error opening file: %s\n", filename);
        exit(2);
    }
    char line[1024];
    int address = 0;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        char *endptr;
        unsigned char byte = strtoul(line, &endptr, 2);
        if (endptr == line)
        {
            continue;
        }
        cpu->ram[address++] = byte;
    }
    fclose(fp);
}

void cpu_run(struct cpu *cpu)
{
    int running = 1;
    unsigned char command, operand1, operand2;

    while (running)
    {

        command = cpu_ram_read(cpu, cpu->PC);

        unsigned char operands = command >> 6;
        operand1 = cpu_ram_read(cpu, cpu->PC + 1);
        operand2 = cpu_ram_read(cpu, cpu->PC + 2);

        switch (command)
        {
        case LDI:
            cpu->reg[operand1] = operand2;
            break;
        case PRN:
            printf("%d\n", cpu->reg[operand1]);
            break;

        case HLT:
            running = 0;
            cpu->PC += 1;
            break;

        case JMP:
            // Jump to the address stored in the given register
            // Set the PC to the address stored in the given register.
            cpu->PC = cpu->reg[operand1] - operands - 1;
            break;

        case CMP:
            if (cpu->reg[operand1] == cpu->reg[operand2])
            {
                cpu->FL = 0b00000001;
            }
            // if op2 is greater set flag to 0b00000100
            else if (cpu->reg[operand1] < cpu->reg[operand2])
            {
                cpu->FL = 0b00000100;
            }
            // if op1 is greater set flag to 0b00000010
            else if (cpu->reg[operand1] > cpu->reg[operand2])
            {
                cpu->FL = 0b00000010;
            }
            break;
        case JEQ:
            // If `equal` flag is set (true),
            //jump to the address stored in the given register.
            if (cpu->FL == 00000001)
            {
                cpu->PC = cpu->reg[operand1] - operands - 1;
            }
            break;

        case JNE:
            //If `E` flag is clear (false, 0),
            //jump to the address stored in the given
            //register.
            if ((cpu->FL & 0b00000001) == 0)
            {
                cpu->PC = cpu->reg[operand1] - operands - 1;
            }
            break;

        default:
            printf("That command was stupid\n");
            exit(1);
        }
        cpu->PC += operands + 1;
    }
}

void cpu_init(struct cpu *cpu)
{
    for (int i = 0; i < 6; i++)
    {
        cpu->reg[i] = 0;
    }
    cpu->reg[7] = 0xF4;
    cpu->PC = 0;
    memset(cpu->ram, 0, sizeof(cpu->ram));
}