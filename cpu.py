"""CPU functionality."""

import sys

# `LDI` instruction handler
LDI = 130
# `HLT` instruction handler
HLT = 1
# `PRN` instruction handler
PRN = 71
# ADD
ADD = 160
# MUL
MUL = 162
# PUSH
PUSH = 69
# POP
POP = 70
# SP
SP = 7
# CALL
CALL = 80
# RET
RET = 17
# CMP
CMP = 167
# JMP
JMP = 84
# JEQ
JEQ = 85
# JNE
JNE = 86

class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        # Add list properties to the `CPU` class to hold 256 bytes of memory and 8
        # general-purpose registers.

        # > Hint: you can make a list of a certain number of zeros with this syntax:
        # >
        # > ```python
        # > x = [0] * 25  # x is a list of 25 zeroes
        # > ```

        # Also add properties for any internal registers you need, e.g. `PC`.

        self.ram = [0] * 256
        self.reg = [0] * 8
        self.pc = 0
        self.less = 0
        self.greater = 0
        self.equal = 0


    # Un-hardcode the machine code
    def load(self):
        """Load a program into memory."""
        try:
            file = sys.argv[1]
            address = 0
            with open(file) as f:
                for line in f:
                    line = line.split("#")[0]
                    line = line.strip()
                    if line == "":
                        continue

                    value = int(line, 2)

                    self.ram[address] = value
                    address += 1

        except FileNotFoundError:
            print("File not found")
            sys.exit(2)

    def ram_read(self, MAR):
        return self.ram[MAR]

    def ram_write(self, MDR, MAR):
        self.ram[MAR] = MDR

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        #elif op == "SUB": etc
        elif op == "MUL": # Multiply
            self.reg[reg_a] *= self.reg[reg_b]
        elif op == "CMP": # Compare
            if self.reg[reg_a] > self.reg[reg_b]:
                self.greater = 1
            elif self.reg[reg_a] < self.reg[reg_b]:
                self.less = 1
            elif self.reg[reg_a] == self.reg[reg_b]:
                self.equal = 1
        else:
            raise Exception("Unsupported ALU operation")

    def trace(self):
        """
        Handy function to print out the CPU state. You might want to call this
        from run() if you need help debugging.
        """

        print(f"TRACE: %02X | %02X %02X %02X |" % (
            self.pc,
            #self.fl,
            #self.ie,
            self.ram_read(self.pc),
            self.ram_read(self.pc + 1),
            self.ram_read(self.pc + 2)
        ), end='')

        for i in range(8):
            print(" %02X" % self.reg[i], end='')

        print()

    def run(self):
        """Run the CPU."""
        self.load()
        while True:
            IR = self.ram[self.pc]
            operand_a = self.ram[self.pc + 1]
            operand_b = self.ram[self.pc + 2]
            operand_c = IR >> 6
            sets_pc = IR >> 4 & 0b0001

            # LDI
            if IR == LDI:
                self.reg[operand_a] = operand_b
            # PRN
            elif IR == PRN:
                data = self.ram[self.pc + 1]
                print(self.reg[data])
            # MUL
            elif IR == MUL:
                reg_a = self.ram[self.pc + 1]
                reg_b = self.ram[self.pc + 2]
                self.reg[reg_a] *= self.reg[reg_b]
            # ADD
            elif IR == ADD:
                reg_a = self.ram[self.pc + 1]
                reg_b = self.ram[self.pc + 2]
                self.reg[reg_a] += self.reg[reg_b]
            # PUSH
            elif IR == PUSH:
                reg = self.ram[self.pc + 1]
                val = self.reg[reg]
                self.reg[SP] -= 1
                self.ram[self.reg[SP]] = val
            # POP
            elif IR == POP:
                reg = self.ram[self.pc + 1]
                val = self.ram[self.reg[SP]]
                self.reg[reg] = val
                self.reg[SP] += 1
            # CALL
            elif IR == CALL:
                self.reg[SP] -= 1
                self.ram[self.reg[SP]] = self.pc + 2
                reg = self.ram[self.pc + 1]
                self.pc = self.reg[reg]
            # RET
            elif IR == RET:
                self.pc = self.ram[self.reg[SP]]
                self.reg[SP] += 1
            elif IR == CMP:
                self.alu("CMP", operand_a, operand_b)
            # JMP(Jump)
            elif IR == JMP:
                self.pc = self.reg[operand_a]
            # JEQ
            elif IR == JEQ:
                if self.equal == 1:
                    self.pc = self.reg[operand_a]
                else:
                    self.pc += 2
            # JNE
            elif IR == JNE:
                if self.equal == 0:
                    self.pc = self.reg[operand_a]
                else:
                    self.pc += 2
            # HLT
            elif IR == HLT:
                sys.exit(0)
            else:
                print(f"Error, {IR}")
                sys.exit(1)
            if sets_pc == 0:
                self.pc += operand_c + 1
