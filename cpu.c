#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "cpu.h"
#include "cpu_instr.h"

/**
 * Performs operations for ALU -> TODO: implement more ALU ops
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param op {enum alu_op} ALU operation enumerator value.
 * @param regA {unsigned char} Value stored in register A.
 * @param regB {unsigned char} Value stored in register B.
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char a = cpu->registers[regA];
  unsigned char b = cpu->registers[regB];

  switch (op)
  {
  case ALU_ADD:
    cpu->registers[regA] = a + b;
    break;
  case ALU_AND:
    cpu->registers[regA] = a & b;
    break;
  case ALU_CMP:
    if (a == b)
      cpu->fl = E;
    if (a < b)
      cpu->fl = L;
    if (a > b)
      cpu->fl = G;
    break;
  case ALU_DEC:
    cpu->registers[regA] = a--;
    break;
  case ALU_DIV:
    cpu->registers[regA] = a / b;
    break;
  case ALU_INC:
    cpu->registers[regA] = a++;
    break;
  case ALU_MOD:
    cpu->registers[regA] = a % b;
    break;
  case ALU_MUL:
    cpu->registers[regA] = a * b;
    break;
  case ALU_NOT:
    cpu->registers[regA] = ~a;
    break;
  case ALU_OR:
    cpu->registers[regA] = a | b;
    break;
  case ALU_SHL:
    cpu->registers[regA] = a << b;
    break;
  case ALU_SHR:
    cpu->registers[regA] = a >> b;
    break;
  case ALU_SUB:
    cpu->registers[regA] = a - b;
    break;
  case ALU_XOR:
    cpu->registers[regA] = a ^ b;
    break;
  }
}

/**
 * Reads from RAM address in MAR and stores value in MDR
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 */
void cpu_ram_read(struct cpu *cpu)
{
  cpu->mdr = cpu->ram[cpu->mar];
}

/**
 * Writes value in MDR to RAM address in MAR
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 */
void cpu_ram_write(struct cpu *cpu)
{
  cpu->ram[cpu->mar] = cpu->mdr;
}

/**
 * Sets instruction register to the next instruction given by PC
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 */
void get_next_instruction(struct cpu *cpu)
{
  cpu->mar = cpu->pc;
  cpu_ram_read(cpu);
  cpu->ir = cpu->mdr;
}

/**
 * Checks if current instruction is a PC mutator
 * PC mutators will set the PC, removing the need to increment PC
 * 
 * @param instr {unsigned char} Current instruction.
 */
int isPCMutator(unsigned char instr)
{
  instr <<= 3;
  return (instr >>= 7) == 1;
}

/**
 * Sets given operand to value at memory address
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 */
void set_operand(struct cpu *cpu, unsigned char address, unsigned char *op)
{
  cpu->mar = address;
  cpu_ram_read(cpu);
  *op = cpu->mdr;
}

/**
 * Gets number of operands in instruction, sets them to variables and updates PC
 * 
 * @param opA {unsigned char*} Pointer to operandA variable.
 * @param opB {unsigned char*} Pointer to operandB variable.
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 */
void set_operands(unsigned char *opA, unsigned char *opB, struct cpu *cpu)
{
  unsigned char ops = cpu->ir >> 6;
  *opA = '\0';
  *opB = '\0';

  if (ops > 0)
    set_operand(cpu, ++cpu->pc, opA);

  if (ops > 1)
    set_operand(cpu, ++cpu->pc, opB);
}

/**
 * If at least one second has passed, set bit 0 of the IS register
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param timer {float*} Pointer to timer.
 * @param tv {struct timevel*} Pointer to time timer was set to 0.
 */
void timer_interrupt(struct cpu *cpu, float *timer, struct timeval *tv)
{
  struct timeval ct;
  gettimeofday(&ct, NULL);
  *timer = (ct.tv_sec - tv->tv_sec) + ((ct.tv_usec - tv->tv_usec) * 0.000001);

  if (*timer >= 1)
  {
    cpu->registers[IS] = cpu->registers[IS] | (1 << 0);
    *timer = 0;
    gettimeofday(tv, NULL);
  }
}

/**
 * Checks if an interrupt the cpu is interested in has triggered
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param i {unsigned char*} Pointer to interrupts variable.
 */
void check_interrupts(struct cpu *cpu, unsigned char *i)
{
  *i = cpu->registers[IM] & cpu->registers[IS];
}

/**
 * Disables further interrupts, saves cpu state and updates pc based on
 * interrupt vector table
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param i {int} Interrupt number (I0-I7).
 */
void process_interrupt(struct cpu *cpu, int i)
{
  // Disable further interrupts
  handle_PUSH(cpu, IM, '\0');
  cpu->registers[IM] = 0b00000000;

  // Clear IS bit
  cpu->registers[IS] = cpu->registers[IS] & ~(1 << i);

  // Save CPU state
  cpu->registers[TMP] = cpu->pc;
  handle_PUSH(cpu, TMP, '\0');
  cpu->registers[TMP] = cpu->fl;
  handle_PUSH(cpu, TMP, '\0');
  handle_PUSH(cpu, 0, '\0');
  handle_PUSH(cpu, 1, '\0');
  handle_PUSH(cpu, 2, '\0');
  handle_PUSH(cpu, 3, '\0');
  handle_PUSH(cpu, 4, '\0');
  handle_PUSH(cpu, 5, '\0');
  handle_PUSH(cpu, 6, '\0');

  // Set PC to address in interrupt vector table
  cpu->mar = IVT + i;
  cpu_ram_read(cpu);
  cpu->pc = cpu->mdr;
}

/**
 * Initialize a CPU struct
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 */
void cpu_init(struct cpu *cpu)
{
  memset(cpu->registers, 0, sizeof cpu->registers);
  memset(cpu->ram, 0, sizeof cpu->ram);

  cpu->pc = 0b00000000;
  cpu->ir = 0b00000000;
  cpu->fl = 0b00000000;
  cpu->registers[SP] = 0xF4;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param program {char*} Pointer to program path.
 */
void cpu_load(struct cpu *cpu, char *program)
{
  FILE *fp = fopen(program, "r");
  char *line = NULL;
  size_t len = 9;
  int data_len = 0;
  int address = 0;
  unsigned char data[256];

  if (fp == NULL)
  {
    printf("error: could not open program %s", program);
    exit(1);
  }

  while (getline(&line, &len, fp) != -1)
  {
    if (line[0] != '#')
      data[data_len++] = strtoul(line, NULL, 2);
  }

  for (int i = 0; i < data_len; i++)
    cpu->ram[address++] = data[i];

  free(line);
  fclose(fp);
}

/**
 * Run the CPU
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 */
void cpu_run(struct cpu *cpu)
{
  int pcMutator = 0;
  float timer = 0;
  unsigned char opA = '\0';
  unsigned char opB = '\0';
  unsigned char interrupts = '\0';
  struct timeval cpu_time;

  handler *branch_table = malloc(256 * sizeof *branch_table);
  load_cpu_instructions(branch_table);
  gettimeofday(&cpu_time, NULL);

  while (1)
  {
    timer_interrupt(cpu, &timer, &cpu_time);
    check_interrupts(cpu, &interrupts);

    if (interrupts)
      for (int i = 0; i < 8; i++)
        if (((interrupts >> i) & 1) == 1)
          process_interrupt(cpu, i);

    get_next_instruction(cpu);

    if (cpu->ir == HLT)
      break;

    pcMutator = isPCMutator(cpu->ir);
    set_operands(&opA, &opB, cpu);
    branch_table[cpu->ir](cpu, opA, opB);

    if (!pcMutator)
      cpu->pc++;
  }

  free(branch_table);
}