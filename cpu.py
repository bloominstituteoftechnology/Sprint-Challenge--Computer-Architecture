"""CPU functionality."""

import sys

# Opcodes:
ADD = 0b10100000
CMP = 0b10100111
HLT = 0b00000001
JEQ = 0b01010101
JMP = 0b01010100
JNE = 0b01010110
LDI = 0b10000010
MUL = 0b10100010
PRN = 0b01000111

# stack pointer
SP = 7

class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.pc = 0  
        self.fl = 0 # default flag to 0

        self.halted = False

        self.should_advance = False # flag for if the program increments pc manually or not

        self.ram = [0] * 256

        self.reg = [0] * 8
        self.reg[SP] = 0xf4

        self.branch_table = { 
            ADD: self.op_add,
            CMP: self.op_cmp,
            HLT: self.op_hlt,
            JEQ: self.op_jeq,
            JMP: self.op_jmp,
            JNE: self.op_jne,
            LDI: self.op_ldi,
            MUL: self.op_mul,
            PRN: self.op_prn,
        }

    def load(self, program):
        """Load a program into memory."""

        address = 0

        program_list = []

        with open(program, "r") as program_file:
            for line in program_file:
                line = line.partition('#')[0]
                line = line.rstrip()
                if line:
                    program_list.append(int(line, 2))


        for instruction in program_list:
            self.ram[address] = instruction
            address += 1
    
    def ram_write(self, mdr, mar):
        self.ram[mar] = mdr

    def ram_read(self, mar):
        return self.ram[mar]

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        elif op == "MUL":
            self.reg[reg_a] *= self.reg[reg_b]
        elif op == "CMP":
            # if a < b, set L flag to 1
            if self.reg[reg_a] < self.reg[reg_b]:
                self.fl = 0b00000100
            # if a > b, set G flag to 1
            elif self.reg[reg_a] > self.reg[reg_b]:
                self.fl = 0b00000010
            # if equal set E flag 1
            elif self.reg[reg_a] == self.reg[reg_b]:
                self.fl = 0b00000001

    def run(self):
        """Run the CPU."""
        while not self.halted:

            ir = self.ram[self.pc]
            operand_a = self.ram_read(self.pc + 1)
            operand_b = self.ram_read(self.pc + 2)

            increment = (ir >> 6) + 1 # get the increment size from the end of the instruction
            self.should_advance = ((ir >> 4) & 0b1) == 1 # check if instruction sets pc manually

            if ir in self.branch_table:
                self.branch_table[ir](operand_a, operand_b)

            if not self.should_advance:
                self.pc += increment


    def op_prn(self, operand_a, operand_b):
        print(self.reg[operand_a])


    def op_ldi(self, operand_a, operand_b):
        self.reg[operand_a] = operand_b

    def op_add(self, operand_a, operand_b):
        self.alu("ADD", operand_a, operand_b)

    def op_mul(self, operand_a, operand_b):
        self.alu("MUL", operand_a, operand_b)

    def op_jmp(self, operand_a, operand_b):
        self.pc = self.reg[operand_a]

    def op_jeq(self, operand_a, operand_b):
        if self.fl == 1:
            self.pc = self.reg[operand_a]
        else:
            self.should_advance = False

    def op_jne(self, operand_a, operand_b):
        if not self.fl == 1:
            self.pc = self.reg[operand_a]
        else:
            self.should_advance = False

    def op_cmp(self, operand_a, operand_b):
        self.alu("CMP", operand_a, operand_b)

    def op_hlt(self, operand_a, operand_b):
        self.halted = True
