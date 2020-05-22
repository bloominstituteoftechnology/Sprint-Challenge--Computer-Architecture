import sys


class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.ram = [0] * 256
        self.pc = 0

        self.registers = [0, 0, 0, 0, 0, 0, 0, 0]
        self.flags = [0, 0, 0, 0, 0, 0, 0, 0]

        self.branch_table = {71: self.PRNT,
                             84: self.JMP,
                             85: self.JEQ,
                             86: self.JNE,
                             130: self.LDI,
                             167: self.CMP}

    def ram_write(self, address, value):
        self.ram[address] = value

    def ram_read(self, address):
        return self.ram[address]

    def LDI(self):
        reg_num = self.ram[self.pc + 1]
        value = self.ram[self.pc + 2]
        self.registers[reg_num] = value
        self.pc += 3

    def PRNT(self):
        reg_num = self.ram[self.pc + 1]
        print(self.registers[reg_num])
        self.pc += 2

    def CMP(self):
        self.alu("CMP", self.ram[self.pc+1], self.ram[self.pc+2])
        self.pc += 3

    def JMP(self):
        self.pc = self.registers[self.ram[self.pc + 1]]

    def JEQ(self):
        if self.flags[7] == 1:
            self.pc = self.registers[self.ram[self.pc + 1]]
        else:
            self.pc += 2

    def JNE(self):
        if self.flags[7] == 0:
            self.pc = self.registers[self.ram[self.pc + 1]]
        else:
            self.pc += 2

    def load(self):
        """Load a program into memory."""

        if len(sys.argv) != 2:
            print("Error")
            sys.exit(1)

        address = 0

        with open(sys.argv[1]) as f:
            for line in f:
                string_val = line.split("#")[0].strip()
                if string_val == '':
                    continue
                v = int(string_val, 2)
                self.ram[address] = v
                address += 1

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        elif op == "CMP":
            if self.registers[reg_a] < self.registers[reg_b]:
                self.flags[5] = 1
                self.flags[6] = 0
                self.flags[7] = 0
            elif self.registers[reg_a] > self.registers[reg_b]:
                self.flags[5] = 0
                self.flags[6] = 1
                self.flags[7] = 0
            elif self.registers[reg_a] == self.registers[reg_b]:
                self.flags[5] = 0
                self.flags[6] = 0
                self.flags[7] = 1
        else:
            raise Exception("Unsupported ALU operation")

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

    def run(self):
        """Run the CPU."""
        halted = False
        HALT = 1

        while not halted:
            instruction = self.ram[self.pc]

            if instruction == HALT:
                halted = True

            else:
                self.branch_table[instruction]()


cpu = CPU()

cpu.load()
cpu.run()
