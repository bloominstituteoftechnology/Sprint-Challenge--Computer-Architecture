#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * THIS IS WHAT WILL HAPPEN LATER
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

// STEP 2
unsigned char cpu_ram_read(struct cpu *cpu, int index)
{
    return cpu->ram[index];
}

// should this return something or just write?
unsigned char cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
    return cpu->ram[address] = value;
}

void cpu_load(struct cpu *cpu, char *filename)
{
    // STEP 7.2: TODO: Replace this with something less hard-coded
    unsigned char address = 0;
    // variable to hold the line
    char line[256];
    // pointer to hold file
    FILE *fp;

    // open file
    fp = fopen(filename, "r");
    if (fp == NULL) // error handling for opening file
    {
        fprintf(stderr, "Unable to open file: %s\n", filename);
        exit(3);
    }

    // fgets gets the line and sets it to our variable "line" and then moves the pointer to the next line
    while (fgets(line, sizeof line, fp) != NULL)
    {
        //printf("%s\n", line);
        // variable for the first non number from strtoul
        char *end;
        // variable for the output of strtoul()
        unsigned char b_number;

        // strtoul calculates the value in what base(2) we want into b_number
        // it assigns the first non-number in the "line" to "end"
        b_number = strtoul(line, &end, 2);

        // if line is empty aka "\n" then line and end are equal
        if (line == end)
        {
            continue;
        }
        // assign cpu->ram[address] with b_number and increment address
        cpu_ram_write(cpu, address++, b_number);
    }

    fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
    unsigned char *registers = cpu->registers;
    switch (op)
    {
    case ALU_MUL:
        // TODO
        registers[regA] *= registers[regB];
        break;

        // TODO: implement more ALU ops
    case ALU_ADD:
        registers[regA] += registers[regB];
        break;
    case ALU_CMP:;
        int result = cpu->registers[regA] - cpu->registers[regB];
        if (result == 0)
        {
            cpu->FL = 1;
        }
        else if (result > 0)
        {
            cpu->FL = 2;
        }
        else
        {
            cpu->FL = 4;
        }
        break;
    }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
    int running = 1; // True until we get a HLT instruction
    unsigned char *PC = &cpu->PC;
    unsigned char *registers = cpu->registers;
    // unsigned char *ram = cpu->ram;
    unsigned char *SP = &cpu->registers[7];
    //int lineCount = 1;

    while (running)
    {
        // printf("1: Entered While Loop\n"); //==================
        // === STEP 3 TODO ===
        // 1. Get the value of the current instruction (in address PC).
        // 2. switch() over it to decide on a course of action.
        // 3. Do whatever the instruction should do according to the spec.
        // 4. Move the PC to the next instruction.

        //Unstructure variables from cpu struct
        unsigned char *IR = &cpu->IR;
        unsigned char *MAR = &cpu->MAR; // operandA == MAR
        unsigned char *MDR = &cpu->MDR; // operandB == MDR

        // Set IR using cpu_ram_read
        *IR = cpu_ram_read(cpu, *PC);
        printf("  IR: %x\n", *IR);

        //get numArgs using bitwise rightshift on IR
        int numArgs = (*IR >> 6) + 1;
        // printf("numArgs: %d\n", numArgs);

        // Switch to set MDR and MAR so we only read when we need to
        switch (numArgs)
        {
        case 3:
            // printf("2: MDR\n");
            *MDR = cpu_ram_read(cpu, *PC + 2);
            printf(" MDR:  %i\n", *MDR);
        case 2:
            // printf("2.5: MAR\n");
            *MAR = cpu_ram_read(cpu, *PC + 1);
            printf(" MAR:  %i\n", *MAR);
            break;
        }
        switch (*IR)
        {
        // STEP 4
        case HLT:
            // printf("4: HLT\n");
            running = 0;
            break;
        // STEP 5
        case LDI:
            // printf("5: LDI\n");
            registers[*MAR] = *MDR;
            break;
        // STEP 6
        case PRN:
            // printf("6: PRN\n");
            printf("%d\n", registers[*MAR]);
            break;
        case MUL:
            // if it's case MUL, then send it to alu();
            alu(cpu, ALU_MUL, *MAR, *MDR);
            break;

            // STEP 10.2
        case PUSH:
            cpu_ram_write(cpu, --*SP, registers[*MAR]);
            break;
        case POP:
            registers[*MAR] = cpu_ram_read(cpu, *SP++);
            break;

            // STEP 11.2
        case ADD:
            alu(cpu, ALU_ADD, *MAR, *MDR);
            break;
        case CALL:
            // move SP to new line, assign it address of instruction to be run upon RET
            cpu_ram_write(cpu, --*SP, *PC + (numArgs - 1));
            // Move PC counter to the value at reg[MAR] which is ADD&Print thing for us
            // minus numArgs bc it's added back on the after the switch statement
            *PC = registers[*MAR] - numArgs;
            break;
        case RET:
            // upon return, set PC back to location that was previously set on CALL
            *PC = cpu_ram_read(cpu, *SP);
            // Move SP up one now that that value is now useless and should be reused
            *SP += 1;
            break;

        // STEP SPRINT ====
        case CMP:
            alu(cpu, ALU_CMP, *MAR, *MDR);
            break;
        case JEQ:
            if ((cpu->FL & 0b1) == 0b1)
            {
                *PC = registers[*MAR] - numArgs;
            }
            break;
        case JMP:
            *PC = registers[*MAR] - numArgs;
            break;
        case JNE:
            if ((cpu->FL & 0b1) == 0b0)
            {
                *PC = registers[*MAR] - numArgs;
            }
            break;

        default:
            // from solution lecture
            fprintf(stderr, "Unknown instruction at line %i: %02x\n", *PC + 1, *IR);
            running = 0;
            exit(2);
        }
        //printf("Trace PC: %d\n", PC);
        *PC += numArgs;
        //lineCount += numArgs;
    }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
    // TODO: Initialize the PC and other special registers
    cpu->PC = 0;

    // TODO: Zero registers and RAM
    // for loop to put 0 value into every position ..... or just use memset
    memset(cpu->registers, 0, sizeof cpu->registers);
    memset(cpu->ram, 0, sizeof cpu->ram);

    // Step 10: set stack pointer to position f4
    cpu->registers[7] = 0xf4;
}
