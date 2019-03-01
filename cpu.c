#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

#define DATA_LEN 6

/**
 * Reads from a CPU struct's RAM array
 */
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
    return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
    cpu->ram[address] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *path)
{
    FILE *fileptr = fopen(path, "r");
    
    if (fileptr == NULL)
    {
        printf("Couldn't open program: %s\n", path);
        exit(2);
    }

    int address = 0;
    char line[8000];

    while (fgets(line, sizeof(line), fileptr) != NULL)
    {
        char *endptr;
        // & 0xFF gives you the last 8 bits of the ul (so it will always fit in an unsigned char)
        unsigned char value = strtoul(line, &endptr, 2) & 0xFF;
        if (endptr == line)
        {
            continue;
        }
        cpu_ram_write(cpu, address++, value);
    }

    fclose(fileptr);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
    unsigned int a = cpu->registers[regA];
    unsigned int b = cpu->registers[regB];

    switch (op) {
        case ALU_MUL: 
            cpu->registers[regA] = a * b;
            break;
        case ALU_ADD:
            cpu->registers[regA] = a + b;
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
        // 1. Get the value of the current instruction (in address PC).
        unsigned char ir = cpu_ram_read(cpu, cpu->pc);
        // 2. Figure out how many operands this next instruction requires
            // Not entirely needed if we use a switch statement
        // 3. Get the appropriate value(s) of the operands following this instruction
            // Operations may need up to 2 bytes after PC
        unsigned char operandA = cpu_ram_read(cpu, cpu->pc + 1);
        unsigned char operandB = cpu_ram_read(cpu, cpu->pc + 2);
        // 4. switch() over it to decide on a course of action.
        // 5. Do whatever the instruction should do according to the spec.
        // 6. Move the PC to the next instruction.
        switch (ir)
        {
            case LDI:
                // Set the value of a register to an integer
                cpu->registers[operandA] = operandB;
                cpu->pc += 3;
                break;
            case PRN:
                // Print numeric value stored in the given register
                printf("%d\n", cpu->registers[operandA]);
                cpu->pc += 2;
                break;
            case MUL:
                alu(cpu, ALU_MUL, operandA, operandB);
                cpu->pc += 3;
                break;
            case ADD:
                alu(cpu, ALU_ADD, operandA, operandB);
                cpu->pc += 3;
                break;
            case HLT:
                // Halt the CPU (and exit the emulator)
                running = 0;
                break;
            default:
                break;
        }
    }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
    cpu->pc = 0;
    memset(cpu->ram, 0, sizeof(cpu->ram));
    memset(cpu->registers, 0, sizeof(cpu->registers));
    cpu->registers[7] = 0xF4;
}