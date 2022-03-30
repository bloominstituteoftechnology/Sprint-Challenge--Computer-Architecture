import sys

# MVP OPCodes
LDI = 0b10000010
HLT = 0b00000001
PRN = 0b01000111
MUL = 0b10100010
NOP = 0b00000000
POP = 0b01000110
RET = 0b00010001
CALL = 0b01010000
PUSH = 0b01000101
SP = 0b00000111
ADD = 0b10100000
SUB = 0b10100001
CMP = 0b10100111
EQ = 0b00000111
JMP = 0b01010100
JEQ = 0b01010101
JNE = 0b01010110

# Stretch OPCodes
AND = 0b10101000
MOD = 0b10100100
SHL = 0b10101100
SHR = 0b10101101
XOR = 0b10101011
OR = 0b10101010
NOT = 0b01101001

class CPU:
    
    def __init__(self):
        # This will hold our 256 bytes of memory
        self.ram = [0] * 256

        # This will hold out 8 general purpose registers
        self.reg = [0] * 8
        self.flag_reg = [0] * 8

        # This is our program counter. It holds the address of the currently executing instruction
        self.pc = 0

        # Boolean value as to whether the program is running or not
        self.running = True

        # Branch table for easier selection
        self.branch_table = {
            NOP: self.NOP,
            HLT: self.HLT,
            PRN: self.PRN,
            LDI: self.LDI,
            MUL: self.MUL,
            ADD: self.ADD,
            SUB: self.SUB,
            PUSH: self.PUSH,
            POP: self.POP,
            CALL: self.CALL,
            RET: self.RET,
            CMP: self.CMP,
            JMP: self.JMP,
            JEQ: self.JEQ,
            JNE: self.JNE
        }

    def ram_read(self, address):
        # This will accept the address to read and return the value stored
        return self.ram[address]

    def ram_write(self, address, value):
        # This will accept the value to write, and the address to write it to
        self.ram[address] = value

    def load(self):
        address = 0

        # We'll open the files
        with open("sctest.ls8", "r") as file:
            # Loop through our file
            for line in file:
                # We'll split the string to a list using the "#" as the separator, and strip to remove spaces at the beginning and at the end of a string
                line = line.split("#")[0].strip()
                if line == "":
                    continue
                try:
                    v = int(line, 2)
                except ValueError:
                    continue
                self.ram_write(address, v)
                address += 1

        

    def alu(self, op, reg_a, reg_b):

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        elif op == "MUL":
            self.reg[reg_a] *= self.reg[reg_b]
        elif op == "SUB":
            self.reg[reg_a] -= self.reg[reg_b]
        elif op == "CMP":
            if reg_a == reg_b:
                self.flag_reg[EQ] = 0b00000001
            else:
                self.flag_reg[EQ] = 0b00000000
        elif op == "AND":
            self.reg[reg_a] = self.reg[reg_a] & self.reg[reg_b]
        elif op == "MOD":
            if self.reg[reg_b] == 0:
                print("Cannot mod by value of 0")
                self.HLT(reg_a, reg_b)
            else:
                self.reg[reg_a] %= self.reg[reg_b]
        elif op == "SHL":
            self.reg[reg_a] << self.reg[reg_b]
        elif op == "SHR":
            self.reg[reg_a] >> self.reg[reg_b]
        elif op == "OR":
            self.reg[reg_a] = self.reg[reg_a] | self.reg[reg_b]
        elif op == "NOT":
            self.reg[reg_a] -= 0b11111111
        elif op == "XOR":
            self.reg[reg_a] = self.reg[reg_a] ^ self.reg[reg_b]
        else:
            raise Exception("Unsupported ALU operation")

    def trace(self):

        print(f"TRACE: %02X | %02X %02X %02X |" % (
            self.pc,
            self.ram_read(self.pc),
            self.ram_read(self.pc + 1),
            self.ram_read(self.pc + 2)
        ), end='')

        for i in range(8):
            print(" %02X" % self.reg[i], end='')

        print()

    def LDI(self, reg_a, reg_b):
        # Set the value of a register to an integer
        self.reg[reg_a] = reg_b
    
    def HLT(self, reg_a, reg_b):
        # We hault the CPU by setting it to false
        self.running = False

    def PRN(self, reg_a, reg_b):
        # This will print the numeric value stored in the given register
        print(self.reg[reg_a])

    def MUL(self, reg_a, reg_b):
        # We'll read the bytes from RAM into variables in case the instructions need them
        self.alu("MUL", reg_a, reg_b)

    def SUB(self, reg_a, reg_b):
        # Use the two inputs and refer to "SUB" in our ALU function
        self.alu("SUB", reg_a, reg_b)

    def ADD(self, reg_a, reg_b):
        # Use the two inputs and refer to "ADD" in our ALU function
        self.alu("ADD", reg_a, reg_b)

    def NOP(self, reg_a, reg_b):
        pass

    def PUSH(self, reg_a, reg_b):
        reg_num = self.ram[reg_a]
        value = self.reg[reg_num]
        # Decrement the SP by 1
        self.reg[SP] -= 1
        top_of_stack_add = self.reg[SP]
        # Write the value in ram at pc to the stack, then save the value to the stack
        self.ram[top_of_stack_add] = value

    def POP(self, reg_a, reg_b):
        # We'll take from the stack and add it to reg
        top_of_stack_add = self.reg[SP]
        value = self.ram[top_of_stack_add]
        reg_num = self.ram[reg_a]
        self.reg[reg_num] = value
        # Increment the sp by 1
        self.reg[SP] += 1

    def CALL(self, reg_a, reg_b):
        return_addr = reg_b

        self.reg[SP] -= 1
        self.ram[self.reg[SP]] = return_addr

        reg_num = self.ram[reg_a]
        subroutine_addr = self.reg[reg_num]

        self.pc = subroutine_addr

    def RET(self, reg_a, reg_b):
        subroutine_addr = self.ram[self.reg[SP]]
        self.reg[SP] += 1
        self.pc = subroutine_addr

    def CMP(self, reg_a, reg_b):
        reg_num1 = self.reg[reg_a]
        reg_num2 = self.reg[reg_b]
        self.alu("CMP", reg_num1, reg_num2)

    def JMP(self, reg_a, reg_b):
        self.pc = self.reg[reg_a]

    def JEQ(self, reg_a, reg_b):
        if self.flag_reg[EQ] == 0b00000001:
            self.pc = self.reg[reg_a]
        else:
            self.pc += 2

    def JNE(self, reg_a, reg_b):
        if self.flag_reg[EQ] == 0b00000000:
            self.pc = self.reg[reg_a]
        else:
            self.pc += 2

    def run(self):
        # While true
        while self.running:
            # Our instruction register is going to read the memory address that's stored in register PC
            ir = self.ram_read(self.pc)
            pc_flag = (ir & 0b00010000) >> 4
            reg_num1 = self.ram[self.pc + 1]
            reg_num2 = self.ram[self.pc + 2]
            self.branch_table[ir](reg_num1, reg_num2)
            if pc_flag == 0:
                move = int((ir & 0b11000000) >> 6)
                self.pc += move + 1
