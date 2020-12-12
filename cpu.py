"""CPU functionality."""

import sys

HLT = 0b00000001
LDI = 0b10000010
PRN = 0b01000111
MUL = 0b10100010
ADD = 0b10100000
PUSH = 0b01000101
POP = 0b01000110

class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.ram = [0] * 256
        self.reg = [0] * 8
        self.reg[7] = 0xF4
        self.pc = 0
        self.ir = 0
        self.mar = 0
        self.mdr = 0
        self.fl = 0
        self.halted = False
        self.end_of_program = 0

    def load(self, filename):
        """Load a program into memory."""

        address = 0

        with open(filename) as f:
            for line in f:
                comment_split = line.split("#")
                maybe_binary_number = comment_split[0]

                try:
                    x = int(maybe_binary_number, 2)
                    self.ram_write(x, address)
                    address += 1
                except:
                    continue
        self.end_of_program = address

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == ADD:
            self.reg[reg_a] += self.reg[reg_b]
        elif op == MUL:
            self.reg[reg_a] *= self.reg[reg_b]
        else:
            raise Exception("Unsupported ALU operation")

        # keep value in 0-255
        self.reg[reg_a] = self.reg[reg_a] & 0xFF

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
        while not self.halted:
            # IR (Instruction Register) = value at memory address in PC (Program Counter)
            ir = self.ram_read(self.pc)
            operand_a = self.ram_read(self.pc + 1)
            operand_b = self.ram_read(self.pc + 2)
            self.execute_instruction(ir, operand_a, operand_b)

    def execute_instruction(self, instruction, operand_a, operand_b):
        # Instruction layout: 'AABCDDDD'
        # * AA = Number of operands for this opcode, 0-2
        # * B == 1 if this is an ALU operation
        # * C == 1 if this instruction sets the PC
        # * DDDD Instruction identifier
        # INSTRUCTION = 0b10000010 # >> 6 --> 0b10 & 0b11 --> 0b10
        is_alu = instruction >> 5 & 0b1               # 0 or 1
        sets_pc = instruction >> 4 & 0b1              # 0 or 1
        num_operands = (instruction >> 6 & 0b11) + 1  # 0, 1, 2

        if not sets_pc:
            if instruction == HLT:
                self.halted = True
            elif instruction == PRN:
                print(self.reg[operand_a])
            elif instruction == LDI:
                self.reg[operand_a] = operand_b
            elif instruction == PUSH:
                if self.reg[7] > self.end_of_program + 1: # prevent overflow
                    self.reg[7] -= 1
                else:
                    print("ERROR: stack overflow")
                    sys.exit(1)
                register_address = self.ram[self.pc + 1]
                value = self.reg[register_address]
                self.ram[self.reg[7]] = value
            elif instruction == POP:
                register_address = self.ram[self.pc + 1]
                self.reg[register_address] = self.ram[self.reg[7]]
                if self.reg[7] < 0xF4: # prevent underflow
                    self.reg[7] += 1
                else:
                    print("ERROR: stack underflow")
                    sys.exit(1)
            elif is_alu:
                self.alu(instruction, operand_a, operand_b)
            else:
                print("INVALID INSTRUCTION.")
            self.pc += num_operands

    def ram_read(self, MAR): # Memory Address Register
        return self.ram[MAR]
    
    def ram_write(self, MDR, MAR): # Memory Data Register, Memory Address Register
        self.ram[MAR] = MDR
