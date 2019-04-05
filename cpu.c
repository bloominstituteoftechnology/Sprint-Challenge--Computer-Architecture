
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

#define DATA_LEN 6
// SP reservs spot 7 in the RAM
#define SP 7

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar)
{
    return cpu->ram[mar];
}

void cpu_ram_read(struct cpu *cpu, unsigned char index, unsigned char mdr)
{
    cpu->ram[index] = mdr;
}

void cpu_push(struct cpu *cpu, unsigned char val)
{
    cpu->reg[SP]--;
    cpu_ram_write(cpu, cpu->reg[SP], val);
}

void cpu_pop(struct cpu *cpu)
{
    unsigned char val = cpu_ram_read(cpu, cpu->reg[SP]);
    cpu->reg[SP]++;
    return val;
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
        operand1 = cpu_ram_read(cpu, cpu->PC + 1);
        operand2 = cpu_ram_read(cpu, cpu->PC + 2);

        switch (command)
        {
        case LDI:
            /* code */
            break;
        case PRN:
            /* code */
            break;
        case HLT:
            /* code */
        case PUSH:
            /* code */
        case POP:
            /* code */
        case CMP:
            /* code */
        case JEQ:
            /* code */
        case JNE:
            /* code */

        default:
            printf("That command was stupid\n");
            exit(1);
            break;
        }
    }
}