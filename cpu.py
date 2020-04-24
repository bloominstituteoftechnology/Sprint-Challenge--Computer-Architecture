"""CPU functionality."""

import sys

# HLT = 0b00000001
# LDI = 0b10000010
# PRN = 0b01000111
# MUL = 0b10100010
# PUSH = 0b01000101
# POP = 0b01000110
# CALL = 0b01010000
# RET = 0b00010001
# ADD = 0b10100000
# CMP = 0b10100111
# JMP = 0b01010100
# JEQ = 0b01010101
# JNE = 0b01010110


class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.reg = [0] * 8
        self.ram = [0] * 256
        self.pc = 0
        self.running = False
        self.SP = 7
        self.reg[self.SP] = 0xF4
        self.FL = 0b00000000
        self.branchtable = {}
        self.branchtable[0b00000001] = self.handle_HLT
        self.branchtable[0b10000010] = self.handle_LDI
        self.branchtable[0b01000111] = self.handle_PRN
        self.branchtable[0b10100010] = self.handle_MUL
        self.branchtable[0b01000101] = self.handle_PUSH
        self.branchtable[0b01000110] = self.handle_POP
        self.branchtable[0b01010000] = self.handle_CALL
        self.branchtable[0b00010001] = self.handle_RET
        self.branchtable[0b10100000] = self.handle_ADD
        self.branchtable[0b10100111] = self.handle_CMP
        self.branchtable[0b01010100] = self.handle_JMP
        self.branchtable[0b01010101] = self.handle_JEQ
        self.branchtable[0b01010110] = self.handle_JNE

    def handle_HLT(self):
        self.running = False

    def handle_LDI(self):
        operand_a = self.ram[self.pc+1]
        operand_b = self.ram[self.pc+2]
        self.reg[operand_a] = operand_b
        self.pc += 3

    def handle_PRN(self):
        operand_a = self.ram[self.pc+1]
        print(self.reg[operand_a])
        self.pc += 2

    def handle_MUL(self):
        operand_a = self.ram[self.pc+1]
        operand_b = self.ram[self.pc+2]
        self.alu('MUL', operand_a, operand_b)
        self.pc += 3

    def handle_ADD(self):
        operand_a = self.ram[self.pc+1]
        operand_b = self.ram[self.pc+2]
        self.alu('ADD', operand_a, operand_b)
        self.pc += 3

    def handle_PUSH(self):
        # Decrement the SP.
        # Copy the value in the given register to the address pointed to by SP.
        self.reg[self.SP] -= 1
        reg_num = self.ram[self.pc+1]
        value = self.reg[reg_num]
        address = self.reg[self.SP]
        self.ram[address] = value

        self.pc += 2

    def handle_POP(self):
        # Copy the value from the address pointed to by SP to the given register.
        # Increment SP.
        reg_num = self.ram[self.pc+1]
        address = self.reg[self.SP]
        value = self.ram[address]
        self.reg[reg_num] = value
        self.reg[self.SP] += 1

        self.pc += 2

    def handle_CALL(self):
        # compute return address
        return_addr = self.pc + 2

        # push on the stack
        self.reg[self.SP] -= 1
        self.ram[self.reg[self.SP]] = return_addr

        # set the PC to the value in the given register
        reg_num = self.ram[self.pc+1]
        dest_addr = self.reg[reg_num]
        self.pc = dest_addr

    def handle_RET(self):
        # pop return address from top of stack
        return_addr = self.ram[self.reg[self.SP]]
        self.reg[self.SP] += 1

        # set the pc
        self.pc = return_addr

    def handle_CMP(self):
        # Compares the values in two registers. increment PC by 3
        operand_a = self.ram[self.pc+1]
        operand_b = self.ram[self.pc+2]
        self.alu('CMP', operand_a, operand_b)
        self.pc += 3

    def handle_JMP(self):
        # Jump to the address stored in the given register.
        reg_num = self.ram[self.pc+1]
        # Set the PC to the address stored in the given register.
        self.pc = self.reg[reg_num]

    def handle_JEQ(self):
        # If equal flag is set (true), jump to the address stored in the given register.
        reg_num = self.ram[self.pc+1]
        if self.FL & 0b00000001 == 1:
            self.pc = self.reg[reg_num]
        else:
            self.pc += 2

    def handle_JNE(self):
        # If E flag is clear (false, 0), jump to the address stored in the given register.
        reg_num = self.ram[self.pc+1]
        if self.FL & 0b00000001 == 0:
            self.pc = self.reg[reg_num]
        else:
            self.pc += 2

    def load(self):
        """Load a program into memory."""

        program_filename = sys.argv[1]
        address = 0

        with open(program_filename) as f:
            for line in f:
                line = line.split('#')
                line = line[0].strip()

                if line == '':
                    continue

                self.ram[address] = int(line, 2)
                address += 1

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        elif op == "MUL":
            self.reg[reg_a] *= self.reg[reg_b]
        elif op == "CMP":
            # 00000LGE
            if self.reg[reg_a] == self.reg[reg_b]:
                # set the E flag to 1
                self.FL = 0b00000001
            elif self.reg[reg_a] < self.reg[reg_b]:
                # set the L flag to 1
                self.FL = 0b00000100
            elif self.reg[reg_a] > self.reg[reg_b]:
                # set the G flag to 1
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
            # self.fl,
            # self.ie,
            self.ram_read(self.pc),
            self.ram_read(self.pc+1),
            self.ram_read(self.pc+2)
        ), end='')

        for i in range(8):
            print(" %02X" % self.reg[i], end='')

        print()

    def run(self):
        """Run the CPU."""

        self.running = True

        while self.running:
            IR = self.ram[self.pc]

            if IR in self.branchtable:
                function = self.branchtable[IR]
                function()
            else:
                print("Unknown instruction", IR)
                self.running = False

    def ram_read(self, MAR):
        # MAR is address
        # return MDR or value
        # accept the address to read and return the value stored there
        MDR = self.ram[MAR]
        return MDR

    def ram_write(self, MDR, MAR):
        # MDR contains the data that was read or the data to write
        # accept a value to write and the address to write it to
        self.ram[MAR] = MDR
