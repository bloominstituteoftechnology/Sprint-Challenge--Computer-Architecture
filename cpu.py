"""CPU functionality."""

import sys


class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        # 8 slot register being created
        self.register = {d: 0 for d in range(0, 8)}
        # PC pointer created; start at 0
        self.pc = 0
        # 8 bit ram array created
        self.ram = [0] * 256

        self.alu_operation = {
            0b0111:self.CMP,
        }

        self.pc_modifier = {
            0b0100: self.JMP,
            0b0101: self.JEQ,
            0b0110: self.JNE,
        }

    def load(self):
        """Load a program into memory."""
        # Starting address
        address = 0

        # For now, we've just hardcoded a program:
        program = [
            # From print8.ls8
            0b10000010,  # LDI R0,8
            0b00000000,
            0b00001000,
            0b01000111,  # PRN R0
            0b00000000,
            0b00000001,  # HLT
        ]
        # Loading instructions into ram
        for instruction in program:
            self.ram[address] = instruction
            # print(f'{instruction} LOAD INSTRUCTION')
            address += 1

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""
        #
        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        # elif op == "SUB": etc
        else:
            raise Exception("Unsupported ALU operation")

    def trace(self):
        """
        Handy function to print out the CPU state. You might want to call this
        from run() if you need help debugging.
        """

        print(f"TRACE: %02X | %02X %02X %02X |" % (
            self.pc,
            self.fl,
            self.ram_read(self.pc),
            self.ram_read(self.pc + 1),
            self.ram_read(self.pc + 2)
        ), end='')

        for i in range(8):
            print(" %02X" % self.register[i], end='')

        print()

    def ram_read(self, position):
        return self.ram[position]

    def ram_write(self, position, value):
        self.ram[position] = value

    def int_to_bin(self, instruction):
        # print(f'{instruction} INSTRUCTION INT')
        # Converting int to binary, chopping off 0b, and adding leading zeros to make instruction 8 bits long
        instruction = bin(instruction)[2:].zfill(8)
        # print(f'{instruction} INSTRUCTION BIN')
        if instruction == "00000001":
            return None
        else:
            # Converting outputs to binary/base 2
            output = int(instruction[4:], 2)
            return output

    def run(self):
        """Run the CPU."""
        halted = False

        while not halted:
            current_instruct = self.ram_read(self.pc)
            binary = self.int_to_bin(current_instruct)
            self.pc += 1
            if binary is None:
                halted = True
            else:
                if binary == 0b0010:  # LDI
                    register_location = self.ram_read(self.pc)
                    self.pc += 1
                    register_value = self.ram_read(self.pc)
                    self.pc += 1
                    self.register[register_location] = register_value
                elif binary == 0b0111:  # PRN
                    register_location = self.ram_read(self.pc)
                    self.pc += 1
                    print(self.register[register_location])

    def CMP(self, reg_a, reg_b):
        val_a = self.register[reg_a]
        val_b = self.register[reg_b]

        if val_a == val_b:
            self.fl = 0b00000001
        elif val_a > val_b:
            self.fl = 0b00000010
        elif val_a < val_b:
            self.fl = 0b00000100
        else:
            self.fl = 0b0

    def JMP(self, reg_a):
        self.pc = self.register[reg_a]

    def JEQ(self, reg_a):
        a = bool(self.fl & 0b00000001)
        if a:
            self.pc = self.register[reg_a]

    def JNE(self, reg_a):
        a = bool(self.fl & 0b00000001)
        if not a:
            self.pc = self.register[reg_a]