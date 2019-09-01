"""CPU functionality."""

import sys

#global operations
#ALU ops
ADD = 0b10100000 # 00000aaa 00000bbb
SUB = 0b10100001 # 00000aaa 00000bbb
MUL = 0b10100010 # 00000aaa 00000bbb
DIV = 0b10100011 # 00000aaa 00000bbb
MOD = 0b10100100 # 00000aaa 00000bbb
INC = 0b01100101 # 00000rrr
DEC = 0b01100110 # 00000rrr
CMP = 0b10100111 # 00000aaa 00000bbb
AND = 0b10101000 # 00000aaa 00000bbb
NOT = 0b01101001 # 00000rrr
OR  = 0b10101010 # 00000aaa 00000bbb
XOR = 0b10101011 # 00000aaa 00000bbb
SHL = 0b10101100 # 00000aaa 00000bbb
SHR = 0b10101101 # 00000aaa 00000bbb
ALU_OP = [ADD, SUB, MUL, DIV, MOD, INC, DEC, CMP, AND, NOT, OR, XOR, SHL, SHR]

## PC mutators
CALL = 0b01010000 # 00000rrr
RET  = 0b00010001 #
INT  = 0b01010010 # 00000rrr
IRET = 0b00010011 #
JMP  = 0b01010100 # 00000rrr
JEQ  = 0b01010101 # 00000rrr
JNE  = 0b01010110 # 00000rrr
JGT  = 0b01010111 # 00000rrr
JLT  = 0b01011000 # 00000rrr
JLE  = 0b01011001 # 00000rrr
JGE  = 0b01011010 # 00000rrr

## Other
NOP = 0b00000000
HLT = 0b00000001 
LDI  = 0b10000010 # 00000rrr iiiiiiii
LD   = 0b10000011 # 00000aaa 00000bbb
ST   = 0b10000100 # 00000aaa 00000bbb
PUSH = 0b01000101 # 00000rrr
POP  = 0b01000110 # 00000rrr
PRN  = 0b01000111 # 00000rrr
PRA  = 0b01001000 # 00000rrr

F3 = 0b11110011

class CPU:
    """Main CPU class."""

    def __init__(self):
        self.ram = [0] * 256
        self.reg = [0] * 8
        self.pc = 0
        self.SP = 0
        self.halt = False
        self.FL = 0b00000000

        self.reg[7] = 0xF3

        self.ops = {
                HLT: self.hlt,
                LDI: self.reg_add,
                PRN: self.print_num,
                MUL: self.mul,
                ADD: self.add,
                PUSH: self.push,
                POP: self.pop,
                CALL: self.call,
                RET: self.ret,
                JMP: self.jump,
                CMP: self.comp,
                JEQ: self.jeq,
                JNE: self.jne
            }
    def hlt(self, MAR, MDR):
        self.halt = not self.halt

    def print_num(self,MAR, MDR):
        print(self.reg[MAR])

    def ram_read(self, MAR):
        return self.ram[MAR]

    def ram_write(self, MAR, MDR):
        self.ram[MAR] = MDR

    def reg_add(self, MAR, MDR):
        self.reg[MAR] = MDR

    def mul(self, operand_a, operand_b):
        self.alu("MUL", operand_a, operand_b)

    def add(self, operand_a, operand_b):
        self.alu("ADD", operand_a, operand_b)

    def push(self, MAR, MDR):
        self.reg[7] = (self.reg[7] - 1) % 255
        self.SP = self.reg[7]
        self.ram[self.SP] = self.reg[MAR]

    def pop(self, MAR, MDR):
        self.SP = self.reg[7]
        self.reg[MAR] = self.ram[self.SP]
        self.reg[7] = (self.reg[7] +1) % 255

    def call(self, operand_a, operand_b):
        self.push(operand_a, operand_b)
        self.ram[self.SP] = self.pc + 2
        self.pc = self.reg[operand_a]

    def ret(self):
        self.pc = self.ram[self.SP]

    def jump(self, MAR, MDR):
        self.pc = self.reg[MAR]

    def comp(self, a, b):
        a = self.reg[a]
        b = self.reg[b]

        self.FL = 0b00000000
        if a == b:
            self.FL = 0b00000001
        else:
            self.FL = 0b00000000

    def jeq(self, MAR, MDR):
        if self.FL == 0b00000001:
            self.jump(MAR, MDR)
        else:
            self.pc += 2

    def jne(self, MAR, MDR):
        if self.FL != 0b00000001:
            self.jump(MAR, MDR)
        else:
            self.pc += 2


    def load(self):
        address = 0
        
        try:
            with open(sys.argv[1]) as file:
                for line in file:
                    if line[0].startswith('0') or line[0].startswith('1'):
                        num = line.split('#')[0]
                        num = num.strip()
                        self.ram[address] = int(num, 2)
                        address += 1
        except FileNotFoundError:
            print(f"{sys.argv[0]}: {sys.argv[1]} Not found")
            sys.exit()


    def alu(self, op, reg_a, reg_b):
        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        # elif op == "SUB": etc
        elif op == "MUL":
            self.reg[reg_a] *= self.reg[reg_b]
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
        self.reg[7] = F3
        self.SP = self.reg[7]

        while not self.halt:
            IR = self.ram[self.pc]
            operand_a = self.ram_read(self.pc + 1)
            operand_b = self.ram_read(self.pc + 2)

            op_a = self.ram_read(self.pc + 1)
            op_b = self.ram_read(self.pc + 2)

            op_size = IR >> 6

            inst_set = ((IR >> 4) & 0b1) == 1

            if IR in self.ops:
                self.ops[IR](operand_a, operand_b)
            else:
                print(f"Error: Instruction {IR} not found")
                exit()
            if inst_set == False:
                self.pc += op_size + 1 



