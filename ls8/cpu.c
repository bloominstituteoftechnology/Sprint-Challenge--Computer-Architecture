#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * read data from RAM
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
void cpu_load(struct cpu *cpu, char* filename)
{
    // open file specified for reading "r"
    FILE* file = fopen(filename, "r");

    // error handling incase `fopen()` fails
    if (file == NULL)
    {
        printf("\nError opening file %s\n\n", filename);
        exit(1);
    }

    char line[100];
    int index = 0;
    char* point;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        // save the content of each line as an `unsigned long` into `cpu->ram`
        // if (point == line)
        //     continue;
        cpu->ram[ index++ ] = strtoul(line, &point, 2);
    }
}

/**
 * ALU
 */
// void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
// {
//   switch (op) {
//     case ALU_MUL:
//       // TODO
//       break;

//     // TODO: implement more ALU ops
//   }
// }

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
    // True until we get a HLT instruction
    int running = 1;
    // determines how much we will increment our PC after each iteration
    unsigned char in_call = 0;

    unsigned char program_counter;
    unsigned char instruction_register;
    unsigned char operandA;
    unsigned char operandB;
    unsigned int next_instruction;

    while (running)
    {
        program_counter = cpu->pc;
        // printf("\nPC: %d\n\n", program_counter);
        instruction_register = cpu_ram_read(cpu, program_counter);

        switch (instruction_register >> 6)
        {
            case 1:
                next_instruction = 2;
                operandA = cpu_ram_read(cpu, program_counter + 1);

                switch (instruction_register)
                {
                    /**
                     * CALL
                     */
                    case CALL:
                        in_call                       = 1;
                        cpu->registers[7]             = --cpu->sp;
                        cpu->ram[ cpu->registers[7] ] = program_counter + next_instruction;
                        cpu->pc                       = cpu->registers[ operandA ];
                        program_counter               = cpu->pc;
                        // printf("\nCALL: Saving %d into RAM[%d]\nJumping to RAM[%d]\n\n", cpu->ram[ cpu->registers[7] ], cpu->registers[7], program_counter);

                        break;
                    
                    /**
                     * JEQ
                     */
                    case JEQ:
                        // printf("\nJEQ\n\n");
                        if (cpu->fl == 1)
                        {
                            // printf("JEQ: JUMP TO RAM[%d]\n\n", cpu->registers[ operandA ]);
                            cpu->pc = cpu->registers[ operandA ];
                            cpu->fl = 0b0;
                        }

                        cpu->fl = 0b0;
                        break;

                    /**
                     * JMP
                     */
                    case JMP:
                        // printf("\nJMP\n\n");
                        // printf("\nJUMPING TO RAM[%d]\n\n", cpu->registers[ operandA ]);
                        cpu->pc = cpu->registers[ operandA ] + 1;
                        // printf("\nPC: %d\n\n", cpu->pc);
                        // continue;
                        break;

                    /**
                     * JNE
                     */
                    case JNE:
                        // printf("\nJNE: CPU->FL = %d\n\n", cpu->fl);

                        if (cpu->fl > 1 || cpu->fl == 0)
                        {
                            // printf("\nJUMPING TO RAM[%d]\n\n", cpu->registers[ operandA ]);
                            cpu->pc = cpu->registers[ operandA ];
                            // printf("\nRAM[%d]\n\n", cpu->pc);
                            continue;
                        }
                        
                        break;
                    
                    /**
                     * POP
                     */
                    case POP:
                        // set the value where `stack pointer` is pointing to in to the given register
                        cpu->registers[ operandA ] = cpu->ram[ cpu->registers[7] ];
                        // increase where our stack pointer is pointing to on RAM
                        cpu->registers[7] = ++cpu->sp;
                        break;
                    
                    /**
                     * PRN
                     */
                    case PRN:
                        printf("\nPRN: %d\n\n", cpu->registers[ operandA ]);
                        break;

                    /**
                     * PUSH
                     */
                    case PUSH:
                        // decrease `stack pointer` by 1
                        cpu->registers[7] = --cpu->sp;
                        // set the value of `R1` to wherever our `stack pointer`, `R7`, is pointing to
                        cpu->ram[ cpu->registers[7] ] = cpu->registers[ operandA ];
                        break;

                    /**
                     * default
                     */
                    default:
                        break;
                    }

                break;  // case 1

            case 2:
                next_instruction = 3;
                operandA = cpu_ram_read(cpu, program_counter + 1);
                operandB = cpu_ram_read(cpu, program_counter + 2);

                switch (instruction_register)
                {
                    case ADD:
                        // printf("\nADD: %d + %d = ", cpu->registers[ operandA ], cpu->registers[ operandB ]);
                        cpu->registers[ operandA ] += cpu->registers[ operandB ];
                        // printf("%d\n\n", cpu->registers[ operandA ]);
                        break;

                    case CMP:
                        // printf("\nCMP: %d || %d = ", cpu->registers[ operandA ], cpu->registers[ operandB ]);

                        if (cpu->registers[ operandA ] > cpu->registers[ operandB ])
                            cpu->fl = 0b01;
                        else if (cpu->registers[ operandA ] < cpu->registers[ operandB ])
                            cpu->fl = 0b100;
                        else
                            cpu->fl = 0b1;

                        // printf("%d\n\n", cpu->fl);
                        break;
                    
                    case LDI:
                        cpu->registers[ operandA ] = operandB;
                        // printf("\nLDI: Saving %d into R[%d]\n\n", operandB, operandA);
                        break;

                    case MUL:
                        cpu->registers[ operandA ] = cpu->registers[ operandA ] * cpu->registers[ operandB ];
                        break;

                    default:
                        break;
                }

                break;  // case 2

            default:
                switch(instruction_register)
                {
                    case HLT:
                        // printf("\nHLT\n\n");
                        running = 0;
                        break;

                    case RET:
                        cpu->pc = cpu->ram[ cpu->sp++ ];
                        cpu->registers[7] = cpu->pc;
                        // printf("\nRETURNING TO RAM[%d]\n\n", cpu->pc);
                        continue;
                        break;

                    default:
                        in_call = 1;
                }
        }

        if (in_call == 0)
            cpu->pc = cpu->pc + next_instruction;
        else if (in_call == 1)
        {
            cpu->pc++;
            in_call = 0;
        }
    }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
    // TODO: Initialize the PC and other special registers
    cpu->fl = 0b00000000;
    cpu->pc = 0;
    cpu->sp = 244;
    cpu->registers[7] = cpu->sp;

    // TODO: Zero registers and RAM
}