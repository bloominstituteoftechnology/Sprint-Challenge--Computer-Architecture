"""CPU functionality."""

import sys

ADD = 0b10100000
HLT = 0b00000001
LDI = 0b10000010
PRN = 0b01000111
MUL = 0b10100010
PUSH = 0b01000101
POP =  0b01000110
CALL = 0b01010000
RET = 0b00010001
CMP = 0b10100111
JMP = 0b01010100
JEQ = 0b01010101
JNE = 0b01010110

class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""

        # is computer running:
        self.running = False

        self.ram = [0] * 256


        self.reg = [0] * 8

        # Internal Registers
        self.pc = 0 # Program Counter: address of the currently executing instruction
        self.mem_add = 0 # Memory Address Register: holds the memory address we're reading or writing
        # Memory Data Register: holds the value to write or the value just read
        self.fl = 0b00000000 # Flag Register: holds the current flags status
        self.SP = 7
        self.reg[self.SP] = 0xf4  #Register at SP contains f4, most recent key press, last element in stack
        
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

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        #elif op == "SUB": etc
        elif op == 'MULT':
            self.reg[reg_a] *= self.reg[reg_b]
        elif op == "CMP":
            self.fl = 0b00000000
            if self.reg[reg_a] == self.reg[reg_b]:
                self.fl = 0b00000001
            elif self.reg[reg_a] < self.reg[reg_b]:
                self.fl = 0b00000100
            elif self.reg[reg_a] > self.reg[reg_b]:
                self.fl = 0b00000010
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
        self.running = True
        while self.running:
            ir = self.ram_read(self.pc)  # Instruction register
            operand_a = self.ram_read(self.pc + 1)
            operand_b = self.ram_read(self.pc + 2)

            if ir == HLT: 
                self.running = False
            elif ir == PRN:
                print(self.reg[operand_a])
                self.pc += 2
            elif ir == LDI:
                self.reg[operand_a] = operand_b
                self.pc += 3
            elif ir == ADD:
                self.alu(ir, operand_a, operand_b)
            elif ir == MUL:
                self.alu("MUL", operand_a, operand_b)
                self.pc += 3
            elif ir == PUSH:
                self.reg[self.SP] -= 1
                self.ram_write(self.reg[self.SP], self.reg[operand_a])
                self.pc += 2
            elif ir == POP:
                self.trace()
                self.reg[operand_a] = self.ram[self.reg[self.SP]]
                self.reg[self.SP] += 1
                self.pc +=2
            elif ir == CMP:
                self.alu("CMP", operand_a, operand_b)
                print("Comparing")
            elif ir == JMP:
                self.pc = self.reg[operand_a]
            elif ir == JEQ:
                if self.fl == 1:
                    self.pc = self.reg[operand_a]
            elif ir == JNE:
                if self.fl != 1:
                    self.pc = self.reg[operand_a]
            else:
                print("Help computer; stop all downloading")
                self.running = False




    def ram_read(self, mem_add):
        return self.ram[mem_add]

    def ram_write(self, mem_add, value):
        self.ram[mem_add] = value