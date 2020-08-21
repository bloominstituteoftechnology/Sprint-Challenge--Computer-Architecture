"""CPU functionality."""

import sys

# re
CALL = 0b01010000
ADD = 0b10100000
DIV = 0b10100011
MUL = 0b10100010
SUB = 0b10100001
LDI = 0b10000010
POP = 0b01000110
PUSH = 0b01000101
PRA = 0b01001000
PRN = 0b01000111
RET = 0b00010001
ST = 0b10000100
CMP = 0b10100111
JEQ = 0b01010101
JMP = 0b01010100
JNE = 0b01010110
HLT = 0b00000001
SP = 7
FL = 0

class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.ram = [0] * 256
        self.pc = 0
        self.reg = [0] * 8
        self.FL = 0
        self.branchtable = {
            CALL: self.call,
            ADD : self.alu,
            DIV : self.alu,
            MUL : self.alu,
            SUB : self.alu,
            LDI : self.ldi,
            POP : self.pop,
            PUSH : self.push,
            PRA : self.pra,
            PRN : self.prn,
            RET : self.ret,
            ST :  self.st,
            CMP : self.alu,
            JEQ : self.jeq,
            JMP : self.jmp,
            JNE : self.jne,
            HLT : self.hlt
           
        }
    
    def load(self, filename):
        """Load a program into memory."""
        address = 0
        # enumerate is cool, but this is syntax I'm most comfortable with
        with open(filename) as file:
            for line in file:
                line = line.split("#")
                try:
                    v = int(line[0], 2)
                except ValueError:
                    continue
                self.ram[address] = v
                address += 1

    def call(self, reg_a, reg_b=None):
        # subroutine called at reg[reg_a]
        # sp-=
        self.reg[SP] -= 1
        # come back
        self.ram_write(self.reg[SP], self.pc + 2)
        self.pc = self.reg[reg_a]

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == ADD:
            self.reg[reg_a] += self.reg[reg_b]
            print(f"ADD at REG[{self.pc + 1}]: {self.reg[self.pc + 1]}")

        elif op == SUB:
            self.reg[reg_a] -= self.reg[reg_b]
            print(f"SUB at REG[{self.pc + 1}]: {self.reg[self.pc + 1]}")

        elif op == DIV:
            if self.reg[reg_b] != 0:
                self.reg[reg_a] /= self.reg[reg_b]
                print(f"DIV at REG[{self.pc + 1}]: {self.reg[self.pc + 1]}")
            else:
                raise Exception("Unsupported DIV operation")

        elif op == MUL:
            self.reg[reg_a] *= self.reg[reg_b]
            print(f"MUL at REG[{self.pc + 1}]: {self.reg[self.pc + 1]}")

        else:
            raise Exception("Unsupported ALU operation")

    def ram_read(self, address):
        return self.ram[address]

    def ram_write(self, value, address):
        self.ram[address] = value

    def bitwise_addition(self, num1, num2): # recursive
        if num2 <= 0:
            return num1
        else: #                          sum of bits   common bits and shift
            return self.bitwise_addition(num1 ^ num2, (num1 & num2) << 1)


    def run(self):
        """Run the CPU.       
        1. read mem at PC
        2. store result in local var
        3. turn into hash_tables
        """
        
        while True:
            ir = self.ram[self.pc]
            reg_a = self.ram_read(self.pc + 1)
            reg_b = self.ram_read(self.pc + 2)
            # pc updater if no set pc given
            update = (ir >> 6) + 1
            # is alu? 1
            alu_op = ((ir >> 5) & 0b1)
            # incase pc will be set
            set_pc = ((ir >> 4) & 0b1)
    
            if ir in self.branchtable:
                if alu_op:
                    # handle for ability to pass in type of operation 
                    self.branchtable[ir](ir, reg_a, reg_b)
                else:
                    # always pass in reg_a, reg_b don't always use reg_b, can just move pc later
                    self.branchtable[ir](reg_a, reg_b)
            else:
                print('Unsupported command')
            if not set_pc:
                self.pc += update

    def trace(self):
        """
        Handy function to print out the CPU state. You might want to call this
        from run() if you need help debugging.
        """

        print(f"TRACE: %02X | %02X %02X %02X %02X |" % (
            self.pc,
            self.FL,
            #self.ie,
            self.ram_read(self.pc),
            self.ram_read(self.pc + 1),
            self.ram_read(self.pc + 2)
        ), end='')

        for i in range(8):
            print(" %02X" % self.reg[i], end='')

        print()

    def ldi(self, reg_a, reg_b):
        self.reg[reg_a] = reg_b 
        #self.PC += 1
    
    def pop(self, reg_a, reg_b=None):
        #get value at memory[sp]
        value = self.ram_read(self.reg[SP])
        #set reg to value
        self.reg[reg_a] = value
        #sp +1
        self.reg[SP] += 1
        return value

    def push(self, reg_a, reg_b=None):
        # given value pushed onto stack, change pointer
        self.reg[SP] -= 1
        self.ram_write(self.reg[SP], self.reg[reg_a])

    def pra(self, reg_a, reg_b=None):
        # ascii
        print(chr(self.reg[reg_a]))

    def prn(self, reg_a, reg_b=None):
        # decimal
        print("Returning", self.reg[reg_a])

    def ret(self, reg_a=None, reg_b=None):
        # pop and store in pc
        self.pc = self.ram_read(self.reg[SP])
        self.reg[SP] += 1

    def st(self, reg_a, reg_b):
        self.ram_write(self.reg[reg_a], self.reg[reg_b])

    def hlt(self, reg_a=None, reg_b=None):
        # no more running, just exit
        sys.exit()

    def jeq(self,ops):
        if self.FL == 1:
            self.MAR = self.ram_read(self.PC + 1)
            self.PC = self.reg[self.MAR]
        else:
            self.PC = self.bitwise_addition(self.PC, ops)

    def jge(self,ops):
        if self.FL == 1:
            self.MAR = self.ram_read(self.PC + 1)
            self.PC = self.reg[self.MAR]
        else:
            self.PC = self.bitwise_addition(self.PC, ops)

    def jmp(self,ops):
        if self.FL == 1:
            self.MAR = self.ram_read(self.PC + 1)
            self.PC = self.reg[self.MAR]
        else:
            self.PC = self.bitwise_addition(self.PC, ops)

    def jne(self,ops):
        if self.FL != 1 and not 0:
            self.MAR = self.ram_read(self.PC + 1)
            self.PC = self.reg[self.MAR]
        else:
            self.PC = self.bitwise_addition(self.PC, ops)
