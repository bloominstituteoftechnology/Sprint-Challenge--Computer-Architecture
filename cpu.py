"""CPU functionality."""

import sys

# move this list outside of the class to call later
HLT = 0b00000001
LDI = 0b10000010
PRN = 0b01000111 
PUSH = 0b01000101 
POP = 0b01000110 
MUL = 0b10100010
ADD = 0b10100000
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
        self.ram = [0] * 256
        self.reg = [0] * 8
        self.fl = [0] * 8
        self.pc = 0
        self.SP = 0x07

        # create dispatch table
        self.dsptchtbl = {
            LDI: self.LDI,
            PRN: self.PRN,
            PUSH: self.PUSH,
            POP: self.POP,
            MUL: self.MUL,
            ADD: self.ADD,
            CALL: self.CALL,
            RET: self.RET,
            CMP: self.CMP,
            JMP: self.JMP,
            JEQ: self.JEQ,
            JNE: self.JNE
        }

    def ram_read(self, MAR):
        # should find and return value at given address in memory
        # _Memory Address Register_ (MAR)
        return self.ram[MAR]

    def ram_write(self, MAR, MDR):
        # should find info at given address in memory and write/overwrite it
        # _Memory Data Register_ (MDR)
        self.ram[MAR] = MDR

    def load(self):
        """Load a program into memory."""

        address = 0
        if len(sys.argv) < 2:
            print("A program name is required.")
            sys.exit()

        with open(sys.argv[1]) as f:
            for line in f:
                # for each line in file, as long as the line is not a comment or empty, run each line
                if line[0] != '#' and line !='\n':
                    self.ram[address] = int(line[0:8], 2)
                    address += 1
            f.closed


    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        elif op == "MUL":
            self.reg[reg_a] *= self.reg[reg_b]
        elif op == "CMP":
            self.fl[0] = 0 # G flag
            self.fl[1] = 0 # L flag
            self.fl[2] = 0 # E flag
            if self.reg[reg_a] > self.reg[reg_b]:
                self.fl[0] = 1
            elif self.reg[reg_a] < self.reg[reg_b]:
                self.fl[1] = 1
            else:
                self.fl[2] = 1
        elif op == "JEQ":
            pass
        elif op == "JNE":
            pass
        #elif op == "SUB": etc
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

    def LDI(self, operand_a, operand_b):
        self.reg[operand_a] = operand_b
        self.pc += 3
    def PRN(self, operand_a, operand_b):
        print(self.reg[operand_a])
        self.pc += 2
    def MUL(self, operand_a, operand_b):
        self.alu("MUL", operand_a, operand_b)
        self.pc += 3
    def ADD(self, operand_a, operand_b):
        self.alu("ADD", operand_a, operand_b)
        self.pc += 3
    def PUSH(self, operand_a, operand_b):
        self.SP -= 1
        self.ram[self.SP] = self.reg[operand_a]
        self.pc += 2
    def POP(self, operand_a, operand_b):
        self.reg[operand_a] = self.ram[self.SP]
        self.SP += 1
        self.pc += 2
    def CALL(self, operand_a, operand_b):
        ret_add = self.pc + 2
        self.reg[self.SP] -= 1
        self.ram[self.reg[self.SP]] = ret_add
        self.pc = self.reg[operand_a]
    def RET(self, operand_a, operand_b):
        ret_add = self.ram[self.reg[self.SP]]
        self.reg[self.SP] += 1
        self.pc = ret_add
    # COMPARE:
    def CMP(self, operand_a, operand_b):
        self.alu("CMP", operand_a, operand_b)
        self.pc += 3
    def JMP(self, operand_a, operand_b):
        pass
    def JEQ(self, operand_a, operand_b):
        pass
    def JNE(self, operand_a, operand_b):
        pass




    def run(self):
        """Run the CPU."""
        running = True
        while running:
            # the _Instruction Register_, local variable
            IR = self.ram[self.pc]
            # reads the next two pieces of data
            operand_a = self.ram_read(self.pc + 1)
            operand_b = self.ram_read(self.pc + 2)

            if IR == HLT:
                running = False
            else:
                self.dsptchtbl[IR](operand_a, operand_b)
