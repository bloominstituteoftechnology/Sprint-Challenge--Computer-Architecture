"""CPU functionality."""

import sys

HLT = 0b00000001
LDI = 0b10000010
PRN = 0b01000111

MUL = 0b10100010
DIV = 0b10100011
ADD = 0b10100000
SUB = 0b10100001
MOD = 0b10100100
CMP = 0b10100111

POP = 0b01000110
PUSH = 0b01000101

CALL = 0b01010000
RET = 0b00010001

# SC
JMP  = 0b01010100
JEQ  = 0b01010101
JNE  = 0b01010110

# Stretch Stuff
AND = 0b10101000
OR = 0b10101010
XOR = 0b10101011
NOT = 0b01101001
SHL = 0b10101100
SHR = 0b10101101

# Defining stack pointer as a variable
SP = 7

class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        # Branch table
        self.branch = {
            HLT: self.hlt,
            LDI: self.ldi,
            PRN: self.prn,
            POP: self.pop,
            PUSH: self.push,
            CALL: self.call,
            RET: self.ret,
            ADD: self.add,
            SUB: self.sub,
            MUL: self.mul,
            DIV: self.div,
            MOD: self.mod,
            CMP: self.cmp,
            JMP: self.jmp,
            JEQ: self.jeq,
            JNE: self.jne,
            AND: self.and_bit,
            OR: self.or_bit,
            XOR: self.xor,
            NOT: self.not_bit,
            SHL: self.shl,
            SHR: self.shr
                       }
        # Initializing 8 general purpose registers
        self.reg = bytearray(8)

        # Initializing memory
        self.ram = bytearray(256) # RAM capacity is 256 bytes
        self.reg[SP] = 0xF4 # reg 7 = 0xF4 in RAM

        # Internal registers
        self.PC = 0 # Program counter (pointer)
        self.IR = 0 # Instruction register

        self.MAR = 0 # Memory address register
        self.MDR = 0 # Memory data register
        
        self.FL = 0 # equal flag 0b00000000

    def hlt(self, reg_num, value):
        """
        Halt the CPU (and exit the emulator)
        """
        sys.exit()

    def ldi(self, reg_num, value):
        """
        Sets the value of a register to an integer
        """
        self.reg[reg_num] = value

    def prn(self, reg_num):
        """
        Prints the numeric value stored in the given register
        """
        print(self.reg[reg_num])

    def pop(self, reg_num):
        """
        Pops the value at the top of the stack into the register
        """
        self.reg[reg_num] = self.ram_read(self.reg[SP])  # Copy the value at the address pointed to by the SP to the given register
        self.reg[SP] += 1 # Increment SP

    def push(self, reg_num):
        """ 
        Pushes the value in the register onto the stack
        """
        self.reg[SP] -= 1 # Decrement SP
        self.ram_write(self.reg[reg_num], self.reg[SP]) # Copy the value in the given register to the address pointed to by the SP

    def call(self, reg_num):
        """
        Calls a subroutine at the address stored in the register
        """
        self.ldi(4, self.PC + 2) # Address of the return
        self.push(4) # Push to stack
        self.PC = self.reg[reg_num] # PC is set to the address stored in the register

    def ret(self):
        """
        Return value from subroutine
        """
        self.pop(4) # Pops the value from the top of the stack 
        self.PC = self.reg[4] # Stores it in the PC

    # Arithmetic operations
    def add(self, reg_a, reg_b):
        self.alu("ADD", reg_a, reg_b)

    def sub(self, reg_a, reg_b):
        self.alu("SUB", reg_a, reg_b)

    def mul(self, reg_a, reg_b):
        self.alu("MUL", reg_a, reg_b)
    
    def div(self, reg_a, reg_b):
        self.alu("DIV", reg_a, reg_b)

    def mod(self, reg_a, reg_b):
        self.alu("MOD", reg_a, reg_b)
    
    def cmp(self, reg_a, reg_b):
        self.alu("CMP", reg_a, reg_b)
    
    def and_bit(self, reg_a, reg_b):
        self.alu("AND", reg_a, reg_b)
    
    def or_bit(self, reg_a, reg_b):
        self.alu("OR", reg_a, reg_b)

    def xor(self, reg_a, reg_b):
        self.alu("XOR", reg_a, reg_b)

    def not_bit(self, reg_num):
        self.alu("NOT", reg_num)

    def shl(self, reg_a, reg_b):
        self.alu('SHL', reg_a, reg_b)

    def shr(self, reg_a, reg_b):
        self.alu('SHR', reg_a, reg_b)
    
    # SC
    def jmp(self, reg_num):
        """
        Jump to the address stored in the given register
        """
        self.PC = self.reg[reg_num] # Sets the program counter to the address stored in the given register
        
    def jeq(self, reg_num):
        """
        If equal flag is set (true), jump to the address stored in the given register
        """
        if self.FL == 1:
            self.PC = self.reg[reg_num]
        else: 
            self.PC += 2 # continue

    def jne(self, reg_num):
        """
        If equal flag is clear (false, 0), jump to the address stored in the given register
        """
        if self.FL != 1:
            self.PC = self.reg[reg_num]
        else: 
            self.PC += 2 # continue
    
    # Memory
    def ram_read(self, address):
        """
        Returns value stored at an address.
        """
        # print(f'RAM at {self.PC} has the address value {self.ram[address]}. ')
        return self.ram[address]

    def ram_write(self, value, address):
        """
        Writes a value to an address.
        """
        # print(f'Value {self.ram[address]} was written to RAM at {self.PC} address {self.ram[address]}. ')
        self.ram[address] = value

    def load(self):
        """Loads a program into memory."""
        address = 0

        # Handling user input error
        if len(sys.argv) != 2:
            print("Usage: ls8.py <filename>")
            sys.exit()

        with open(sys.argv[1]) as file:
            program = [int(line[:line.find('#')].strip(), 2) \
            for line in file \
            if line != '\n' and line[0] != '#']

        for instruction in program:
            self.ram[address] = instruction
            address += 1


    def alu(self, op, reg_a, reg_b):
        """ALU operations."""
        # Addition
        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        # Subtraction
        elif op == "SUB":
            self.reg[reg_a] -= self.reg[reg_b]
        # Multiplication
        elif op == "MUL":
            self.reg[reg_a] *= self.reg[reg_b]
        # Division
        elif op == "DIV":
            self.reg[reg_a] //= self.reg[reg_b]
        # Modulo
        elif op == "MOD":
            self.reg[reg_a] %= self.reg[reg_b]
        # Compare
        elif op == "CMP": # changes the flag depending on regA & regB
            if self.reg[reg_a] < self.reg[reg_b]:
                self.FL = 0b00000100 # 4 
            elif self.reg[reg_a] > self.reg[reg_b]:
                self.FL = 0b00000010 # 2 
            elif self.reg[reg_a] == self.reg[reg_b]:
                self.FL = 0b00000001 # 1
        # Stretch 
        # Bitwise &
        elif op == "AND": # Stores the result in register A
            self.reg[reg_a] &= self.reg[reg_b]
        # Bitwise or
        elif op == "OR": # Stores the result in register A
            self.reg[reg_a] |= self.reg[reg_b]
        # Bitwise XOR (exclusive or)
        elif op == "XOR": # Stores the result in register A
            self.reg[reg_a] ^= self.reg[reg_b]
        # Bitwise not
        elif op == "NOT": # stores the result in the register
            self.reg[reg_a] = ~self.reg[reg_a]
        # Bitwise shift left
        elif op == SHL: # Shift the value in register A left by the number of bits specified in register B, fills rightmost bits with 0
            self.reg[reg_a] = self.reg[reg_a] << reg_b
        # bitwise shift right
        elif op == SHR: # Shift the value in register A right by the number of bits specified in register B, fills rightmost bits with 0
            self.reg[reg_a] = self.reg[reg_a] >> reg_b
        else:
            raise Exception("Unsupported ALU operation")

    def trace(self):
        """
        Handy function to print out the CPU state. You might want to call this
        from run() if you need help debugging.
        """

        print(f"TRACE: %02X | %02X %02X %02X |" % (
            self.PC,
            #self.fl,
            #self.ie,
            self.ram_read(self.PC),
            self.ram_read(self.PC + 1),
            self.ram_read(self.PC + 2)
        ), end='')

        for i in range(8):
            print(" %02X" % self.reg[i], end='')
        print()

    def run(self):
        """Run the CPU.
        Reads memory address and stores result in IR.
        """
        self.IR = self.ram_read(self.PC) # set IR (increments every loop)
        op_a = self.ram_read(self.PC + 1) # reg 1
        op_b = self.ram_read(self.PC + 2) # reg 2

        # While cpu is running
        while self.IR != HLT:
            nums = (self.IR & 0b11000000) >> 6 # CPU ops
            pc_set = (self.IR & 0b00010000) >> 4
            try:
                if nums == 0:
                    self.branch[self.IR]()
                elif nums == 1:
                    self.branch[self.IR](op_a)
                else:
                    self.branch[self.IR](op_a, op_b)

            except KeyError:
                raise Exception("Unsupported operation {self.IR} at address {self.PC}.")

            if pc_set == 0:
                self.PC += nums + 1

            self.IR = self.ram_read(self.PC)
            op_a = self.ram_read(self.PC + 1)
            op_b = self.ram_read(self.PC + 2)