#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned char cpu_ram_read(struct cpu *cpu, int index)
{
  return cpu->ram[index];  // access RAM inside struct cpu
}

void cpu_ram_write(struct cpu *cpu, int index, unsigned char value)
{
  cpu->ram[index] = value;  // access RAM inside struct cpu
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file)
{ 
    FILE * fp;
    fp = fopen(file, "r");
    char str[30];

    if (fp == NULL) {
      printf("Error: Could not open file\n");
      exit(1);
    }

    int address = 0;

    while( fgets(str, sizeof(str), fp) != NULL )
    {
        unsigned char binary;
        char *endptr;
        binary = strtoul(str, &endptr, 2);
        if (str != endptr) {
            cpu->ram[address++] = binary;
        }
    }

    fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->registers[regA] *= cpu->registers[regB];
      break;

    case ALU_ADD:
      cpu->registers[regA] += cpu->registers[regB];
      break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  // var time = gettimeofday();

  while (running) {

    // if (gettimeofday() - time >= 1s) {
     // time = gettimeofday();
     // set bit 0 of R6 (interrupt status) cpu->registers[IS] = (cpu->registers[IS] | 0b00000001);
    //}

    if (cpu->registers[IS] > 0) { //if interrupts are enabled
      unsigned char interrupts = cpu->registers[IM] & cpu->registers[IS];
      for (int i = 0; i < 8; i++) {
        if (((interrupts >> i) & 1) == 1) { // Right shift interrupts down by i, then mask with 1 to see if that bit was set
          // 1. Disable further interrupts.
          ????????????
          // 2. Clear the bit in the IS register.
          cpu->registers[IS] = (cpu->registers[IS] & 0b11111110);
          // 3. The `PC` register is pushed on the stack.
          cpu->registers[SP] --;
          cpu_ram_write(cpu, cpu->registers[SP], cpu->PC);
          // 4. The `FL` register is pushed on the stack.
          cpu->registers[SP] --;
          cpu_ram_write(cpu, cpu->registers[SP], cpu->FL);
          // 5. Registers R0-R6 are pushed on the stack in that order.
          for (int j = 0; j < 7; j++) {
            cpu->registers[SP] --;
            cpu_ram_write(cpu, cpu->registers[SP], cpu->registers[j]);
          }
          // 6. The address (_vector_ in interrupt terminology) of the appropriate
          //   handler is looked up from the interrupt vector table.
          // 7. Set the PC is set to the handler address.
          return;
        } else {
          continue;
        }
      }
    }
  
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);  // Get the value of the current instruction (in address PC)
    int move_pc = (IR >> 6) + 1;  // move counter

    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);

    switch (IR) {
      case HLT:   //Halt the CPU (and exit the emulator).
        running = 0;
        break;

      case LD:  //Loads registerA with the value at the memory address stored in registerB.
        cpu->registers[operandA] = cpu_ram_read(cpu, cpu->registers[operandB]);
        cpu->PC += move_pc;
        break;

      case LDI:  //Set the value of a register to an integer.
        cpu->registers[operandA] = operandB;
        cpu->PC += move_pc;
        break;

      case PRN:   //Print numeric value stored in the given register.
        printf("Saved value: %d\n", cpu->registers[operandA]);
        cpu->PC += move_pc;
        break;

      case MUL:   //Multiply the values in two registers together and store the result in registerA.
        alu(cpu, ALU_MUL, operandA, operandB);
        cpu->PC += move_pc;
        break;

      case ADD:   //Add the value in two registers and store the result in registerA.
        alu(cpu, ALU_ADD, operandA, operandB);
        cpu->PC += move_pc;
        break;

      case INC:   //Increment (add 1 to) the value in the given register.
        cpu->registers[operandA] ++;
        cpu->PC += move_pc;
        break;

      case DEC:   //Decrement (subtract 1 from) the value in the given register.
        cpu->registers[operandA] --;
        cpu->PC += move_pc;
        break;

      case PUSH:  //Push the value in the given register on the stack.
        cpu->registers[SP] --;
        cpu_ram_write(cpu, cpu->registers[SP], cpu->registers[operandA]);
        cpu->PC += move_pc;
        break;

      case POP:   //Pop the value from the top of the stack and store it in the `PC`.
        cpu->registers[operandA] = cpu_ram_read(cpu, cpu->registers[SP]);
        cpu->registers[SP] ++;
        cpu->PC += move_pc;
        break;

      case CALL:  //Calls a subroutine (function) at the address stored in the register.
        cpu->registers[SP] --;
        cpu_ram_write(cpu, cpu->registers[SP], cpu->PC + 2);
        cpu->PC = cpu->registers[operandA];
        break;

      case RET:  //Return from subroutine.
        cpu->PC = cpu_ram_read(cpu, cpu->registers[SP]);
        cpu->registers[SP] ++;
        break;

      case IRET:  //Return from an interrupt handler.
        // 1. Registers R6-R0 are popped off the stack in that order.
        for (int i = 6; i >= 0; i--) {
          cpu->registers[i] = cpu_ram_read(cpu, cpu->registers[SP]);
          cpu->registers[SP] ++;
        }
        // 2. The `FL` register is popped off the stack.
        cpu->FL = cpu_ram_read(cpu, cpu->registers[SP]);
        cpu->registers[SP] ++;
        // 3. The return address is popped off the stack and stored in `PC`.
        cpu->PC = cpu_ram_read(cpu, cpu->registers[SP]);
        cpu->registers[SP] ++;
        // 4. Interrupts are re-enabled
        ??????????????????????
        break;

      case ST:  //Store value in registerB in the address stored in registerA.
        cpu_ram_write(cpu, cpu->registers[operandA], cpu->registers[operandB]);
        cpu->PC += move_pc;
        break;

      case PRA:  //Print alpha character value stored in the given register.
        printf("%c", cpu->registers[operandA]);
        cpu->PC += move_pc;
        break;

      case CMP:  //Compare the values in two registers.
        // * If they are equal, set the Equal `E` flag to 1, otherwise set it to 0.
        if (cpu->registers[operandA] == cpu->registers[operandB]) {
          cpu->FL = 1;
        }
        // * If registerA is less than registerB, set the Less-than `L` flag to 1,
        //   otherwise set it to 0.
        if (cpu->registers[operandA] < cpu->registers[operandB]) {
          cpu->FL = 4;
        }
        // * If registerA is greater than registerB, set the Greater-than `G` flag
        //   to 1, otherwise set it to 0.
        if (cpu->registers[operandA] > cpu->registers[operandB]) {
          cpu->FL = 2;
        }
        cpu->PC += move_pc;
        break;

      case JMP:   //Jump to the address stored in the given register.
        cpu->PC = cpu->registers[operandA];
        break;

      case JEQ:  //If `equal` flag is set (true), jump to the address stored in the given register.
        if ((cpu->FL & 0b00000001) == 1) {
          cpu->PC = cpu->registers[operandA];
        } else {
          cpu->PC += move_pc;
        }
        break;

      case JNE:  //If `E` flag is clear (false, 0), jump to the address stored in the given register.
        if ((cpu->FL & 0b00000001) == 0) {
          cpu->PC = cpu->registers[operandA];
        } else {
          cpu->PC += move_pc;
        }
        break;

      default:
        exit(1);
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // Initialize the PC and other special registers
  cpu->PC = 0;
  cpu->FL = 0;
  // Zero registers and RAM
  memset(cpu->registers, 0, sizeof(cpu->registers));
  memset(cpu->ram, 0, sizeof(cpu->ram));

  cpu->registers[SP] = 0xF4;
}
