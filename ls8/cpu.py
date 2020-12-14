"""CPU functionality."""

import sys

class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.running = False
        self.ram = [00000000] * 256
        self.pc = 0
        self.reg = [0] * 8
        self.sp = 7
        #Commands - these come from the list below
    def call_stack(self, func):
        branch_table = {
            0b00000001 : self.HLT,
            0b01000111 : self.PRN,
            0b10000010 : self.LDI,
            0b10100010 : self.MUL,
            0b01000101: self.PUSH,
            0b01000110: self.POP,
            0b10100111: self.CMP,
            0b01010100: self.JMP,
            0b01010101: self.JEQ,
            0b01010110: self.JNE,
            
        }
        
        if func in branch_table:
            branch_table[func]()
        else:
            print('Unknown function')
            sys.exit(1)

    def load(self, f):
        """Load a program into memory."""

        file_path = f
        program = open(f"{file_path}", "r")
        address = 0
        for line in program:
            if line[0] ==  "0" or line[0] == "1":
                command = line.split('#', 1)[0]
                self.ram[address] = int(command, 2)
                address += 1


    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        #elif op == "SUB": etc
        elif op == "MUL":
            self.reg[self.ram[reg_a]] *= self.reg[self.ram[reg_b]]
        elif op == "CMP":
            if self.reg[reg_a] == self.reg[reg_b]:
                self.fl = self.fl | 1
            if self.reg[reg_a] > self.reg[reg_b]:
                self.fl = self.fl | 2
            if self.reg[reg_a] < self.reg[reg_b]:
                self.fl = self.fl | 4
        elif op == "MOD":
            self.reg[reg_a] %= self.reg[reg_b]
        elif op == "XOR":
            self.reg[reg_a] ^ self.reg[reg_b]
        elif op == "AND":
            self.reg[reg_a] & self.reg[reg_b]
        elif op == "OR":
            self.reg[reg_a] | self.reg[reg_b]
        elif op == "NOT":
            not self.reg[reg_a]
        elif op == "SHL":
            self.reg[reg_a] << self.reg[reg_b]
        elif op == "SHR":
            self.reg[reg_a] >> self.reg[reg_b]
        
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
    
    def ram_read(self, MAR):    #MAR = Memory Address Register
        MDR = self.ram[MAR]     #MDR = Memory Data Register
        return MDR
        
    
    def ram_write(self, MAR, MDR):
        self.ram[MAR] = MDR

    def run(self):
        """Run the CPU."""
        self.running = True

        while self.running:
            ir = self.ram_read(self.pc)
            self.call_stack(ir)

    def LDI(self):
        reg = self.ram_read(self.pc+1)
        val = self.ram_read(self.pc+2)
        self.reg[reg] = val
        self.pc += 3

    def PRN(self):
        reg = self.ram[self.pc + 1]
        print(self.reg[reg])
        self.pc += 2
    
    def HLT(self):
        self.running = False
        self.pc += 1
    
    def MUL(self):
        self.alu('MUL', self.pc+1, self.pc+2)
        self.pc += 3

    def PUSH(self):
        self.sp -=1
        reg_num = self.ram[self.pc + 1]
        value = self.reg[reg_num]
        self.ram[self.sp] = value
        self.pc += 2

    def POP(self):
        value = self.ram[self.sp]
        self.reg[self.ram[self.pc + 1]] = value
        self.sp += 1
        self.pc += 2

    def CMP(self):
        reg_1 = self.reg[self.ram[self.pc + 1]]
        reg_2 = self.reg[self.ram[self.pc + 2]]

        if reg_1 == reg_2:
            self.fl = 1
        else: 
            self.fl = 0
        
        self.pc+=3

    def JMP(self):
        self.pc = self.reg[self.ram[self.pc + 1]]
        return True
    
    def JEQ(self):

        a = self.fl
        if a == 1:
            self.pc = self.reg[self.ram[self.pc+1]]
        elif a == 0:
            self.pc += 2

    def JNE(self):
        a = self.fl
        if a == 0:
            self.pc = self.reg[self.ram[self.pc+1]]
        else: 
            self.pc += 2
    



        

    
