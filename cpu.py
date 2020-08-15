"""CPU functionality."""

import sys


class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.ram = [0] * 256
        self.reg = [0] * 8
        self.sp = 7
        self.reg[self.sp] = 0xF4
        self.pc = 0
        self.ir = 0
        self.fl = 6
        self.op_table = {
            0b10000010: self.ldi,
            0b01000111: self.prn,
            0b10100010: self.mult,
            0b00000001: self.hlt,
            0b01000101: self.push,
            0b01000110: self.pop,
            0b01010000: self.call,
            0b00010001: self.ret,
            0b10100000: self.add,
            0b01010110: self.jne,
            0b01010100: self.jmp,
            0b01010101: self.jeq,
            0b10100111: self.cmpp}

    def load(self):
        """Load a program into memory."""

        address = 0

        file = open('sctest.ls8',  "r")
        for line in file.readlines():
            try:
                x = line[:line.index("#")]
            except ValueError:
                x = line

            try:
                y = int(x, 2)
                self.ram[address] = y
            except ValueError:
                continue
            address += 1

    def ldi(self, op_a, op_b):
        self.reg[op_a] = op_b

    def mult(self, op_a, op_b):
        self.reg[op_a] = self.reg[op_a] * self.reg[op_b]

    # Push the value in the given register on the stack.
    def push(self, op_a, op_b):
        self.reg[self.sp] -= 1
        self.ram[self.reg[self.sp]] = self.reg[op_a]

    # Pop the value at the top of the stack into the given register.
    def pop(self, op_a, op_b):
        self.reg[op_a] = self.ram[self.reg[self.sp]]
        self.reg[self.sp] += 1

    # Sets the PC to the register value
    def call(self, op_a, op_b):
        self.reg[self.sp] -= 1
        self.ram[self.reg[self.sp]] = self.pc + 2
        self.pc = self.reg[op_a]
        return True

    def ret(self, op_a, op_b):
        self.pop(op_a, 0)
        self.pc = self.reg[op_a]
        return True

    def add(self, op_a, op_b):
        self.reg[op_a] = self.reg[op_a] + self.reg[op_b]

    def cmpp(self, op_a, op_b):
        value_a = self.reg[op_a]
        value_b = self.reg[op_b]

        if value_a == value_b:
            self.reg[self.fl] = 1
        elif value_a > value_b:
            self.reg[self.fl] = 2
        else:
            self.reg[self.fl] = 4

    def jne(self, op_a, op_b):
        value = self.reg[self.fl]
        if value == 2 or value == 4:
            return self.jmp(op_a, 0)

    def jmp(self, op_a, op_b):
        self.pc = self.reg[op_a]
        return True

    def jeq(self, op_a, op_b):
        if self.reg[self.fl] == 1:
            return self.jmp(op_a, 0)

    def ram_read(self, address):
        return self.ram[address]

    def ram_write(self, value, address):
        self.ram[address] = value

    def prn(self, op_a, op_b):
        print(self.reg[op_a])

    def hlt(self, op_a, op_b):
        sys.exit()

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""
        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        else:
            raise Exception("Unsupported ALU operation")

    def trace(self):
        """
        Handy function to print out the CPU state. You might want to call this
        from run() if you need help debugging.
        """
        print(f"TRACE: %02X | %02X %02X %02X |" % (
            self.pc,
            # self.fl,
            # self.ie,
            self.ram_read(self.pc),
            self.ram_read(self.pc + 1),
            self.ram_read(self.pc + 2)
        ), end='')

        for i in range(8):
            print(" %02X" % self.reg[i], end='')

        print()

    def run(self):
        """Run the CPU."""
        while True:
            self.ir = self.ram[self.pc]
            op_a = self.ram[self.pc + 1]
            op_b = self.ram[self.pc + 2]

            willJump = self.op_table[self.ir](op_a, op_b)
            if not willJump:
                self.pc += (self.ir >> 6) + 1
