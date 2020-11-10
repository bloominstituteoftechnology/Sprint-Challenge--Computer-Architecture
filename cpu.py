import sys


LDI = 0b10000010
CMP = 0b10100111
JMP = 0b01010100
JEQ = 0b01010101
JNE = 0b01010110
PRN = 0b01000111
EE = 0b00000111


class CPU:

    # create new CPU
    def __init__(self):
        self.ram = [0] * 256
        self.reg = [0] * 8
        self.flag_reg = [0] * 8
        self.pc = 0
        self.FL = 0b00000000
        self.bt = {LDI: self.LDI,
                   CMP: self.CMP,
                   JMP: self.JMP,
                   JEQ: self.JEQ,
                   JNE: self.JNE,
                   PRN: self.PRN
                   }

        self.running = True
    # store memory

    def ram_read(self, address):
        return self.ram[address]

    def ram_write(self, address, value):
        self.ram[address] = value

    # load memory
    def load(self):
        address = 0
        program = []

        with open(sys.argv[-1]) as f:
            for line in f:
                try:
                    line = line.split("#", 1)[0]
                    line = int(line, 2)
                    program.append(line)
                except ValueError:
                    continue

        for instruction in program:
            self.ram[address] = instruction
            address += 1

    # alu
    def alu(self, op, reg_a, reg_b):
        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        elif op == "CMP":
            if reg_a == reg_b:
                self.flag_reg[EE] = 0b00000001
            elif reg_a < reg_b:
                self.flag_reg[L] = 0b00000100
            elif reg_b > reg_b:
                self.flag_reg[G] = 0b00000010
        else:
            raise Exception("Unsupported ALU operation")

    # print CPU
    def trace(self):
        """
       Handy function to print out the CPU state. You might want to call this
       from run() if you need help debugging.
       """
        print(f"TRACE: %02X | %02X %02X %02X |" % (
            self.pc,
            # self.fl,
            # self.ie,
            self.ram_read(self.pc),
            self.ram_read(self.pc + 1),
            self.ram_read(self.pc + 2)
        ), end='')

        for i in range(8):
            print(" %02X" % self.reg[i], end='')

        print()

    def LDI(self, reg_a, reg_b):
        self.reg[reg_a] = reg_b

    def PRN(self, reg_a, reg_b):
        print(self.reg[reg_a])

    def HLT(self, reg_a, reg_b):
        self.running = False

    def CMP(self, reg_a, reg_b):
        register_1 = self.reg[reg_a]
        register_2 = self.reg[reg_b]
        self.alu("CMP", register_1, register_2)

    def JMP(self, reg_a, reg_b):
        self.pc = self.reg[reg_a]

    def JEQ(self, reg_a, reg_b):
        if self.flag_reg[EE] == 0b000001:
            self.pc = self.reg[reg_a]
        else:
            self.pc += 2

    def JNE(self, reg_a, reg_b):
        if self.flag_reg[EE] == 0b000000:
            self.pc = self.reg[reg_a]
        else:
            self.pc += 2

    def run(self):
        """Run the CPU."""

        # while self.running:

            # ir = self.ram_read(self.pc)
            # pc_flag = (ir & 0b00010000) >> 4
            # register_1 = self.ram[self.pc + 1]
            # register_2 = self.ram[self.pc + 2]
            # self.bt[ir](register_1, register_2)
            # if pc_flag == 0:
            #     move = int((ir & 0b11000000) >> 6)
            #     self.pc += move + 1
           
        while not self.running:
                ir = self.ram_read(self.pc)
                reg_a = self.ram_read( self.pc + 1 )
                reg_b = self.ram_read( self.pc + 2 )
                self.bt(ir, reg_a, reg_b)
           
          

