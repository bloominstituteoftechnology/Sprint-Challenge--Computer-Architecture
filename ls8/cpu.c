#include "cpu.h"

void cpu_ram_write(struct cpu *cpu, unsigned char value, unsigned char address)
{
  cpu->ram[address] = value;
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };

  FILE *fp = fopen(filename, "r");
  char buff[255];
  int index = 0;
  unsigned char data[256];
  int data_index = 0;
  int found_something = 0;
  for(int curr=fgetc(fp); !feof(fp); curr=fgetc(fp)){
    if(curr == ' '){
      continue;
    }
    if(curr=='\n' || curr=='#'){
      if(!found_something){
        continue;
      }
      buff[index] = '\0';
      index = 0;
      data[data_index++] = strtoul(buff, 0, 2);
      if(curr == '#'){
        while(curr != '\n'){
          curr = fgetc(fp);
        }
      }
      found_something = 0;
      continue;
    }
    buff[index++] = curr;
    found_something = 1;
  }
  fclose(fp);

  int address = 0;

  for (int i = 0; i < data_index; i++) {
    cpu->ram[address++] = data[i];
  }
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB];
      break;

    // TODO: implement more ALU ops
    case ALU_ADD:
      cpu->registers[regA] = cpu->registers[regA] + cpu->registers[regB];
      break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  cpu->registers[7] = 0b11110100;
  // int number = 1;
  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu->ram[cpu->PC];
    unsigned char FL = 0;
    // 2. Figure out how many operands this next instruction requires
    unsigned int amount_of_operands = IR >> 6;
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operandA = 0;
    unsigned char operandB = 0;
    if(amount_of_operands > 0){
      operandA = cpu->ram[cpu->PC+1];
    }
    if(amount_of_operands == 2){
      operandB = cpu->ram[cpu->PC+2];
    }
    // 4. switch() over it to decide on a course of action.
    int increment_PC = 1;
    switch(IR){
      case HLT:
        running = 0;
        break;
      case LDI:
        cpu->registers[operandA] = operandB;
        break;
      case PRN:
        printf("%d\n", cpu->registers[operandA]);
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;
      case PUSH:
        cpu->ram[--cpu->registers[7]] = cpu->registers[operandA];
        break;
      case POP:
        cpu->registers[operandA] = cpu->ram[cpu->registers[7]++];
        break;
      case CALL:
        cpu->ram[--cpu->registers[7]] = cpu->PC + 2;
        cpu->PC = cpu->registers[operandA];
        increment_PC = 0;
        break;
      case RET:
        cpu->PC = cpu->ram[cpu->registers[7]++];
        increment_PC = 0;
        break;
      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        break;
      case ST:
        cpu->ram[cpu->registers[operandA]] = cpu->registers[operandB];
        break;
      case JMP:
        cpu->PC = cpu->registers[operandA];
        increment_PC = 0;
        break;
      case PRA:
        printf("%c\n", cpu->registers[operandA]);
        break;
      case IRET:
        for(int i=6; i>=0; i--){
          cpu->registers[i] = cpu->ram[cpu->registers[7]++];
        }
        break;
      case CMP:
        FL = 0;
        if(cpu->registers[operandA] == cpu->registers[operandB]){
          FL = FL | 1;
        }
        if(cpu->registers[operandA] > cpu->registers[operandB]){
          FL = FL | 0b00000010;
        }
        if(cpu->registers[operandA] < cpu->registers[operandB]){
          FL = FL | 0b00000100;
        }
        break;
      default:
        printf("Instruction $d not implemented.\n", IR);
        break;
    }
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    if(increment_PC){
      cpu->PC = cpu->PC+1+amount_of_operands;
    }else{
      increment_PC = 1;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu = malloc(sizeof(cpu));
  cpu->PC = 0;
  cpu->registers[7] = 0b11110011;
  memset(cpu->registers, 0, 8);
  memset(cpu->ram, 0, 256);
}
