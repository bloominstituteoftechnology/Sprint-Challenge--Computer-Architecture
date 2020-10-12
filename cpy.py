"""CPU functionality."""

import sys

# FL bits: 00000LGE
# L Less-than: during a CMP, set to 1 if registerA is less than registerB, zero otherwise.
# G Greater-than: during a CMP, set to 1 if registerA is greater than registerB, zero otherwise.
# E Equal: during a CMP, set to 1 if registerA is equal to registerB, zero otherwise.
LESS_FL_CMP = 0b00000100
GREATER_FL_CMP = 0b00000010
EQUAL_FL_CMP = 0b00000001  # Why it's equal to the HTL?


# Operation Codes
# OP Codes
LDI = 0b10000010
PRN = 0b01000111
HLT = 0b00000001
MUL = 0b10100010
PUSH = 0b01000101
POP = 0b01000110
CMP = 0b10100111
JMP = 0b01010100
JEQ = 0b01010101
JNE = 0b01010110
AND = 0b10101000
OR = 0b10101010
XOR = 0b10101011
NOT = 0b01101001
SHL = 0b10101100
SHR = 0b10101101
MOD = 0b10100100


class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.ram = 256 * [0]
        self.reg = 8 * [0]
        self.pc = 0
        self.running = True
        self.branchtable = {
            LDI: self.call_LDI,
            PRN: self.call_PRN,
            HLT: self.call_HLT,
            MUL: self.call_MUL,
            PUSH: self.call_PUSH,
            POP: self.call_POP,
            CMP: self.call_CMP,
            JMP: self.call_JMP,
            JEQ: self.call_JEQ,
            JNE: self.call_JNE,
            AND: self.call_AND,
            OR: self.call_OR,
            XOR: self.call_XOR,
            NOT: self.call_NOT,
            SHL: self.call_SHL,
            SHR: self.call_SHR,
            MOD: self.call_MOD,
        }
        self.sp = 7
        self.reg[self.sp] = len(self.ram)
        self.fl = {
            "equal": 0,
            "greater": 0,
            "less": 0,
        }

    def ram_read(self, address):
        return self.ram[address]

    def ram_write(self, address, value):
        self.ram[address] = value

    def load(self):
        """Load a program into memory."""
        if len(sys.argv) != 2:
            print("Usage: ls8.py filename")
            sys.exit(1)

        address = 0
        file = sys.argv[1]

        try:
            with open(file) as f:
                for line in f:
                    split_line = line.split("#")

                    code_value = split_line[0].strip()
                    if code_value == "":
                        continue

                    num = int(code_value, 2)
                    self.ram[address] = num
                    address += 1

        except FileNotFoundError:
            print(f"{sys.argv[1]} file not found")
            sys.exit(2)

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]

        elif op == "MUL":
            self.reg[reg_a] *= self.reg[reg_b]

        elif op == "CMP":
            if reg_a < reg_b:
                self.fl["less"] = 1
            elif reg_a > reg_b:
                self.fl["greater"] = 1
            elif reg_a == reg_b:
                self.fl["equal"] = 1

        elif op == "AND":
            operand_a = self.reg[reg_a]
            operand_b = self.reg[reg_b]
            self.reg[reg_a] = operand_a & operand_b

        elif op == "OR":
            operand_a = self.reg[reg_a]
            operand_b = self.reg[reg_b]
            self.reg[reg_a] = operand_a | operand_b

        elif op == "XOR":
            operand_a = self.reg[reg_a]
            operand_b = self.reg[reg_b]
            self.reg[reg_a] = operand_a ^ operand_b

        elif op == "NOT":
            self.reg[reg_a] = ~self.reg[reg_a]

        elif op == "SHL":
            operand_a = self.reg[reg_a]
            operand_b = self.reg[reg_b]
            self.reg[reg_a] = operand_a << operand_b

        elif op == "SHR":
            operand_a = self.reg[reg_a]
            operand_b = self.reg[reg_b]
            self.reg[reg_a] = operand_a >> operand_b

        elif op == "MOD":
            operand_a = self.reg[reg_a]
            operand_b = self.reg[reg_b]
            self.reg[reg_a] = operand_a % operand_b

        else:
            raise Exception("Unsupported ALU operation")

    def trace(self):
        """
        Handy function to print out the CPU state. You might want to call this
        from run() if you need help debugging.
        """

        print(f"TRACE: %02X | %02X %02X %02X |" % (
            self.pc,
            self.fl,
            # self.ie,
            self.ram_read(self.pc),
            self.ram_read(self.pc + 1),
            self.ram_read(self.pc + 2)
        ), end='')

        for i in range(8):
            print(" %02X" % self.reg[i], end='')

        print()

    def call_LDI(self):
        # address
        operand_a = self.ram_read(self.pc + 1)
        # value
        operand_b = self.ram_read(self.pc + 2)
        self.reg[operand_a] = operand_b
        self.pc += 3

    def call_PRN(self):
        # address
        operand_a = self.ram_read(self.pc + 1)
        self.pc += 2
        print(self.reg[operand_a])

    def call_MUL(self):
        # first register
        operand_a = self.ram_read(self.pc + 1)
        # second register
        operand_b = self.ram_read(self.pc + 2)
        self.pc += 3
        self.alu("MUL", operand_a, operand_b)

    def call_HLT(self):
        self.running = False

    def call_PUSH(self):
        given_register = self.ram[self.pc + 1]
        value_in_register = self.reg[given_register]
        self.reg[self.sp] -= 1
        self.ram[self.reg[self.sp]] = value_in_register
        self.pc += 2

    def call_POP(self):
        given_register = self.ram[self.pc + 1]
        value_from_ram = self.ram[self.reg[self.sp]]
        self.reg[given_register] = value_from_ram
        self.reg[self.sp] += 1
        self.pc += 2

    def call_CMP(self):
        # first register
        operand_a = self.reg[self.ram_read(self.pc + 1)]
        # second register
        operand_b = self.reg[self.ram_read(self.pc + 2)]
        self.pc += 3
        self.alu("CMP", operand_a, operand_b)

    def call_JMP(self):
        # address
        self.pc = self.ram_read(self.pc + 1)

    def call_JEQ(self):
        if self.fl["equal"] == 1:
            operand_a = self.ram_read(self.pc + 1)
            self.pc = self.reg[operand_a]
        else:
            self.pc += 2

    def call_JNE(self):
        if self.fl["equal"] == 0:
            operand_a = self.ram_read(self.pc + 1)
            self.pc = self.reg[operand_a]
        else:
            self.pc += 2

    def call_AND(self):
        # first register
        operand_a = self.ram_read(self.pc + 1)
        # second register
        operand_b = self.ram_read(self.pc + 2)
        self.pc += 3
        self.alu("AND", operand_a, operand_b)

    def call_OR(self):
        # first register
        operand_a = self.ram_read(self.pc + 1)
        # second register
        operand_b = self.ram_read(self.pc + 2)
        self.pc += 3
        self.alu("OR", operand_a, operand_b)

    def call_XOR(self):
        # first register
        operand_a = self.ram_read(self.pc + 1)
        # second register
        operand_b = self.ram_read(self.pc + 2)
        self.pc += 3
        self.alu("XOR", operand_a, operand_b)

    def call_NOT(self):
        operand_a = self.ram_read(self.pc + 1)
        self.pc += 2
        self.alu("NOT", operand_a, 0)

    def call_SHL(self):
        # first register
        operand_a = self.ram_read(self.pc + 1)
        # second register
        operand_b = self.ram_read(self.pc + 2)
        self.pc += 3
        self.alu("SHL", operand_a, operand_b)

    def call_SHR(self):
        # first register
        operand_a = self.ram_read(self.pc + 1)
        # second register
        operand_b = self.ram_read(self.pc + 2)
        self.pc += 3
        self.alu("SHR", operand_a, operand_b)

    def call_MOD(self):
        # first register
        operand_a = self.ram_read(self.pc + 1)
        # second register
        operand_b = self.ram_read(self.pc + 2)
        if operand_b == 0:
            print("You can't divide on '0'! ")
            self.call_HLT()
        else:
            self.pc += 3
            self.alu("MOD", operand_a, operand_b)

    def run(self):
        """Run the CPU."""
        while self.running:
            instruction = self.ram[self.pc]

            if instruction in self.branchtable:
                self.branchtable[instruction]()
            else:
                sys.exit(1)
