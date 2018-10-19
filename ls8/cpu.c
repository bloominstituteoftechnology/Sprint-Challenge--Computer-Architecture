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

    //Add FLAG Register
    int FL = 0; //00000LGE (LGE = 000)

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
            printf("Value at register %d: %d\n", operandA, cpu->reg[operandA]);
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

        case CMP:
            //if A<B
            if (cpu->reg[operandA] < cpu->reg[operandB])
            {
                printf("CMP: %d < %d\n", cpu->reg[operandA], cpu->reg[operandB]);
                FL = 4; //set FL to 00000100 (LGE = 100)
            }
            //if A>B
            else if (cpu->reg[operandA] > cpu->reg[operandB])
            {
                printf("CMP: %d > %d\n", cpu->reg[operandA], cpu->reg[operandB]);
                FL = 2; //set FL to 00000010 (LGE = 010)
            }
            //if A==B
            else
            {
                printf("CMP: %d = %d\n", cpu->reg[operandA], cpu->reg[operandB]);
                FL = 1; //set FL to 00000001 (LGE = 001)
            }
            break;

        case JMP:
            //Perform a jump: Set PC to new location
            cpu->PC = cpu->reg[operandA];
            break;

        case JEQ:
            //If FL == 1 then perform the jump and change PC count to the new location
            if (FL == 1)
            {
                printf("JEQ: Jumping to %02X\n", cpu->reg[operandA]);
                cpu->PC = cpu->reg[operandA];
            }
            else //If not equal then do not perform the jump, but still update PC to the next instruction
            {
                printf("JEQ: Not Jumping\n");
                cpu->PC += 2;
            }
            break;

        case JNE:
            //If FL == 2 or 4 (not equal) then perform the jump and change PC count to the new location
            if (FL == 2 || FL == 4)
            {
                printf("JNE: Jumping to %02X\n", cpu->reg[operandA]);
                cpu->PC = cpu->reg[operandA];
            }
            else //If equal then do not perform the jump, but still update PC to the next instruction
            {
                printf("JNE: Not Jumping\n");
                cpu->PC += 2;
            }
            break;

        default:
            fprintf(stderr, "PC %02x: unknown instruction %02X\n", cpu->PC, IR);
            exit(3);
        }

        //Only run if instruction is not PC changing type or not JMP, JEQ, JNE
        if (!instruction_set_pc && IR != JMP && IR != JEQ && IR != JNE)
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