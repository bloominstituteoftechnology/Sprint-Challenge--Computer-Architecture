"""CPU functionality."""

import sys

class CPU:
    """Main CPU class."""

    def __init__(self):
        self.ram = [0] * 256
        self.reg = [0] * 8
        self.pc = 0
        self.SP = 0

        self.reg[7] = 0xF3

    def ram_read(self, MAR):
        return self.ram[MAR]

    def ram_write(self, MAR, MDR):
        self.ram[MAR] = MDR

    def load(self):
        address = 0
        
        try:
            with open(sys.argv[1]) as file:
                for line in file:
                    if line[0].startswith('0') or line[0].startswith('1'):
                        num = line.split('#')[0]
                        num = num.strip()
                        self.ram[address] = int(num, 2)
                        address += 1
        except FileNotFoundError:
            print(f"{sys.argv[0]}: {sys.argv[1]} Not found")
            sys.exit()


    def alu(self, op, reg_a, reg_b):
        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        # elif op == "SUB": etc
        elif op == "MUL":
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
        running = True
        LDI = 0b10000010
        PRN = 0b01000111
        HLT = 0b00000001
        MUL = 0b10100010
        PUSH = 0b01000101
        POP = 0b01000110
        CALL = 0b01010000
        RET = 0b00010001
        MULT2PRINT = 0b00011000

        while running:
            IR = self.ram[self.pc]
            operand_a = self.ram_read(self.pc + 1)
            operand_b = self.ram_read(self.pc + 2)
            if IR == HLT:
                running = False
            elif IR == LDI:
                self.reg[operand_a] = operand_b
                self.pc += 3
            elif IR == PRN:
                print(self.reg[operand_a])
                self.pc += 2
            elif IR == MUL:
                self.alu("MUL", operand_a, operand_b)
                self.pc += 3
            elif IR == PUSH:
                self.reg[7] = (self.reg[7] - 1) % 255
                self.SP = self.reg[7]
                regadd = operand_a
                val = self.reg[regadd]
                self.ram[self.SP] = val
                self.pc += 2
            elif IR == POP:
                self.SP = self.reg[7]
                val = self.ram[self.SP]
                regadd = operand_a
                self.reg[regadd] = val
                self.reg[7] = (self.reg[7] + 1) % 255
                self.pc += 2
            elif IR == CALL:
                # regadd = operand_a
                # # calladd = self.reg[regadd]
                # nextinst = self.pc + 2
                # self.reg[7] = (self.reg[7] - 1) % 255
                # self.SP = self.reg[7]
                # self.ram[self.SP] = nextinst
                self.reg[4] = self.pc + 2
                self.push(4)
                self.pc = self.reg[operand_a]
            elif IR == RET:
                self.SP = self.reg[7]
                retadd = self.ram[self.SP]
                self.reg[7] = (self.reg[7] + 1) % 255
                self.pc = retadd
            else:
                print('Error')
                sys.exit()

