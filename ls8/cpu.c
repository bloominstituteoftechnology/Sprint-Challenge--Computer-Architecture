#include "cpu.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define ADDR_EMPTY_STACK 0xF4
#define ADDR_PROGRAM_ENTRY 0x00


unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char val, unsigned char index){
  cpu->ram[index] = val;
}

void push_handler(struct cpu *cpu, unsigned char val)
{
    cpu_ram_write(cpu,  val, --cpu->SP);
}

unsigned char pop_handler(struct cpu *cpu){
  unsigned char val = cpu_ram_read(cpu, cpu->SP++);

  return val;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(stderr, "Missing arguments. Provide: ./ls8 filename");
    exit(1);
  }

  FILE *fp;
  char line [1024];
  char *file = argv[1];

  int address = ADDR_PROGRAM_ENTRY;

  if ((fp = fopen(file, "r")) == NULL) {
    fprintf(stderr, "Error: unable to open file %s\n", file);
    exit(1);
  }

  while (fgets(line, sizeof(line), fp) != NULL){

    char *ptr;
    unsigned char instruction = strtol(line, &ptr, 2);

    if (ptr == line) {
      continue;
    }

    cpu->ram[++address] = instruction;
  }
  //   for (int j = 0; j<89; j++){
  //   printf("Line %d: %d\n", j+7, cpu->ram[j]);
  // }
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *reg = cpu->reg;
  unsigned char valB = reg[regB];
  switch (op) {
    case ALU_MUL:
      reg[regA] *= valB;
      break;

    case ALU_ADD:
      // printf("alu_add\n");
      reg[regA] += valB;
      break;

    case ALU_CMP:
      if (regA == regB)     { cpu->FL = 0b00000001; }
      else if (regA > regB) { cpu->FL = 0b00000010; }
      else if (regA < regB) { cpu->FL = 0b00000100; }
      // printf("Flag: %d\n", cpu->FL);
      break;
      // if (regA == regB){ cpu->FL = 1 }
      // else if (regA > regB) {cpu->FL = 2}
      // else if (regA < regB) {cpu->FL = 4}
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu )
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char instruction = cpu->ram[cpu->PC];
    // 2. Figure out how many operands this next instruction requires
    unsigned char num_operands = instruction >> 6;
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operandA;
    unsigned char operandB;
    if (num_operands == 2) {
      operandA = cpu_ram_read(cpu, (cpu->PC + 1) & 0xff);
      operandB = cpu_ram_read(cpu, (cpu->PC + 2) & 0xff);
    } else if (num_operands == 1) {
      operandA = cpu->ram[cpu->PC + 1];
    }

    
    // printf("\nSP: %4d, PC: %4d\n", cpu->SP, cpu->PC);
    // printf("memory slots~~~      ");
    // printf("%4d %4d %4d %4d %4d %4d\n", cpu->ram[246], cpu->ram[245], cpu->ram[244], cpu->ram[243], cpu->ram[242], cpu->ram[241]);
    // printf("register slots~~~      ");
    // printf("%4d %4d %4d %4d %4d %4d\n", cpu->reg[0], cpu->reg[1], cpu->reg[2], cpu->reg[3], cpu->reg[4], cpu->reg[5]);
    // printf("instruction: %d\n", instruction);

    switch (instruction) {
      case HLT:
        running = 0;
        break;

      case PRN:
      // printf("print\n");

        printf("%d\n", cpu->reg[operandA]);
        break;

      case LDI:
      // printf("ldi\n");
        cpu->reg[operandA] = operandB;
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;

      case ADD:
      // printf("add\n");
        alu(cpu, ALU_ADD, operandA, operandB);
        break;

      case POP:
        cpu->reg[operandA] = pop_handler(cpu);
        break;

      case PUSH:
        push_handler(cpu, cpu->reg[operandA]);
        break;

      case CALL:
      // printf("call\n");
        push_handler(cpu, cpu->PC + 1);
        cpu->PC = cpu->reg[operandA];
        break;

      case RET:
      // printf("ret\n");
        cpu->PC = pop_handler(cpu);
        break;

      case CMP:
        alu(cpu, ALU_CMP, cpu->reg[operandA], cpu->reg[operandB]);
        break;

      case JMP:
        cpu->PC = cpu->reg[operandA]-num_operands-1;
        break;

      case JEQ:
        if (cpu->FL == 0b00000001) {
          cpu->PC=cpu->reg[operandA]-num_operands-1;
        }
        break;

      case JNE:
        if ((cpu->FL & 0b00000001) == 0) {
          cpu->PC=cpu->reg[operandA]-num_operands-1;
        }
        break;

      default:
        break;
    }
    // 6. Move the PC to the next instruction.
    cpu->PC += num_operands + 1;
  }
  // unsigned char test = 4;
  // printf("%d\n", test & 0b00000001);
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  cpu->FL = 0;
  cpu->SP = ADDR_EMPTY_STACK;
  memset(cpu->reg, 0, sizeof(cpu->reg[0]));
  memset(cpu->ram, 0, sizeof(cpu->ram[0]));
}


