"""CPU functionality."""

import sys
import re

class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        # general registers
        self.reg = bytearray([0]*7 + [0xf4]) # [00,00,00,00,00,00,00,f4]
        # memory
        self.ram = bytearray(256)
        # internal registers
        self.pc = 0 # Program Counter: address of the currently executing instruction
        self.mar = 0 # Memory Address Register: holds the memory address we're reading or writing
        self.mdr = 0 # Memory Data Register: holds the value to write or the value just read
        self.fl = 0 # Flags: 00000LGE; L: less than, G: Greater than, E: Equal
    
    def ram_read(self, address):
        """Accepts an address to read and returns the value stored there."""
        self.mar = address
        self.mdr = self.ram[self.mar]
        return self.mdr

    def raw_write(self, value, address):
        """Accepts a value and address to write the value to."""
        self.mdr = value
        self.mar = address
        self.ram[self.mar] = self.mdr

    def load(self, path):
        """Load a program into memory."""

        address = 0

        with open(path) as f:
            program = f.read()
            program = re.findall('^[01]{8}', program, re.MULTILINE)
            program = map(lambda code: int(code, 2), program)

        for instruction in program:
            self.ram[address] = instruction
            address += 1


    def alu(self, op, reg_a, reg_b):
        """ALU operations."""
        MUL = 0b0010

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        elif op == MUL:
            self.reg[reg_a] *= self.reg[reg_b]
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
        HLT = 0b0001
        LDI = 0b0010
        PRN = 0b0111
        """Run the CPU."""
        while True:
            ir = self.ram_read(self.pc) # Instruction Register: contains a copy of the currently executing instruction
            operand_a = self.ram_read(self.pc + 1)
            operand_b = self.ram_read(self.pc + 2)

            operands = (ir & 0b11000000) >> 6
            alu_oper = (ir & 0b00100000) >> 5
            sets_pc  = (ir & 0b00010000) >> 4
            instr_id = (ir & 0b00001111)

            if not sets_pc:
                self.pc += 1 + operands

            if alu_oper:
                self.alu(instr_id, operand_a, operand_b)
            
            elif instr_id == HLT:
                break

            elif instr_id == LDI:
                self.reg[operand_a] = operand_b

            elif instr_id == PRN:
                print(self.reg[operand_a])
