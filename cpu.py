"""CPU functionality."""

import sys

LDI = 0b10000010
HLT = 0b00000001
PRN = 0b01000111
MUL = 0b10100010
NOP = 0b00000000
PUSH = 0b01000101
POP = 0b01000110
RET = 0b00010001
CALL = 0b01010000
SP = 0b00000111
ADD = 0b10100000
SUB = 0b10100001
CMP = 0b10100111
EQ = 0b00000111
JMP = 0b01010100
JEQ = 0b01010101
JNE = 0b01010110

class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        # Create storage for the memory
        self.ram = [0] * 256
        # Create general purpose registers
        self.register = [0] * 8
        self.flag_register = [0] * 8
        # Create a program counter
        self.pc = 0
        self.running = True
        self.sp = len(self.register) - 1
        self.branch_table = {
            NOP: self.nop,
            HLT: self.hlt,
            PRN: self.prn,
            LDI: self.ldi,
            MUL: self.mul,
            ADD: self.add,
            SUB: self.sub,
            PUSH: self.push,
            POP: self.pop,
            CALL: self.call,
            RET: self.ret,
            CMP: self.cmp,
            JMP: self.jmp,
            JEQ: self.jeq,
            JNE: self.jne
        }

    def read_from_memory(self, address):
        return self.ram[address]

    def write_to_memory(self, address, value):
        self.ram[address] = value

    def load(self):
        address = 0
        with open("sctest.ls8", "r") as file:
            for line in file:
                line = line.split("#")[0].strip()
                if line == "":
                    continue
                try:
                    value = int(line, 2)
                except ValueError:
                    continue
                self.ram_write(address, value)
                address += 1

    def alu(self, op, reg_a, reg_b):
        
        if op == "ADD":
            self.register[reg_a] += self.register[reg_b]
        elif op == "MUL":
            self.register[reg_a] *= self.register[reg_b]
        elif op == "SUB":
            self.register[reg_a] -= self.register[reg_b]
        elif op == "CMP":
            # If they are equal, set the Equal E flag to 1, otherwise set it to 0.
            if reg_a == reg_b:
                self.flag_register[E] = 0b00000001
            # If registerA is less than registerB, set the Less-than L flag to 1, otherwise set it to 0.
            elif reg_a < reg_b:
                self.flag_register[L] = 0b00000100
            # If registerA is greater than registerB, set the Greater-than G flag to 1, otherwise set it to 0.
            elif reg_b > reg_b:
                self.flag_register[G] = 0b00000010 
        elif op == "AND":
            self.register[reg_a] = self.register[reg_a] & self.register[reg_b]
        elif op == "MOD":
            if self.register[reg_b] == 0:
                print("Cannot mod by value of 0")
                self.hlt(reg_a, reg_b)
            else:
                self.register[reg_a] %= self.register[reg_b]
        elif op == "SHL":
            self.register[reg_a] << self.register[reg_b]
        elif op == "SHR":
            self.register[reg_a] >> self.register[reg_b]
        elif op == "OR":
            self.register[reg_a] = self.register[reg_a] | self.register[reg_b]
        elif op == "NOT":
            self.register[reg_a] -= 0b11111111
        elif op == "XOR":
            self.register[reg_a] = self.register[reg_a] ^ self.register[reg_b]
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
            self.read_from_memory(self.pc),
            self.read_from_memory(self.pc + 1),
            self.read_from_memory(self.pc + 2)
        ), end='')

        for i in range(8):
            print(" %02X" % self.register[i], end='')

        print()
        
    def ldi(self, reg_a, reg_b):
        self.register[reg_a] = reg_b
    
    def prn(self, reg_a, reg_b):
        # Print the value stored at the given register
        print(self.register[reg_a])

    def hlt(self, reg_a, reg_b):
        self.running = False

    def mul(self, reg_a, reg_b):
        # Read the bytes from memory, and convert readable commands
        self.alu("MUL", reg_a, reg_b)

    def pop(self, reg_a, reg_b):
        top_of_stack = self.register[SP]
        value = self.ram[top_of_stack]
        register = self.ram[reg_a]
        self.register[register] = value
        self.register[SP] += 1

    def push(self, reg_a, reg_b):
        register = self.ram[reg_a]
        value = self.register[register]
        self.register[SP] -= 1
        add_to_stack = self.register[SP]
        self.ram[add_to_stack] = value

    def sub(self, reg_a, reg_b):
        self.alu("SUB", reg_a, reg_b)

    def add(self, reg_a, reg_b):
        self.alu("ADD", reg_a, reg_b)

    def nop(self, reg_a, reg_b):
        pass

    def call(self, reg_a, reg_b):
        address = reg_b
        self.register[SP] -= 1
        self.ram[self.register[SP]] = address

        register_number = self.ram[reg_a]
        subroutine_address = self.register[register_number]
    
        self.pc = subroutine_address

    def ret(self, reg_a, reg_b):
        subroutine_address = self.ram[self.register[SP]]
        self.register[SP] += 1
        self.pc = subroutine_address

    def cmp(self, reg_a, reg_b):
        register_1 = self.register[reg_a]
        register_2 = self.register[reg_b]
        self.alu("CMP", register_1, register_2)

    def jmp(self, reg_a, reg_b):
        # Jump to the address stored in the given register.
        # Set the PC to the address stored in the given register.
        self.pc = self.register[reg_a]

    def jeq(self, reg_a, reg_b):
        # If equal flag is set (true), jump to the address stored in the given register.
        if self.flag_register[E] == 0b000001:
            self.pc = self.register[reg_a]
        else:
            self.pc += 2
    
    def jne(self, reg_a, reg_b):
        # If E flag is clear (false, 0), jump to the address stored in the given register.
        if self.flag_register[E] == 0b000000:
            self.pc = self.register[reg_a]
        else:
            self.pc += 2
    
    def run(self):
        while self.running:
            IR = self.read_from_memory(self.pc)
            pc_flag = (IR & 0b00010000) >> 4
            register_1 = self.ram[self.pc + 1]
            register_2 = self.ram[self.pc + 2]
            self.branch_table[IR](register_1, register_2)
            if pc_flag == 0:
                move = int((IR & 0b11000000) >> 6)
                self.pc += move + 1