"""CPU functionality."""

import sys

HLT = 0b00000001
LDI = 0b10000010
PRN = 0b01000111
MUL = 0b10100010
ADD = 0b10100000
PUSH = 0b01000101
POP = 0b01000110
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
        self.pc = 0
        self.SP = 7
        self.reg[self.SP] = 0xf4
        self.E = 0
        self.L = 0
        self.G = 0

    def load(self):
        """Load a program into memory."""

        address = 0

        # For now, we've just hardcoded a program:

        program = [
            0b10000010,  # LDI R0,10
            0b00000000,
            0b00001010,
            0b10000010,  # LDI R1,20
            0b00000001,
            0b00010100,
            0b10000010,  # LDI R2,TEST1
            0b00000010,
            0b00010011,
            0b10100111,  # CMP R0,R1
            0b00000000,
            0b00000001,
            0b01010101,  # JEQ R2
            0b00000010,
            0b10000010,  # LDI R3,1
            0b00000011,
            0b00000001,
            0b01000111,  # PRN R3
            0b00000011,
            # , TEST1 (address 19):
            0b10000010,  # LDI R2,TEST2
            0b00000010,
            0b00100000,
            0b10100111,  # CMP R0,R1
            0b00000000,
            0b00000001,
            0b01010110,  # JNE R2
            0b00000010,
            0b10000010,  # LDI R3,2
            0b00000011,
            0b00000010,
            0b01000111,  # PRN R3
            0b00000011,
            # , TEST2 (address 32):
            0b10000010,  # LDI R1,10
            0b00000001,
            0b00001010,
            0b10000010,  # LDI R2,TEST3
            0b00000010,
            0b00110000,
            0b10100111,  # CMP R0,R1
            0b00000000,
            0b00000001,
            0b01010101,  # JEQ R2
            0b00000010,
            0b10000010,  # LDI R3,3
            0b00000011,
            0b00000011,
            0b01000111,  # PRN R3
            0b00000011,
            # , TEST3 (address 48):
            0b10000010,  # LDI R2,TEST4
            0b00000010,
            0b00111101,
            0b10100111,  # CMP R0,R1
            0b00000000,
            0b00000001,
            0b01010110,  # JNE R2
            0b00000010,
            0b10000010,  # LDI R3,4
            0b00000011,
            0b00000100,
            0b01000111,  # PRN R3
            0b00000011,
            # , TEST4 (address 61):
            0b10000010,  # LDI R3,5
            0b00000011,
            0b00000101,
            0b01000111,  # PRN R3
            0b00000011,
            0b10000010,  # LDI R2,TEST5
            0b00000010,
            0b01001001,
            0b01010100,  # JMP R2
            0b00000010,
            0b01000111,  # PRN R3
            0b00000011,
            # , TEST5 (address 73):
            0b00000001  # HLT
        ]

        for instruction in program:
            self.ram[address] = instruction
            address += 1

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        elif op == "MUL":
            self.reg[reg_a] *= self.reg[reg_b]
        elif op == 'CMP':
            if self.reg[reg_a] == self.reg[reg_b]:
                self.E = 1
                self.L = 0
                self.G = 0
            elif self.reg[reg_a] < self.reg[reg_b]:
                self.E = 0
                self.L = 1
                self.G = 0
            elif self.reg[reg_a] > self.reg[reg_b]:
                self.E = 0
                self.L = 0
                self.G = 1
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

    def ram_read(self, key):
        return self.ram[key]

    def ram_write(self, key, value):
        self.ram[key] = value

    def run(self):
        """Run the CPU."""
        halted = False

        while not halted:
            instruction = self.ram_read(self.pc)

            if instruction == PUSH:
                self.reg[self.SP] -= 1

                reg_num = self.ram_read(self.pc+1)
                reg_val = self.reg[reg_num]
                self.ram_write(self.reg[self.SP], reg_val)
                # operand_a = self.ram_read(self.pc+1)
                # self.ram_write(self.reg[self.SP], self.reg[operand_a])

                self.pc += 2

            elif instruction == POP:
                val = self.ram_read(self.reg[self.SP])
                reg_num = self.ram_read(self.pc + 1)
                self.reg[reg_num] = val
                # operand_a = self.ram_read(self.pc+1)
                # self.reg[operand_a] = self.ram_read(self.reg[self.SP])

                self.reg[self.SP] += 1

                self.pc += 2

            elif instruction == CMP:
                reg_num = self.ram_read(self.pc + 1)
                value = self.ram_read(self.pc + 2)

                self.alu('CMP', reg_num, value)
                self.pc += 3

            elif instruction == JMP:
                reg_num = self.ram_read(self.pc + 1)
                value = self.ram_read(self.pc + 2)

                self.pc = self.reg[reg_num]

            elif instruction == JEQ:
                reg_num = self.ram_read(self.pc + 1)
                value = self.ram_read(self.pc + 2)

                if self.E == 1:
                    self.pc = self.reg[reg_num]
                else:
                    self.pc += 2

            elif instruction == JNE:
                reg_num = self.ram_read(self.pc + 1)
                value = self.ram_read(self.pc + 2)

                if self.E == 0:
                    self.pc = self.reg[reg_num]
                else:
                    self.pc += 2

            elif instruction == LDI:
                operand_a = self.ram_read(self.pc+1)
                operand_b = self.ram_read(self.pc+2)

                self.reg[operand_a] = operand_b

                self.pc += 3

            elif instruction == PRN:
                operand_a = self.ram_read(self.pc+1)
                print(self.reg[operand_a])

                self.pc += 2

            elif instruction == HLT:
                halted = True
                self.pc = 0

            elif instruction == MUL:
                operand_a = self.ram_read(self.pc+1)
                operand_b = self.ram_read(self.pc+2)

                self.alu("MUL", operand_a, operand_b)

                self.pc += 3

            elif instruction == ADD:
                operand_a = self.ram_read(self.pc+1)
                operand_b = self.ram_read(self.pc+2)

                self.alu("ADD", operand_a, operand_b)

                self.pc += 3

            else:
                print(f"Unknown instruction at index {self.pc}")
                sys.exit(1)