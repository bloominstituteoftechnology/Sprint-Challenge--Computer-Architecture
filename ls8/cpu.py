"""CPU functionality."""

import sys


LDI = 0b10000010 
PRN = 0b01000111 
HLT = 0b00000001
MUL = 0b10100010 
ADD = 0b10100000 
PUSH = 0b01000101
POP = 0b01000110
RET = 0b00010001
CALL = 0b01010000
CMP = 0b10100111
JMP = 0b01010100
JEQ = 0b01010101
JNE = 0b01010110
SP = 7


class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""        
        self.ram = [0] * 256
        self.reg = [0] * 8
        self.pc = 0
        self.flags = 0
      
    def ram_read(self, address):
        return self.ram[address]

    def ram_write(self, address, value):
        self.ram[address] = value

    def load(self):
        """Load a program into memory."""
        filename = sys.argv[1]
        try:
            address = 0
            with open(filename) as f:
                for line in f:
                    line = line.strip().split('#')[0].strip()
                    if line != '':
                        self.ram[address] = int(line, 2)
                        address += 1
                    else:
                        continue
        
        except FileExistsError:
            print(f'Error: {filename} not found')
            sys.exit(2)

    def alu(self, operation, register_a, register_b):
        """ALU operations."""
        
        if operation == "ADD":
            self.reg[register_a] += self.reg[register_b]
        elif operation == "SUB":
            self.reg[register_a] -= self.reg[register_b]
        elif operation == "MUL":
            self.reg[register_a] *= self.reg[register_b]
        elif operation == "DIV":
            self.reg[register_a] /= self.reg[register_b]
        else:
            raise Exception("ALU operation not supported")

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
        self.load()
        while True:
            pc = self.pc
            instruction_register = self.ram_read(pc)

            if instruction_register == LDI:
                self.reg[self.ram_read(pc + 1)] = self.ram_read(pc + 2)
                self.pc += 3

            elif instruction_register == PRN:
                print(self.reg[self.ram_read(pc + 1)])
                self.pc +=2

            elif  instruction_register == MUL:
                register_a = self.ram_read(pc + 1)
                register_b = self.ram_read(pc + 2)
                self.alu('MUL', register_a, register_b)
                self.pc += 3

            elif instruction_register == ADD:
                register_a = self.ram_read(pc + 1)
                register_b = self.ram_read(pc + 2)
                self.alu("ADD", register_a, register_b)
                self.pc += 3

            elif instruction_register == PUSH:
                self.reg[SP] -= 1
                stack_address = self.reg[SP]
                register_number = self.ram_read(pc + 1)
                value = self.reg[register_number]
                self.ram_write(stack_address, value)
                self.pc += 2

            elif instruction_register == POP:
                stack_value = self.ram_read(self.reg[SP])
                register_number = self.ram_read(pc + 1)
                self.reg[register_number] = stack_value
                self.reg[SP] += 1 
                self.pc += 2

            elif instruction_register == CALL:
                self.reg[SP] -= 1
                stack_address = self.reg[SP]
                returned_address = pc + 2
                self.ram_write(stack_address, returned_address)
                register_number = self.ram_read(pc + 1)
                self.pc = self.reg[register_number]

            elif instruction_register == RET:
                self.pc = self.ram_read(self.reg[SP])
                self.reg[SP] += 1  

            elif instruction_register == CMP:
                register_a = self.ram_read(self.pc + 1)
                register_b = self.ram_read(self.pc + 2)
                value_a = self.reg[register_a]
                value_b = self.reg[register_b]
                if value_a == value_b:
                    self.flags = 0b1
                elif value_a > value_b:
                    self.flags = 0b10
                elif value_b > value_a:
                    self.flags = 0b100
                self.pc += 3

            elif instruction_register == JMP:
                register_a = self.ram_read(self.pc + 1)
                self.pc = self.reg[register_a]

            elif instruction_register == JEQ:
                if not self.flags & 0b1:
                    self.pc += 2
                elif self.flags & 0B1:
                    register_a = self.ram_read(self.pc + 1)
                    self.pc = self.reg[register_a]

            elif instruction_register == JNE:
                if self.flags & 0b1:
                    self.pc += 2
                elif not self.flags & 0b0:
                    register_a = self.ram_read(self.pc + 1)
                    self.pc = self.reg[register_a]

            elif instruction_register == HLT:
                sys.exit(1)

            else:
                print(f"Error: Unknown input:\t {instruction_register}")
                sys.exit(1)
