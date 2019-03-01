#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fp = fopen(filename, "r");
  char line[8192];
  int address = 0;
  while (fgets(line, sizeof(line), fp) != NULL)
  {
    char *endpointer;
    unsigned char val = strtoul(line, &endpointer, 2);
    if (endpointer == line)
    {
      continue;
    }
    cpu_ram_write(cpu, address++, val);
  }
  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char valA = cpu->registers[regA];
  unsigned char valB = cpu->registers[regB];
  switch (op)
  {
  case ALU_MUL:
    cpu->registers[regA] = valA * valB;
    break;
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
    unsigned char _IR = cpu_ram_read(cpu, cpu->pc);
    // reset the manual pc move flag
    cpu->FL[3] = 0;

    // TODO: refactor with bitwise operations
    int _operands;
    if (_IR > 0b00111111 && _IR < 0b10000000)
    {
      _operands = 1;
    }
    if (_IR >= 0b10000000)
    {
      _operands = 2;
    }

    unsigned char operandA = cpu_ram_read(cpu, cpu->pc + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->pc + 2);

    switch (_IR)
    {

    case CALL: // CALL register //
      /*Calls a subroutine (function) at the address stored 
      in the register.*/
      /*The address of the instruction directly after CALL is 
      pushed onto the stack. This allows us to return to where 
      we left off when the subroutine finishes executing.*/
      cpu->registers[7]--;
      cpu->ram[cpu->registers[7]] = cpu->pc + 1;

      /*The PC is set to the address stored in the given register. 
      We jump to that location in RAM and execute the first 
      instruction in the subroutine. The PC can move forward 
      or backwards from its current location.*/
      cpu->pc = cpu->registers[operandA];
      cpu->FL[3] = 1;
      break;

    case CMP: // CMP registerA registerB //
      /*Compare the values in two registers:
      equal, set the Equal E flag to 1, otherwise set it to 0.
      A is less than registerB, set the Less-than L flag to 1, 
      otherwise set it to 0.
      A is greater than registerB, set the Greater-than G 
      flag to otherwise set it to 0.*/
      printf("%d CMP %d\n", cpu->registers[operandA], cpu->registers[operandB]); // <-debugging
      if (cpu->registers[operandA] == cpu->registers[operandB])
      {
        cpu->FL[0] = 1;
        cpu->FL[1] = 0;
        cpu->FL[2] = 0;
      }
      if (cpu->registers[operandA] < cpu->registers[operandB])
      {
        cpu->FL[0] = 0;
        cpu->FL[1] = 1;
        cpu->FL[2] = 0;
      }
      if (cpu->registers[operandA] > cpu->registers[operandB])
      {
        cpu->FL[0] = 0;
        cpu->FL[1] = 0;
        cpu->FL[2] = 1;
      }
      printf("compare flags E:%d L:%d G:%d\n", cpu->FL[0], cpu->FL[1], cpu->FL[2]); // <-debugging
      break;

    case HLT: // HLT //
      /*Halt the CPU (and exit the emulator).*/
      printf("--done exiting--\n");
      exit(0);
      break;

    // TODO:
    case JEQ: // JEQ register //
      /*If equal flag is set (true), jump to the address 
      stored in the given register.*/
      printf("E %d\n", cpu->FL[0]); // <-debugging
      if (cpu->FL[0] == 1)
      {
        cpu->pc = cpu->registers[operandA];
        printf("JMP to %d\n", cpu->registers[operandA]); // <-debugging
        cpu->FL[3] = 1;
      }
      break;

    // TODO:
    case JGE: // JGE register //
      /*If greater-than flag or equal flag is set (true), 
      jump to the address stored in the given register.*/
      if (cpu->FL[2] == 1 || cpu->FL[0] == 1)
      {
        cpu->pc = cpu->registers[operandA];
        cpu->FL[3] = 1;
      }
      break;

    // TODO:
    case JLE: // JLE register //
      /*If less-than flag or equal flag is set (true), 
      jump to the address stored in the given register.*/
      if (cpu->FL[1] == 1 || cpu->FL[0] == 1)
      {
        cpu->pc = cpu->registers[operandA];
        cpu->FL[3] = 1;
      }
      break;

    case JMP: // JMP register //
      /*Jump to the address stored in the given register.
      Set the PC to the address stored in the given register.*/
      cpu->pc = cpu->registers[operandA];
      cpu->FL[3] = 1;
      break;

    case JNE: // JNE register //
      /*If E flag is clear (false, 0), jump to the address 
      stored in the given register.*/
      if (cpu->FL[0] == 0)
      {
        cpu->pc = cpu->registers[operandA];
        cpu->FL[3] = 1;
      }
      break;

    case LDI: // LDI register immediate //
      /*Set the value of a register to an integer.*/
      // printf("setting register %u to %u\n", operandA, operandB); // <-- debugging
      cpu->registers[operandA] = operandB;
      break;

    case MUL: // MUL registerA registerB // --> ALU
      /*Multiply the values in two registers together 
      and store the result in registerA.*/
      alu(cpu, ALU_MUL, operandA, operandB);
      break;

    case POP: // POP register //
      /*Pop the value at the top of the stack into the given register.
      Copy the value from the address pointed to by SP to the given 
      register.
      Increment SP.*/
      cpu->registers[operandA] = cpu->ram[cpu->registers[7]];
      cpu->registers[7]++;
      break;

    case PRN: // PRN register //
      /*Print numeric value stored in the given register.
      Print to the console the decimal integer value that 
      is stored in the given register.*/
      printf("%u\n", cpu->registers[operandA]);
      break;

    case PUSH: // PUSH register //
      /*Push the value in the given register on the stack.
      Decrement the SP.
      Copy the value in the given register to the address 
      pointed to by SP.*/
      cpu->registers[7]--;
      cpu->ram[cpu->registers[7]] = cpu->registers[operandA];
      break;

    case RET: // RET //
      /*Return from subroutine.
      Pop the value from the top of the stack and store it in the PC.*/
      cpu->pc = cpu->ram[cpu->registers[7]];
      cpu->registers[7]++;
      cpu->FL[3] = 1;
      break;

    default:
      printf("unknown command %u\n", _IR);
      running = 0;
      exit(1);
      break;
    }
    if (cpu->FL[3] == 0)
    {
      cpu->pc = cpu->pc + _operands + 1;
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
  // FL[0] Equal / FL[1] Less Than / FL[2] Greater Than / FL[3] pc manually moved /
  memset(cpu->FL, 0, sizeof(cpu->FL));
  cpu->registers[7] = 243;
}

unsigned char cpu_ram_read(struct cpu *cpu, int index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, int index, unsigned char value)
{
  cpu->ram[index] = value;
  return;
}
