#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void handle_instruction(struct cpu *cpu);
void handle_interrupts(struct cpu *cpu);
void push(struct cpu *cpu, unsigned char value);
unsigned char pop(struct cpu *cpu);
void push_state(struct cpu *cpu);
void pop_state(struct cpu *cpu);

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file)
{
  // printf("~CPU_LOAD: File path is: '%s'\n", file);
  FILE *file_d = fopen(file, "r");
  if (file_d == NULL)
  {
    fprintf(stderr, "ERROR loading the file %s\n", file);
    exit(1);
  }

  int ram_index = 0;
  // I get reference on how to read lines from a 'FILE pointer' from : https://stackoverflow.com/a/3501681/9355810
  char *line = NULL;
  size_t lenght = 0; // I do not understand what this variable does in 'getline()'
  ssize_t line_length;

  unsigned long binary_code;
  char *end_of_binary_number;

  while ((line_length = getline(&line, &lenght, file_d)) != -1)
  {
    // // printf("~Retrieved line of length %zu :\n", line_length);
    // // printf("~File line: %s", line);
    if (line_length > 8)
    {
      binary_code = strtoul(line, &end_of_binary_number, 2);
      // // printf("~Binary: %lu\n", binary_code);
      if (line != end_of_binary_number)
      {
        // // printf("~*line == *end_of_binary_number : %d\n\n", line == end_of_binary_number);
        // printf("~CPU_LOAD: **** Adding entry to RAM index: %d ****\n", ram_index);
        cpu->ram[ram_index++] = binary_code;
      }
    }
  }
  // printf("~ram_index after reading '%s': %d\n\n", file, ram_index);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  // printf("~\n\nALU function running\n");
  switch (op)
  {
  case ALU_MUL:
    // printf("~ALU_MUL: HANDLER FOUND\n");
    // printf("~regA = %d and regB = %d\n", regA, regB);
    cpu->reg[cpu->ram[cpu->PC + 1]] = regA * regB;
    // printf("~%d\n", cpu->reg[cpu->ram[cpu->PC + 1]]);
    // printf("~R%d, set to %d * %d = %d\n", cpu->ram[cpu->PC + 1], regA, regB, cpu->reg[cpu->ram[cpu->PC + 1]]);
    break;
  // TODO: implement more ALU ops
  case ALU_ADD:
    // printf("~ALU_ADD: HANDLER FOUND\n");
    // printf("~regA = %d and regB = %d\n", regA, regB);
    cpu->reg[cpu->ram[cpu->PC + 1]] = regA + regB;
    // printf("~Reg-index %d, set to %d + %d = %d\n", cpu->ram[cpu->PC + 1], regA, regB, cpu->reg[cpu->ram[cpu->PC + 1]]);
    break;
  case ALU_CMP:
    // printf("~ALU_CMP: HANDLER FOUND\n");
    if (regA < regB)
      cpu->FL = (cpu->FL & 0b0) | 4;
    if (regA > regB)
      cpu->FL = (cpu->FL & 0b0) | 2;
    if (regA == regB)
      cpu->FL = (cpu->FL & 0b0) | 1;
    // printf("~ALU_CMP: cpu->FL set to %d\n", cpu->FL);
    break;
  default:
    printf("ALU instruction handler was no found.");
    exit(2);
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  // printf("~CPU_RUN running\n");
  int running = 1; // True until we get a HLT instruction

  unsigned char *rgstr = cpu->reg;
  unsigned char *ram = cpu->ram;

  unsigned char PC = cpu->PC;
  unsigned char instruction_operands;
  cpu->IR = ram[PC];

  while (running)
  {
    cpu->tick = (cpu->tick + 1) % (unsigned long)(5 * pow(10, 7));
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    // // printf("~PC %d\nIRd %d\n", PC, cpu->IR);
    // // printf("~PC %d\nIRc %c\n", PC, cpu->IR);

    // printf("~\n\n>>>>>  TICK = %d\n", tick);

    /**
     * INTERRUPTS HANDLERS
     * 1. Check for IS bits, and set them.
     * 2. Check for IM, is any had been set -> call the interrupt_handler() function.
     */
    // Set timer-bit
    if (cpu->tick == 0) // If tick has make a whole round form 0 to 999.
    {
      cpu->reg[IS] = (cpu->reg[IS] | 0b1); // Set/activate the frist-bit.
    }
    // printf("~>>>>>  IM has been set? IM value = %d\n", cpu->reg[IM]);
    if (cpu->reg[IM] > 0 && cpu->interrupt_running == 0) // Check if the IM has been set && if there are no interrupts running.
    {
      // printf("~>>>>>  IM has been set? IM value = %d\n", cpu->reg[IM]);
      handle_interrupts(cpu); // After this fucntion ends execution, the execution continues where it left.
      // printf("Tick %d\n", cpu->tick);
    }

    /**
     * PROGRAM HANDLERS
     */
    handle_instruction(cpu);
    if (cpu->IR == '\0') // Exit after HLT handlers is executed.
    {
      break;
    }

    // printf("~>>>>>  IR is a PC_mutator? %d\n", (cpu->IR >> 4) & 1);

    if (((cpu->IR >> 4) & 1) == 0) // Chekc is the instruction is a PC_Mutator, if not jump to next instruction.
    {
      // printf("~IR = %d\n", cpu->IR);
      instruction_operands = (cpu->IR >> 6);
      // printf("~IR >> 6 = %d, instruction_operands %d\n", cpu->IR >> 6, instruction_operands);
      cpu->PC = cpu->PC + instruction_operands + 1;
    }
    // // printf("~cpu->PC next instruction index: %d\n", cpu->PC);
    PC = cpu->PC;
    // printf("~PC next instruction index: %d\n", PC);
    cpu->IR = ram[cpu->PC];
  }
  // printf("~\nCPU_RUN END\n");
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  cpu->IR = 0;
  cpu->MAR = 0;
  cpu->MDR = 0;
  cpu->FL = 0;
  cpu->interrupt_running = 0;
  cpu->tick = 0;

  // TODO: Zero registers and RAM
  memset(cpu->reg, 0, sizeof(cpu->reg));
  cpu->reg[SP] = 0xF3;
  // cpu->reg[IM] = 1; // Temporarily hardcoded until I implement the timer.
  memset(cpu->ram, 0, sizeof(cpu->ram));
}

/**
 * HELPERS
*/
void handle_instruction(struct cpu *cpu)
{
  // printf("~\n\n~CURRENT PC: %d\n", cpu->PC);
  // printf("~CURRENT IR: %d\n", cpu->IR);
  switch (cpu->IR)
  {
    /* ALU */
  case ADD:
    // printf("~ADD. HANDLER FOUND\n");
    alu(cpu, ALU_ADD, cpu->reg[cpu->ram[cpu->PC + 1]], cpu->reg[cpu->ram[cpu->PC + 2]]);
    break;
  // case SUB:
  case MUL:
    // printf("~MUL. HANDLER FOUND\n");
    alu(cpu, ALU_MUL, cpu->reg[cpu->ram[cpu->PC + 1]], cpu->reg[cpu->ram[cpu->PC + 2]]);
    break;
  // case DIV:
  // case MOD:
  // case INC:
  // case DEC:
  case CMP: // **
    // printf("~CMP. HANDLER FOUND\n");
    alu(cpu, ALU_CMP, cpu->reg[cpu->ram[cpu->PC + 1]], cpu->reg[cpu->ram[cpu->PC + 2]]);
    break;
  // case AND:
  // case NOT:
  // case OR:
  // case XOR:
  // case SHL:
  // case SHR:
  /* PC mutators */
  case CALL:
    // printf("~CALL. HANDLER FOUND\n");
    cpu->reg[SP] -= 1;
    // printf("~SP move from %d to %d\n", cpu->reg[SP] + 1, cpu->reg[SP]);
    cpu->ram[cpu->reg[SP]] = cpu->PC + (cpu->IR >> 6) + 1;
    // printf("~CALL. Set RAM[SP] = %d\n", cpu->ram[cpu->reg[SP]]);
    cpu->PC = cpu->reg[cpu->ram[cpu->PC + 1]];
    // printf("~CALL. PC set to %d\n", cpu->PC);
    break;
  case RET:
    // printf("~RET. HANDLER FOUND\n");
    cpu->PC = cpu->ram[cpu->reg[SP]];
    // printf("~Set PC to %d", cpu->PC);
    cpu->reg[SP] += 1;
    // printf("~SP move from %d to %d\n", cpu->reg[SP] - 1, cpu->reg[SP]);
    break;
  // case INT:
  case IRET:
    // printf("~IRET. HANDLER FOUND\n");
    pop_state(cpu);
    cpu->interrupt_running = 0;
    printf("tick %lu\n\n", cpu->tick);
    break;
  case JMP:
    // printf("~JMP. HANDLER FOUND\n");
    cpu->PC = cpu->reg[cpu->ram[cpu->PC + 1]];
    // printf("~PC set to %d\n", cpu->PC);
    break;
  case JEQ: // **
    // printf("~JEQ. HANDLER FOUND\n");
    if ((cpu->FL & 0b1) == 1)
    {
      cpu->PC = cpu->reg[cpu->ram[cpu->PC + 1]];
      // printf("~cpu->PC set to %d", cpu->PC);
    }
    else
    {
      cpu->PC = cpu->PC + 2;
    }
    break;
  case JNE: // **
    // printf("~JNE. HANDLER FOUND\n");
    if ((cpu->FL & 0b1) == 0)
    {
      cpu->PC = cpu->reg[cpu->ram[cpu->PC + 1]];
      // printf("~cpu->PC set to %d", cpu->PC);
    }
    else
    {
      cpu->PC = cpu->PC + 2;
    }
    break;
  // case JGT:
  // case JLT:
  // case JLE:
  // case JGE:
  /* Other */
  // case NOP:
  case HLT:
    printf("~HLT. HANDLER FOUND\n");
    cpu->IR = '\0';
    break;
  case LDI:
    // printf("~LDI. HANDLER FOUND\n");
    cpu->reg[cpu->ram[cpu->PC + 1]] = cpu->ram[cpu->PC + 2];
    // printf("~LDI. number set to: %d, in R%d\n", cpu->reg[cpu->ram[cpu->PC + 1]], cpu->ram[cpu->PC + 1]);
    break;
  // case LD:
  case ST:
    // printf("~ST. HANDLER FOUND\n");
    cpu->ram[cpu->reg[cpu->ram[cpu->PC + 1]]] = cpu->reg[cpu->ram[cpu->PC + 2]];
    // printf("~Set ram[%d] = (reg[%d] value: %d)\n", cpu->reg[cpu->ram[cpu->PC + 1]], cpu->PC + 2, cpu->reg[cpu->ram[cpu->PC + 2]]);
    break;
  case PUSH:
    // printf("~PUSH. HANDLER FOUND\n");
    push(cpu, cpu->reg[cpu->ram[cpu->PC + 1]]);
    break;
  case POP:
    // printf("~POP. HANDLER FOUND\n");
    cpu->reg[cpu->ram[cpu->PC + 1]] = pop(cpu);
    break;
  case PRN:
    // printf("~PRN. HANDLER FOUND\n");
    printf("%d\n", cpu->reg[cpu->ram[cpu->PC + 1]]);
    // printf("~PRN. PRINTED Number %d in R%d\n", cpu->reg[cpu->ram[cpu->PC + 1]], cpu->ram[cpu->PC + 1]);
    break;
  case PRA:
    // printf("~PRA. HANDLER FOUND\n");
    printf("%c\n", cpu->reg[cpu->ram[cpu->PC + 1]]);
    break;
  default:
    printf("~The IR %d-instruction has no handler\n", cpu->IR);
    exit(2);
  }
};

void handle_interrupts(struct cpu *cpu)
{
  // printf("~\n\nINTERRUPT HANDLER function running\n");
  unsigned char IS_aux, IM_aux;

  // Loop thorugh each bit and check if that bit is active in both IS and IM, if so -> handle that specific I-vector
  for (int i = 0; i < WORD /* our machine handle 8 bit WORDS */; i++)
  {
    IS_aux = (cpu->reg[IS] >> i) & 1; // Get the i-th bit.
    IM_aux = (cpu->reg[IM] >> i) & 1; // Get the i-th bit.
    // printf("~i = %d - IS_aux = %d, IM_aux = %d\n", i, IS_aux, IM_aux);
    if ((IS_aux & IM_aux) == 1)
    {
      // // printf("~\n\nINTERRUPT HANLDER: I%d-vector handler will be called.\n\n", i);
      push_state(cpu); // If we are at this level we have to save the exec-state before handle the Interrupt.
      cpu->interrupt_running = 1;

      switch (i) // Handle the i-th bit.
      {
      case 0:
        // printf("~I0. HANDLER FOUND\n");
        // printf("~I0 = %d, %X\n", I0, I0);
        // printf("~cpu->ram[I0] = %d\n", cpu->ram[I0]);
        cpu->PC = cpu->ram[I0];
        // printf("~PC set to %d\n", cpu->PC);
        cpu->IR = cpu->ram[cpu->PC];
        // printf("~IR set to %d\n", cpu->IR);
        cpu->reg[IS] = cpu->reg[IS] & 0b11111110; // Set the i-th bit to cero.
        return;
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
      default:
        // printf("~The IS %d-bit has no handler.\n", i);
        exit(2);
      }
    }
  }
}

void push(struct cpu *cpu, unsigned char value)
{
  // // printf("~\n\nPUSH function running\n");
  // // printf("~cpu->reg[SP] = %d\n", cpu->reg[SP]);
  cpu->reg[SP] -= 1;
  // // printf("~cpu->reg[SP] = %d\n", cpu->reg[SP]);
  // // printf("~SP move form %d to %d\n", cpu->reg[SP] + 1, cpu->reg[SP]);
  cpu->ram[cpu->reg[SP]] = value;
  // // printf("~Copied value %d form REG[%d] to RAM[%d]\n", cpu->ram[cpu->reg[SP]], value, cpu->reg[SP]);
}
unsigned char pop(struct cpu *cpu)
{
  // // printf("~\n\nPOP function running\n");
  unsigned char value = cpu->ram[cpu->reg[SP]];
  // // printf("~cpu->reg[SP] = %d\n", cpu->reg[SP]);
  // // printf("~Copied value %d form RAM[%d] to REG[%d]\n", cpu->ram[cpu->reg[SP]], cpu->reg[SP], cpu->ram[cpu->PC + 1]);
  cpu->reg[SP] += 1;
  // // printf("~cpu->reg[SP] = %d\n", cpu->reg[SP]);
  // // printf("~SP move from %d to %d\n", cpu->reg[SP] - 1, cpu->reg[SP]);
  return value;
}
void push_state(struct cpu *cpu)
{
  // printf("~\n\nPUSH_STATE function running\n");

  push(cpu, cpu->PC); // Push(PC)
  // // printf("~cpu->PC PUSHED TO STACK");

  push(cpu, cpu->IR); // Push(IR)
  // // printf("~cpu->IR PUSHED TO STACK");

  push(cpu, cpu->MAR); // Push(MAR)
  // // printf("~cpu->MAR PUSHED TO STACK");

  push(cpu, cpu->MDR); // Push(MDR)
  // // printf("~cpu->MDR PUSHED TO STACK");

  push(cpu, cpu->FL); // Push(FL)
  // // printf("~cpu->FL PUSHED TO STACK");

  for (int i = 0; i < 7; i++) // Push Ro -> R6
  {
    push(cpu, cpu->reg[i]);
    // // printf("~cpu->reg[%d] PUSHED TO STACK - R%d = %d\n", i, i, cpu->reg[i]);
  }
}
void pop_state(struct cpu *cpu)
{
  // printf("~\n\nPOP_STATE function running\n");
  for (int i = 6; i >= 0; --i) //Pop R6 -> R0
  {
    cpu->reg[i] = pop(cpu);
    // // printf("~cpu->reg[%d] POPED FROM STACK - R%d = %d\n", i, i, cpu->reg[i]);
  }

  cpu->FL = pop(cpu); // Push(FL)
  // // printf("~cpu->FL POPED FROM STACK");

  cpu->MDR = pop(cpu); // Push(MDR)
  // // printf("~cpu->MD POPED FROM STACK");

  cpu->MAR = pop(cpu); // Push(MAR)
  // // printf("~cpu->MA POPED FROM STACK");

  cpu->IR = pop(cpu); // Push(IR)
  // // printf("~cpu->IR POPED FROM STACK");

  cpu->PC = pop(cpu); // Push(PC)
  // // printf("~cpu->PC POPED FROM STACK");
}