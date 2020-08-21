"""CPU functionality."""

import sys

ADD = 0b10100000
LDI = 0b10000010
PRN = 0b01000111
HLT = 0b00000001
MUL = 0b10100010
PUSH = 0b01000101
POP = 0b01000110
CALL = 0b01010000
RET = 0b00010001
CMP = 0b10100111
JMP = 0b01010100
JEQ = 0b01010101
JNE = 0b01010110

sp = 7

class CPU:
    """Main CPU class."""


    def __init__(self):
        """Construct a new CPU."""
        self.ram = [0] * 256
        self.reg = [0] * 8
        self.pc = 0
        self.reg[sp] = 0xF4
        self.FL = 0b00000000

    def ram_read(self, ram_address):
        ram_value = self.ram[ram_address]
        return ram_value

    def ram_write(self, ram_value, ram_address):
        self.ram[ram_address] = ram_value

    def load(self):
        """Load a program into memory."""

        address = 0

        with open(sys.argv[1]) as f:
            for line in f:
                l_value = line.split("#")[0].strip()
                if l_value == '':
                    continue
                val = int(l_value, 2)
                self.ram[address] = val
                address += 1

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == ADD:
            self.reg[reg_a] += self.reg[reg_b]
        elif op == MUL:
            self.reg[reg_a] *= self.reg[reg_b]
        elif op == CMP:
            self.FL = 0b00000000
            if self.reg[reg_a] == self.reg[reg_b]:
                self.FL = 0b00000001
            elif self.reg[reg_a] < self.reg[reg_b]:
                self.FL = 0b00000100
            elif self.reg[reg_a] > self.reg[reg_b]:
                self.FL = 0b00000010
        else:
            raise Exception("Unsupported ALU operation")

    def trace(self):
        """
        Handy function to print out the CPU state. You might want to call this
        from run() if you need help debugging.
        """

        print(f"TRACE: %02X | %02X %02X %02X |" % (
            self.pc,
            self.ram_read(self.pc),
            self.ram_read(self.pc + 1),
            self.ram_read(self.pc + 2)
        ), end='')

        for i in range(8):
            print(" %02X" % self.reg[i], end='')

        print()

    def run(self):
        """Run the CPU."""
        running = True

        while running:
            ir = self.ram_read(self.pc)
            operand_a = self.ram_read(self.pc + 1)
            operand_b = self.ram_read(self.pc + 2)
          

            if ir == LDI:
                self.reg[operand_a] = operand_b

            elif ir == PRN:
                print_item = self.ram[self.pc + 1]
                print(self.reg[print_item])

            elif ir == HLT:
                running = False
            
            elif ir == ADD:
                self.alu(ir, operand_a, operand_b)
            
            elif ir == MUL:
                self.alu(ir, operand_a, operand_b)

            elif ir == CMP:
                self.alu(ir, operand_a, operand_b)

            elif ir == PUSH:
                self.reg[sp] -= 1
                self.ram[self.reg[sp]] = self.reg[operand_a]
            
            elif ir == POP:
                self.reg[operand_a] = self.ram[self.reg[sp]]
                self.reg[sp] += 1

            elif ir == CALL:
                self.reg[sp] -= 1
                jump_point = self.pc + (ir >> 6) + 1
                self.ram[self.reg[sp]] = jump_point
                self.pc = self.reg[operand_a]
                continue
                
            elif ir == RET:
                ret_value = self.ram[self.reg[sp]]
                self.pc = ret_value
                self.reg[sp] += 1
                continue
            
            elif ir == JMP:
                self.pc = self.reg[operand_a]
                continue

            elif ir == JEQ:
                if self.FL == 1:
                    self.pc = self.reg[operand_a]
                    continue


            elif ir == JNE:
                if self.FL != 1:
                    self.pc = self.reg[operand_a]
                    continue
            
            else:
                print('Not working')
                running = False
            
            self.pc += (ir >> 6) + 1 