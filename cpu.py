import sys


class CPU:
    def __init__(self):
        self.running = False
        self.op_size = 0
        self.fl = {'l': 0, 'g': 0, 'e': 0}
        self.pc = 0
        self.sp = 7
        self.reg = [0] * 10
        self.reg[self.sp] = 0xF4
        self.ram = [0] * 256
        self.cmds = {
            0b10000010: self.LDI,
            0b01000111: self.PRN,
            0b00000001: self.HLT,
            0b10100010: self.MUL,
            0b01000101: self.PUSH,
            0b01000110: self.POP,
            0b01010000: self.CALL,
            0b00010001: self.RET,
            0b10100000: self.ADD,
            0b10100111: self.CMP,
            0b01010100: self.JMP,
            0b01010101: self.JEQ,
            0b01010110: self.JNE,
            0b10101011: self.XOR,
            0b10101000: self.AND,
            0b10101010: self.OR,
            0b01101001: self.NOT,
            0b10101100: self.SHL,
            0b10101101: self.SHR,
            0b10100100: self.MOD
        }

    def LDI(self, op1, op2):
        self.reg[op1] = op2

    def PRN(self, op1, op2):
        print(self.reg[op1])

    def HLT(self, op1, op2):
        self.running = False

    def ADD(self, op1, op2):
        self.alu('ADD', op1, op2)

    def MUL(self, op1, op2):
        self.alu('MUL', op1, op2)

    def AND(self, op1, op2):
        self.alu('AND', op1, op2)

    def OR(self, op1, op2):
        self.alu('OR', op1, op2)

    def XOR(self, op1, op2):
        self.alu('XOR', op1, op2)

    def NOT(self, op1, op2):
        self.alu('NOT', op1, op2)

    def SHL(self, op1, op2):
        self.alu('SHL', op1, op2)

    def SHR(self, op1, op2):
        self.alu('SHR', op1, op2)

    def MOD(self, op1, op2):
        if self.reg[op2] == 0:
            print('Error: MOD requires non-zero values')
            self.running = False
        else:
            self.alu('MOD', op1, op2)

    def CMP(self, op1, op2):
        self.alu('CMP', op1, op2)

    def JMP(self, op1, op2):
        self.pc = self.reg[op1]
        self.op_size = 0

    def JEQ(self, op1, op2):
        if self.fl['e'] == 1:
            self.pc = self.reg[op1]
            self.op_size = 0

    def JNE(self, op1, op2):
        if self.fl['e'] == 0:
            self.pc = self.reg[op1]
            self.op_size = 0

    def PUSH(self, op1, op2):
        self.reg[self.sp] -= 1
        self.ram_write(self.reg[self.sp], self.reg[op1])

    def POP(self, op1, op2):
        self.reg[op1] = self.ram_read(self.reg[self.sp])
        self.reg[self.sp] += 1

    def CALL(self, op1, op2):
        self.reg[self.sp] -= 1
        self.ram_write(self.reg[self.sp], self.pc + 2)

        self.pc = self.reg[op1]
        self.op_size = 0

    def RET(self, op1, op2):
        self.pc = self.ram_read(self.reg[self.sp])
        self.reg[self.sp] += 1

        self.op_size = 0

    def alu(self, op, reg_a, reg_b):
        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        elif op == "SUB":
            self.reg[reg_a] -= self.reg[reg_b]
        elif op == "MUL":
            self.reg[reg_a] *= self.reg[reg_b]
        elif op == "DIV":
            self.reg[reg_a] /= self.reg[reg_b]
        elif op == "CMP":
            if self.reg[reg_a] < self.reg[reg_b]:  # L
                self.fl = {'l': 1, 'g': 0, 'e': 0}
            elif self.reg[reg_a] > self.reg[reg_b]:  # G
                self.fl = {'l': 0, 'g': 1, 'e': 0}
            elif self.reg[reg_a] == self.reg[reg_b]:  # E
                self.fl = {'l': 0, 'g': 0, 'e': 1}
        elif op == 'AND':
            self.reg[reg_a] = self.reg[reg_a] & self.reg[reg_b]
        elif op == 'OR':
            self.reg[reg_a] = self.reg[reg_a] | self.reg[reg_b]
        elif op == 'XOR':
            self.reg[reg_a] = self.reg[reg_a] ^ self.reg[reg_b]
        elif op == 'NOT':
            self.reg[reg_a] = self.reg[reg_a] ^ 0b11111111
        elif op == 'SHL':
            self.reg[reg_a] = self.reg[reg_a] << self.reg[reg_b]
        elif op == 'SHR':
            self.reg[reg_a] = self.reg[reg_a] >> self.reg[reg_b]
        elif op == 'MOD':
            self.reg[reg_a] = self.reg[reg_a] % self.reg[reg_b]
        else:
            raise Exception("Unsupported ALU operation")

    def load(self, filename):
        MAR = 0

        try:
            with open(filename) as f:
                for line in f:
                    line = line.split('#')
                    n = line[0].strip()

                    if n == '':
                        continue

                    MDR = int(n, 2)

                    self.ram_write(MAR, MDR)
                    MAR += 1

        except FileNotFoundError:
            print(f"{sys.argv[0]}: {filename} not found")
            sys.exit(2)

    def ram_read(self, MAR):
        return self.ram[MAR]

    def ram_write(self, MAR, MDR):
        self.ram[MAR] = MDR
        return self.ram[MAR]

    def trace(self):
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

    def run(self):
        self.load(sys.argv[1])
        self.running = True

        op1 = self.ram_read(self.pc + 1)
        op2 = self.ram_read(self.pc + 2)

        while self.running:
            # FETCH
            cmd = self.ram_read(self.pc)

            op1 = self.ram_read(self.pc + 1)
            op2 = self.ram_read(self.pc + 2)

            self.op_size = (cmd >> 6) + 1

            # DECODE
            if cmd in self.cmds:

                # EXECUTE
                self.cmds[cmd](op1, op2)

            else:
                print(f"Invalid Instruction: {cmd:b}")
                self.running = False

            self.pc += self.op_size 