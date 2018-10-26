#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
    return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
    cpu->ram[address] = value;
}

void cpu_push(struct cpu *cpu, unsigned char value)
{
    cpu->reg[SP]--;
    cpu_ram_write(cpu, cpu->reg[SP], value);
}

unsigned char cpu_pop(struct cpu *cpu)
{
    unsigned char value = cpu_ram_read(cpu, cpu->reg[SP]);
    cpu->reg[SP]++;

    return value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
    FILE *fp = fopen(filename, "r");

    char line[1024];

    int address = cpu->PC;

    if (fp == NULL)
    {
        fprintf(stderr, "Can't open file. %s\n", filename);
        exit(2);
    }

    char *endchar;

    while (fgets(line, sizeof line, fp) != NULL)
    {
        unsigned char byte = strtol(line, &endchar, 2);

        if (endchar == line)
        {
            continue;
        }

        cpu_ram_write(cpu, address++, byte);
    }
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
    switch (op)
    {
    case ALU_MUL:
        cpu->reg[regA] *= cpu->reg[regB];
        break;

    case ALU_ADD:
        cpu->reg[regA] += cpu->reg[regB];
        break;

    default:
        printf("Unknown ALU case @ %02x: %02x\n", cpu->PC, op);
        exit(3);
    }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
    int running = 1;

    while (running)
    {
        // 1. Get the value of the current instruction (in address PC).
        unsigned char IR = cpu_ram_read(cpu, cpu->PC);

        unsigned char operandA = cpu_ram_read(cpu, (cpu->PC + 1) & MAX_ADDR);
        unsigned char operandB = cpu_ram_read(cpu, (cpu->PC + 2) & MAX_ADDR);

        // 2. switch() over it to decide on a course of action.
        // 3. Do whatever the instruction should do according to the spec.

        switch (IR)
        {

        case PRN:
            printf("%d\n", cpu->reg[operandA]);
            break;

        case LDI:
            cpu->reg[operandA] = operandB;
            break;

        case HLT:
            running = 0;
            break;

        case MUL:
            cpu->reg[operandA] *= cpu->reg[operandB];
            cpu->PC += 3;

            break;

        case ADD:
            cpu->reg[operandA] += cpu->reg[operandB];
            cpu->PC += 3;
            break;

        case PUSH:
            cpu_push(cpu, cpu->reg[operandA]);
            break;

        case POP:
            cpu->reg[operandA] = cpu_pop(cpu);
            break;

        case PRA:
            printf("%c\n", cpu->reg[operandA]);
            break;

        case CALL:
            cpu_push(cpu, cpu->PC + 2);
            cpu->PC = cpu->reg[operandA];
            break;

        case RET:
            cpu->PC = cpu_pop(cpu);
            break;

            // Sprint Challenge:

        case CMP:
            if (cpu->reg[operandA] == cpu->reg[operandB])
            {
                cpu->FL = 1;
            }
            else
            {
                cpu->FL = 0;
            }
            break;

        case JMP:
            cpu->PC = cpu->reg[operandA];
            break;

        case JEQ:
            if (cpu->FL)
            {
                cpu->PC = cpu->reg[operandA];
            }
            else
            {
                cpu->PC += 2;
            }
            break;

        case JNE:
            if (!cpu->FL)
            {
                cpu->PC = cpu->reg[operandA];
            }
            else
            {
                cpu->PC += 2;
            }
            break;

        default:
            printf("Unknown instruction at %02x: %02x\n", cpu->PC, IR);
            exit(2);
        }

        // 4. Move the PC to the next instruction.

        int instruction_set_PC = (IR >> 4) & 1;

        if (!instruction_set_PC)
        {
            cpu->PC += ((IR >> 6) & 0x3) + 1;
        }
    }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
    cpu->PC = START_OF_PROGRAM_ADDR;
    cpu->FL = CPU_FLAG;

    cpu->reg[SP] = START_OF_STACK_ADDR;
    cpu->reg[IM] = INTERRUPT_MASK;
    cpu->reg[IS] = INTERRUPTS;

    memset(cpu->reg, 0, sizeof cpu->reg);
    memset(cpu->ram, 0, sizeof cpu->ram);
}