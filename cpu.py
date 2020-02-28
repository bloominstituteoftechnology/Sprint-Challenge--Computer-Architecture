"""CPU functionality."""

import sys

HLT = 0b00000001
LDI = 0b10000010
PRN = 0b01000111
MUL = 0b10100010
CMP = 0b10100111
JMP = 0b01010100
JEQ = 0b01010101
JNE = 0b01010110

class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.ram = [0] * 256
        self.reg = [0] * 8
        self.pc = 0b00000000
        self.fl = 0b00000000
        self.filename = sys.argv[1]

    def load(self):
        """Load a program into memory."""
        address = 0
        try:
            with open(self.filename) as f:
                for line in f:
                    comment_split = line.strip().split('#')
                    value = comment_split[0].strip()
                    if value == '':
                        continue
                    num = int(value, 2)
                    # print(num)
                    self.ram[address] = f'{num:08b}'
                    address += 1
        except FileNotFoundError:
            print("File Not Found")
            sys.exit(2)

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        #elif op == "SUB": etc
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

    def ram_read(self, address):
        return self.ram[address]
    
    def ram_write(self, address, value):
        self.ram[address] = value
    
    def run(self):
        """Run the CPU."""
        while True:
            command = int(self.ram_read(self.pc),2)
            if command == HLT:
                sys.exit(0)
            elif command == LDI:
                register = self.ram_read(self.pc + 1)
                value = self.ram[self.pc + 2]
                # print(int(value,2))
                self.reg[int(register,2)] = value
                self.pc += 3
            elif command == PRN:
                register = self.ram[self.pc + 1]
                print(int(self.reg[int(register,2)], 2))
                self.pc += 2
            elif command == MUL:
                reg0 = self.ram[self.pc + 1]
                reg1 = self.ram[self.pc + 2]
                self.reg[reg0] = self.reg[reg0] * self.reg[reg1]
                self.pc += 3
            elif command == CMP:
                reg0 = int(self.ram[self.pc + 1],2)
                reg1 = int(self.ram[self.pc + 2],2)
                if self.reg[reg0] == self.reg[reg1]:
                    # print(self.fl & 0b00000001)
                    self.fl = self.fl & 0b00000001
                    # print(self.fl | 0b00000001)
                    self.fl = self.fl | 0b00000001
                if self.reg[reg0] > self.reg[reg1]:
                    # print(self.fl & 0b00000010)
                    self.fl = self.fl & 0b00000010
                    # print(self.fl | 0b00000010)
                    self.fl = self.fl | 0b00000010
                if self.reg[reg0] < self.reg[reg1]:
                    # print(self.fl & 0b00000100)
                    self.fl = self.fl & 0b00000100
                    # print(self.fl | 0b00000100)
                    self.fl = self.fl | 0b00000100
                self.pc += 3
            elif command == JMP:
                self.pc = int(self.reg[int(self.ram[self.pc + 1], 2)], 2)
            elif command == JEQ:
                # print(int(self.reg[int(self.ram[self.pc + 1], 2)], 2))
                # print(self.fl & 1)
                if self.fl & 1:
                    self.pc = int(self.reg[int(self.ram[self.pc + 1], 2)], 2)
                else:
                    self.pc += 2
            elif command == JNE:
                if not self.fl & 1:
                    self.pc = int(self.reg[int(self.ram[self.pc + 1], 2)], 2)
                else:
                    self.pc += 2

