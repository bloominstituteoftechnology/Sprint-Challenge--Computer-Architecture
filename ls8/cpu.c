#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>


#define DATA_LEN 6

/**
 * Read from RAM
 */
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
    // address = MAR
    return cpu->ram[address];
}

/**
 * Write to RAM
 */
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
    // value = MDR, address = MAR
    cpu->ram[address] = value;
}


/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
    FILE *file = fopen(filename, "r");
    
    int address = 0;
    char line[8000];
    
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char *end;
        unsigned char value = strtoul(line, &end, 2) & 0xFF;
        if (end == line)
        {
            continue;
        }
        cpu_ram_write(cpu, address++, value);
    }
    
    fclose(file);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB) // does math and operations
{
    unsigned int a = cpu->registers[regA];
    unsigned int b = cpu->registers[regB];
    
    switch (op) {
    case ALU_MUL:
      cpu->registers[a] *= cpu->registers[b];
      break;
    case ALU_ADD:
        cpu->registers[a] += cpu->registers[b];
        break;
    case CMP:
        if (cpu->registers[a] == cpu->registers[b]) {
            cpu->flag = 1;
        }
        else {
            cpu->flag = 0;
        }
        cpu->pc = cpu->pc + op + 1;
        break;
  }
}

void cpu_push(struct cpu *cpu, unsigned char val)
{
    cpu->registers[7]--;
    cpu_ram_write(cpu, cpu->registers[7], val);
}

unsigned char cpu_pop(struct cpu *cpu)
{
    unsigned char val = cpu_ram_read(cpu, cpu->registers[7]);
    cpu->registers[7]++;
    return val;
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
    int running = 1; // True until we get a HLT instruction

    while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
        unsigned char ir = cpu_ram_read(cpu, cpu->pc);
    // 2. Figure out how many operands this next instruction requires
        
        unsigned int num_ops = ir >> 6;
    // 3. Get the appropriate value(s) of the operands following this instruction
        unsigned char operand0 = cpu_ram_read(cpu, cpu->pc + 1); // register number
        unsigned char operand1 = cpu_ram_read(cpu, cpu->pc + 2); // value
        
        printf("TRACE: %02X: %02X     %02X %02X\n", cpu->pc, ir, operand0, operand1);
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
          switch (ir) {
              case LDI:
                  cpu->registers[operand0] = operand1;
                  cpu->pc = cpu->pc + num_ops + 1;
                  break;
              case PRN:
                  printf("%d\n", cpu->registers[operand0]);
                  // 6. Move the PC to the next instruction.
                  cpu->pc = cpu->pc + num_ops + 1;
                  break;
              case MUL:
                  //alu(cpu, ALU_MUL, operand0, operand1);
                  cpu->registers[operand0] *= cpu->registers[operand1];
                  cpu->pc = cpu->pc + num_ops + 1;
                  break;
              case ADD:
                  //alu(cpu, ALU_ADD, operand0, operand1);
                  cpu->registers[operand0] += cpu->registers[operand1];
                  cpu->pc = cpu->pc + num_ops + 1;
                  break;
              case POP:
                  cpu->registers[operand0] = cpu_pop(cpu);
                  cpu->pc = cpu->pc + num_ops + 1;
                  break;
              case PUSH:
                  cpu_push(cpu, cpu->registers[operand0]);
                  cpu->pc = cpu->pc + num_ops + 1;
                  break;
              case CALL:
                  cpu_push(cpu, cpu->pc + 2);
                  cpu->pc = cpu->registers[operand0];
                  break;
              case RET:
                  cpu->pc = cpu_pop(cpu);
                  break;
              case CMP:
                  //alu(cpu, CMP, operand0, operand1);
                  
                  if (cpu->registers[operand0] == cpu->registers[operand1]) {
                      cpu->flag = 1;
                  }
                  else {
                      cpu->flag = 0;
                  }
                  cpu->pc = cpu->pc + num_ops + 1;
                  break;
              case JMP:
                  cpu->pc = cpu->registers[operand0];
                  break;
              case JEQ:
                  if (cpu->flag == 1) {
                      cpu->pc = cpu->registers[operand0];
                  } else {
                      cpu->pc = cpu->pc + num_ops + 1;
                  }
                  break;
              case JNE:
                  if (cpu->flag != 1) {
                      cpu->pc = cpu->registers[operand0];
                  } else {
                      cpu->pc = cpu->pc + num_ops + 1;
                  }
                  break;
              case HLT:
                  running = 0;
                  break;
              default:
                  printf("Unexpected instruction 0x%02X at 0%02X\n", ir, cpu->pc);
                  exit(1);
          }
  }
} // INC ... extract two bits (first two digits) from machine code number --> number of operands --> add to pc + 1

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
    // Init registers
    memset(cpu->registers, 0, 8); // Same as loop and cpu->reg[i]
    cpu->registers[7] = 0xF4;
    // Init PC
    cpu->pc = 0;
    // Init RAM
    memset(cpu->ram, 0, sizeof(cpu->ram));
    
    cpu->flag = 0;
}
