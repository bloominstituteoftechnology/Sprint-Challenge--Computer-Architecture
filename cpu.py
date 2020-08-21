"""CPU functionality."""

import sys

PRN = 0b01000111 
MUL = 0b10100010 
HLT = 0b00000001 
CALL = 0b01010000 
ADD = 0b10100000 
PUSH = 0b01000101 
POP = 0b01000110 
RET = 0b00010001 
LDI = 0b10000010 
JMP = 0b01010100 # Jumps PC to an address in given register
JEQ = 0b01010101 # If E flag is (true, 1), jump to given register
JNE = 0b01010110 # If E flag is clear (false, 0), jump to given register
CMP = 0b10100111 #Compares values and sets flags

if len(sys.argv) < 2:
    print("To use enter python ls8.py *name of program file*")
    sys.exit(1)


class CPU:
    """Main CPU class."""

    def __init__(self, reg = [0] * 8, ram = [0] * 256, pc = 0):
        """Construct a new CPU."""
        self.reg = reg
        self.ram = ram
        self.pc = pc # mem address starts at 0
        self.sp = 244 # start of an empty stack is 244 in ram it then grows downward
        self.running = True

        # Setup Branch Table
        self.branchtable = {}
        self.branchtable[LDI] = self.ldi
        self.branchtable[PRN] = self.prn
        self.branchtable[MUL] = self.mul
        self.branchtable[PUSH] = self.push
        self.branchtable[POP] = self.pop
        self.branchtable[HLT] = self.hlt
        self.branchtable[CALL] = self.call
        self.branchtable[RET] = self.ret
        self.branchtable[ADD] = self.add

        self.branchtable[JMP] = self.jmp
        self.branchtable[JEQ] = self.jeq
        self.branchtable[JNE] = self.jne
        self.branchtable[CMP] = self.cmp

        self.E = 0  # 0 == false, 1 == true
        self.L = 0 # 1 == regA.value < regB.value
        self.G = 0 # 1 == regA.value > regB.value

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            print("ADDING NOW")
            self.reg[reg_a] += self.reg[reg_b]
        elif op == "MUL":
            self.reg[reg_a] *= self.reg[reg_b]
        else:
            raise Exception("Unsupported ALU operation")

    def ram_read(self, mem_address_register):
        value = self.ram[mem_address_register]
        return value

    def ram_write(self, mem_data_register, mem_address_register):
        self.ram[mem_address_register] = mem_data_register

    def jmp(self):
        jump_address = self.reg[self.ram[self.pc + 1]]
        self.pc = jump_address

    def cmp(self):
        self.E = 0
        self.L = 0
        self.G = 0

        value_one = self.reg[self.ram[self.pc + 1]]
        value_two = self.reg[self.ram[self.pc + 2]]

        if value_one == value_two:
            self.E = 1
        elif value_one < value_two:
            self.L = 1
        elif value_one > value_two:
            self.G = 1

    def jne(self):
        if self.E == 0:
            self.pc = self.reg[self.ram[self.pc + 1]]
        else:
            self.pc += 2

    def jeq(self):
        if self.E == 1:
            self.pc = self.reg[self.ram[self.pc + 1]]
        else:
            self.pc += 2

    def add(self):
        self.reg[self.ram[self.pc + 1]] += self.reg[self.ram[self.pc + 2]]

    def call(self):
        next_address = self.pc + 2
        self.sp -= 1
        self.ram[self.sp] = next_address
        # set pc
        address = self.reg[self.ram[self.pc + 1]]
        self.pc = address

    def prn(self):
        operand_a = self.ram[self.pc + 1]
        print(self.reg[operand_a])

    def hlt(self):
        self.running = False

    def mul(self):
        operand_a = self.ram[self.pc + 1]
        operand_b = self.ram[self.pc + 2]
        self.alu("MUL", operand_a, operand_b)


    def ret(self):
        next_address = self.ram[self.sp]
        self.sp += 1

        self.pc = next_address

    def push(self):
        reg_address = self.ram[self.pc + 1]
        self.sp -= 1
        value = self.reg[reg_address]
        self.ram[self.sp] = value

    def pop(self):
        pop_value = self.ram[self.sp]
        reg_address = self.ram[self.pc + 1]
        self.reg[reg_address] = pop_value
        self.sp += 1

    def ldi(self):
        operand_a = self.ram[self.pc + 1] # selected register
        operand_b = self.ram[self.pc + 2] # value to load
        self.reg[operand_a] = operand_b

    def load(self):
        """Load a program into memory."""
        program = []

        with open(sys.argv[1]) as f:
            for line in f:
                split_comment = line.split('#')
                num = split_comment[0].strip()
                try:
                    program.append(int(num, 2))
                except ValueError:
                    pass

        address = 0

        for instruction in program:
            self.ram[address] = instruction
            address += 1

    def run(self):
        """Run the CPU."""
        while self.running:

            ir = self.pc
            op = self.ram[ir]
            instruction_size = ((op & 11000000) >> 6) + 1
            set_flag = (op & 0b00010000) # applies a mask to get pc_set bit
            self.branchtable[op]()
        
            if set_flag != 0b00010000:
                self.pc += instruction_size