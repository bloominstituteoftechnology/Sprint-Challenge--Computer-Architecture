#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

// push a value to the CPU stack
void cpu_push(struct cpu *cpu, unsigned char val)
{
    cpu->reg[SP]--;

    cpu->ram[cpu->reg[SP]] = val;
}

// pop a value from the CPU stack
unsigned char cpu_pop(struct cpu *cpu)
{
    unsigned char val = cpu->ram[cpu->reg[SP]];

    cpu->reg[SP]++;

    return val;
}

// helper functions
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char MAR)
{
    // MAR: Memory Address Register, holds the memory address we're reading
    return cpu->ram[MAR];
}

void cpu_ram_write(struct cpu *cpu, unsigned char MAR, unsigned char MDR)
{
    // MDR: Memory Data Register, holds the value to write or the value just read
    cpu->ram[MAR] = MDR;
}

// load the binary bytes from an .ls8 source file into a RAM array
void cpu_load(char *filename, struct cpu *cpu)
{
    // You can use the fopen( ) function to create a new file or to open an existing 
    // file. This call will initialize an object of the type FILE, which contains all 
    // the information necessary to control the stream.
    FILE *fp;
    // allocate 1024 bytes for a string variable named "line"
    char line[1024];
    // check out where ADDR_PROGRAM_Entry comes from
    // answer for ^,  it's in cpu.h, defined as 0xF4
    int address = ADDR_PROGRAM_ENTRY;

    // open the source file
    if ((fp = fopen(filename, "r")) == NULL)
    {
        // error handling
        fprintf(stderr, "Cannot open file %s\n", filename);
        exit(2);
    }

    // read all the lines and store them in RAM
    // fgets = reads a line from the specified stream and stores it 
    // into the string pointed to by str
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        // convert string to a number
        char *endchar;
        unsigned char byte = strtol(line, &endchar, 2);

        // ignore lines from which no numbers were read
        if (endchar == line)
        {
            continue;
        }

        // store it in RAM
        cpu->ram[address++] = byte;
    }
}

void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
    // variable declarations
    unsigned char *reg = cpu->reg;
    unsigned char valA = reg[regA];
    unsigned char valB = reg[regB];

    switch (op)
    {
        case ALU_MUL:
            valA *= valB;
            break;

        case ALU_ADD:
            valA += valB;
            break;

        case ALU_CMP:
            // compares values
            if (valA == valB) 
            {
                cpu->fl = cpu->fl | FL_EQ; // could be written |=
            }
            else
            {
                cpu->fl = cpu->fl & (~FL_EQ);  // could be written &=
            }
            break;
    }
}

void cpu_run(struct cpu *cpu)
{
    // we create a 'running' variable, and set it to 1, for TRUE
    // it is TRUE until we get an HLT instruction
    int running = 1;

    while (running)
    {
        // 1. Get the value of the current instruction (in address PC).
        // It needs to read the memory address that's stored in register PC, 
        // and store that result in IR, the Instruction Register.
        unsigned char IR = cpu_ram_read(cpu, cpu->pc);
        // read the bytes at PC+1 and PC+2 from RAM into variables operandA 
        // and operandB in case the instruction needs them.
        unsigned char operandA = cpu_ram_read(cpu, cpu->pc+1);
        unsigned char operandB = cpu_ram_read(cpu, cpu->pc+2);

        // True if this instruction might set the counter;
        int instruct_set_pc = (IR >> 4) & 1;


        // 2. switch() over it to decide on a course of action.
        // 3. Do whatever the instruction should do according to the spec.
        switch(IR) {
            case LDI:
                cpu->reg[operandA] = operandB;
                break;

            case PRN:
                printf("%d\n", cpu->reg[operandA]);
                break;

            case MUL:
                alu(cpu, ALU_MUL, operandA, operandB);
                break;

            case ADD:
                alu(cpu, ALU_ADD, operandA, operandB);
                break;

            case PUSH:
                cpu->reg[SP]--;
                cpu_ram_write(cpu, cpu->reg[SP], cpu->reg[operandA]);
                break;
            
            case POP:
                cpu->reg[operandA] = cpu_ram_read(cpu, cpu->reg[SP]);
                cpu->reg[SP]++;
                break;

            case CALL:
                cpu_push(cpu, cpu->pc + 2);
                cpu->pc = cpu->reg[operandA];
                break;

            case RET:
                cpu->pc = cpu_pop(cpu);
                break;

            case JMP:
                cpu->pc = cpu->reg[operandA];
                break;

            case CMP:
                alu(cpu, ALU_CMP, operandA, operandB);
                break;

            case JEQ:
                if (cpu->fl & FL_EQ)
                {
                    cpu->pc = cpu->reg[operandA];
                }
                else
                {
                    instruct_set_pc = 0;
                }
                break;
            
            case JNE:
                if(!(cpu->fl & FL_EQ))
                {
                    cpu->pc = cpu->reg[operandA];
                }
                else
                {
                    instruct_set_pc = 0;   
                }
                break;

            case HLT:
                running = 0;
                break;

            default:
                printf("unkown instruction at %02x: %02x\n", cpu->pc, IR);
                exit(2);
        }
        
        // 4. Move the PC to the next instruction.
        if (!instruct_set_pc)
        {
            // >> splices off 6 characters in IR
            cpu->pc += (IR >> 6) + 1;
        }
    }
}

// Initialize a CPU struct
void cpu_init(struct cpu *cpu)
{
    // init the PC and other special registers
    cpu->pc = 0;
    cpu->fl = 0;

    // zero registers and RAM
    memset(cpu->reg, 0, sizeof cpu->reg);
    memset(cpu->ram, 0, sizeof cpu->ram);

    // init SP
    cpu->reg[SP] = ADDR_EMPTY_STACK;
}
