#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

/**
 * cpu_ram_read/cpu_ram_write
 */
unsigned char cpu_ram_read(struct cpu *cpu, int idx)
{
    return cpu->ram[idx];
}

void cpu_ram_write(struct cpu *cpu, int idx, unsigned char val)
{
    cpu->ram[idx] = val;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
    FILE *fp;
    char line[1024];
    int address = 0;

    // Open the source file
    if ((fp = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "Cannot open file %s\n", filename);
        exit(2);
    }

    // Read all the lines and store them in RAM
    while (fgets(line, sizeof line, fp) != NULL)
    {

        // Convert string to a number
        char *endchar;
        unsigned char byte = strtol(line, &endchar, 2);
        ;

        // Ignore lines from whicn no numbers were read
        if (endchar == line)
        {
            continue;
        }

        // Store in ram
        cpu->ram[address++] = byte;
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
        // TODO
        cpu->reg[regA] = cpu->reg[regA] * cpu->reg[regB];
        break;
    case ALU_ADD:
        cpu->reg[regA] += cpu->reg[regB];
        break;
    }
    // TODO: implement more ALU ops
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
    int running = 1; // True until we get a HLT instruction
    while (running)
    {
        int PC = cpu->PC;
        unsigned char IR = cpu_ram_read(cpu, PC); //instruction register
        unsigned char operandA = cpu_ram_read(cpu, PC + 1);
        unsigned char operandB = cpu_ram_read(cpu, PC + 2);

        // printf("TRACE: %02X:  %02X  %02X  %02X\n", cpu->PC, IR, operandA, operandB);
        // True if this instruction might set the PC
        int instruction_set_pc = (IR >> 4) & 1;

        switch (IR)
        {

        case LDI:
            cpu->reg[operandA] = operandB;
            break;

        case PRN:
            printf("\nValue at register %d: %d\n", operandA, cpu->reg[operandA]);
            break;

        case MUL:
            alu(cpu, ALU_MUL, operandA, operandB);
            break;

        case ADD:
            alu(cpu, ALU_ADD, operandA, operandB);
            break;

        case HLT:
            running = 0;
            break;

        case PRA:
            printf("%c\n", cpu->reg[operandA]);
            //printf("%c", cpu->reg[operandA]); fflush(stdout); // Without newline
            break;

        case CALL:
            cpu->reg[SP]--;
            cpu_ram_write(cpu, cpu->reg[SP], cpu->PC + 2);
            cpu->PC = cpu->reg[operandA];
            break;

        case RET:
            cpu->PC = cpu_ram_read(cpu, cpu->reg[SP]);
            cpu->reg[SP]++;
            break;

        case PUSH:
            cpu->reg[SP]--;
            cpu_ram_write(cpu, cpu->reg[SP], cpu->reg[operandA]);
            break;

        case POP:
            cpu->reg[operandA] = cpu->ram[cpu->reg[SP]];
            cpu->reg[SP]++;
            break;

        default:
            fprintf(stderr, "PC %02x: unknown instruction %02X\n", cpu->PC, IR);
            exit(3);
        }

        if (!instruction_set_pc)
        {
            cpu->PC += (IR >> 6) + 1;
        }
    }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
    cpu->PC = 0;

    // Zero registers and RAM
    memset(cpu->reg, 0, sizeof cpu->reg);
    memset(cpu->ram, 0, sizeof cpu->ram);

    // Initialize SP
    cpu->reg[SP] = EMPTY_STACK;
}