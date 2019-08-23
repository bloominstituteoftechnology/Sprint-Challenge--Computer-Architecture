"""CPU functionality."""

import sys


class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.ram = [0] * 256
        self.reg = [0] * 8
        self.pc = 0
        self.stack_pointer = 4
        self.reg[self.stack_pointer] = 0xf4
        self.fl_L = 0b00000000
        self.fl_G = 0b00000000
        self.fl_E = 0b00000000

    def ram_read(self, address):
        return self.ram[address]

    def ram_write(self, address, value):
        self.ram[address] = value

    def push_val(self, val):
        self.reg[self.stack_pointer] -= 1
        self.ram_write(val, self.reg[7])

    def pop_val(self):
        val = self.ram_read(self.reg[7])
        self.reg[self.stack_pointer] += 1
        return val

    def load(self, filename):
        address = 0
        with open(filename) as fp:
            for line in fp:
                comment_split = line.split("#")
                num = comment_split[0].strip()
                if num == '':  # ignore blanks
                    continue
                val = int(num, 2)
                self.ram[address] = val
                address += 1

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        # elif op == "SUB": etc
        elif op == "MUL":
            self.reg[reg_a] *= self.reg[reg_b]
        elif op == "CMP":
            # print("CMP HIT")
            if self.reg[reg_a] == self.reg[reg_b]:
                self.fl_E = 0b00000001
                # print("CMP E")
            elif self.reg[reg_a] > self.reg[reg_b]:
                # print("CMP G")
                self.fl_G = 0b00000001
            elif self.reg[reg_a] < self.reg[reg_b]:
                # print("CMP L")
                self.fl_L = 0b00000001
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
        # ir = None
        halt = 0b00000001
        ldi = 0b10000010
        prn = 0b01000111
        mul = 0b10100010
        cmp = 0b10100111
        jmp = 0b01010100
        jeq = 0b01010101
        jne = 0b01010110
        while self.ram[self.pc] != halt:
            if self.ram[self.pc] == ldi:
                operand_a = self.ram_read(self.pc + 1)
                operand_b = self.ram_read(self.pc + 2)
                self.reg[operand_a] = operand_b
                self.pc += 3
            elif self.ram[self.pc] == prn:
                # print("PRN")
                print(self.reg[self.ram[self.pc + 1]])
                self.pc += 2
            elif self.ram[self.pc] == mul:
                operand_a = self.ram_read(self.pc + 1)
                operand_b = self.ram_read(self.pc + 2)
                self.alu("MUL", operand_a, operand_b)
                self.pc += 3
            elif self.ram[self.pc] == cmp:
                operand_a = self.ram_read(self.pc + 1)
                operand_b = self.ram_read(self.pc + 2)
                self.alu("CMP", operand_a, operand_b)
                self.pc += 3
            elif self.ram[self.pc] == jmp:
                address = self.ram_read(self.pc + 1)
                self.pc = self.reg[address]
            elif self.ram[self.pc] == jeq:
                # print("JEQ")
                if self.fl_E == 0b00000001:
                    address = self.ram_read(self.pc + 1)
                    self.pc = self.reg[address]
                else:
                    # print("ELIF")
                    # print(self.pc)
                    self.pc += 2
            elif self.ram[self.pc] == jne:
                # print("JNE")
                if self.fl_E == 0b00000000:
                    address = self.ram_read(self.pc + 1)
                    self.pc = self.reg[address]
                else:
                    # print("ELIF")
                    # print(self.pc)
                    self.pc += 2
