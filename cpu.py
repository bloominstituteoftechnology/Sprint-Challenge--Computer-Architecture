"""CPU functionality."""

import sys

HLT = 0b00000001
LDI = 0b10000010
PRN = 0b01000111
MUL = 0b10100010
POP = 0b01000110
PUSH = 0b01000101
CALL = 0b01010000
RET = 0b00010001
ADD = 0b10100000
CMP = 0b10100111
JMP = 0b01010100
JEQ = 0b01010101
JNE = 0b01010110
SP = 7

class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.ram = [0] * 256
        self.pc = 0
        self.reg = [0] * 8
        self.fl = 0
        self.branchtable = {}
        self.branchtable[HLT] = self.handle_hlt
        self.branchtable[LDI] = self.handle_ldi
        self.branchtable[PRN] = self.handle_prn
        self.branchtable[MUL] = self.handle_mul
        self.branchtable[POP] = self.handle_pop
        self.branchtable[PUSH] = self.handle_push
        self.branchtable[ADD] = self.handle_add
        self.branchtable[CALL] = self.handle_call
        self.branchtable[RET] = self.handle_ret
        self.branchtable[CMP] = self.handle_cmp
        self.branchtable[JMP] = self.handle_jmp
        self.branchtable[JEQ] = self.handle_jeq
        self.branchtable[JNE] = self.handle_jne
        self.reg[SP] = 0xf4
        self.halted = False

    def load(self):
        """Load a program into memory."""

        if len(sys.argv) != 2:
            sys.exit(1)
        progname = sys.argv[1]
        mar = 0

        with open(progname) as f:
            for line in f:
                line = line.split("#")[0]
                line = line.strip()

                if line == "":
                    continue

                mdr = int(line, 2)

                self.ram[mar] = mdr
                mar += 1

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        #elif op == "SUB": etc
        elif op == "MUL":
            self.reg[reg_a] *= self.reg[reg_b]
        elif op == "CMP":
            if self.reg[reg_a] > self.reg[reg_b]:
                self.fl = 0b00000010
            if self.reg[reg_a] < self.reg[reg_b]:
                self.fl = 0b00000100
            if self.reg[reg_a] == self.reg[reg_b]:
                self.fl = 0b00000001
        else:
            raise Exception("Unsupported ALU operation")

    def trace(self):
        """
        Handy function to print out the CPU state. You might want to call this
        from run() if you need help debugging.
        """

        print(f"TRACE: %02X | %02X %02X %02X |" % (
            self.pc,
            self.ram_read(self.pc),
            self.ram_read(self.pc + 1),
            self.ram_read(self.pc + 2)
        ), end='')

        for i in range(8):
            print(" %02X" % self.reg[i], end='')

        print()

    def handle_hlt(self):
        self.halted = True

    def handle_ldi(self):
        reg_number = self.ram_read(self.pc + 1)
        value = self.ram_read(self.pc + 2)
        self.reg[reg_number] = value

    def handle_prn(self):
        reg_number = self.ram_read(self.pc + 1)
        print(self.reg[reg_number])

    def handle_add(self):
        number1 = self.ram_read(self.pc + 1)
        number2 = self.ram_read(self.pc + 2)
        self.alu("ADD", number1, number2)

    def handle_mul(self):
        number1 = self.ram_read(self.pc + 1)
        number2 = self.ram_read(self.pc + 2)
        self.alu("MUL", number1, number2)

    def handle_pop(self):
        value = self.ram[self.reg[SP]]
        reg_number = self.ram_read(self.pc + 1)
        self.reg[reg_number] = value
        self.reg[SP] += 1

    def handle_push(self):
        self.reg[SP] -= 1
        reg_number = self.ram_read(self.pc + 1)
        value = self.reg[reg_number]
        self.ram[self.reg[SP]] = value

    def handle_call(self):
        ret_address = self.pc + 2
        self.reg[SP] -= 1
        self.ram[self.reg[SP]] = ret_address

        reg_number = self.ram_read(self.pc + 1)
        self.pc = self.reg[reg_number]

    def handle_ret(self):
        self.pc = self.ram[self.reg[SP]]
        self.reg[SP] += 1

    def handle_cmp(self):
        number1 = self.ram_read(self.pc + 1)
        number2 = self.ram_read(self.pc + 2)
        self.alu("CMP", number1, number2)

    def handle_jmp(self):
        reg_number = self.ram_read(self.pc + 1)
        self.pc = self.reg[reg_number]

    def handle_jeq(self):
        reg_number = self.ram_read(self.pc + 1)
        if self.fl == 0b00000001:
            self.pc = self.reg[reg_number]
        else:
            self.pc += 2

    def handle_jne(self):
        reg_number = self.ram_read(self.pc + 1)
        if self.fl != 0b00000001:
            self.pc = self.reg[reg_number]
        else:
            self.pc += 2

    def ram_read(self, mar):
        return self.ram[mar]

    def ram_write(self, mdr, mar):
        self.ram[mar] = mdr

    def run(self):
        """Run the CPU."""

        while self.halted != True:
            instruction = self.ram[self.pc]
            op_count = instruction >> 6
            instruction_length = op_count + 1
            self.branchtable[instruction]()

            if instruction == 0 or None:
                print(f"Unknown instructions at index {self.pc}")
                sys.exit(1)

            if instruction != 80 and instruction != 17 and instruction != 84 and instruction != 85 and instruction != 86:
                self.pc += instruction_length