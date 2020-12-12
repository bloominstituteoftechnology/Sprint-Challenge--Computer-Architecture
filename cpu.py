import sys

ltf = 0b100
gtf = 0b010
etf = 0b001
LDI = 0b10000010
PRNR0 = 0b01000111
ADD = 0b10100000
HLT = 0b00000001
POP = 0b01000110
CALL = 0b01010000
RET = 0b00010001
CMP = 0b10100111
JMP = 0b01010100
JEQ = 0b01010101
JNE = 0b01010110


class CPU:
    """
    Main CPU class.
    """

    def __init__(self):
        """
        Construct a new CPU.
        """
        self.ram = [0]*256
        self.reg = [0]*8
        self.pc = 0
        self.reg[7] = 0xF4
        self.flags = 0b00000001

    def load(self):
        """
        Load a program into memory.
        """
        address = 0
        if len(sys.argv) < 2:
            print('Remember to pass a second file')
            exit()

        file = sys.argv[1]

        try:
            with open(file, 'r')as f:
                for line in f:
                    if line != "\n" and line[0] != "#":
                        self.ram[address] = int(line[0:8], 2)
                        address += 1
        except FileNotFoundError:
            print(f'Error from {sys.argv[0]}: file{sys.argv[1]}not found.')
            exit()

    def ram_read(self, key):
        return self.ram[key]

    def ram_write(self, mar, mdr):
        mar %= 256

        self.ram[mar] = mdr

    def alu(self, op, reg_a, reg_b):
        """
        ALU operations.
        """

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        elif op == "SUB":
            self.reg[reg_a] -= self.reg[reg_b]
        elif op == "MUL":
            self.reg[reg_a] *= self.reg[reg_b]
        elif op == "DIV":
            self.reg[reg_a] /= self.reg[reg_b]
        elif op == "OR":
            self.reg[reg_a] |= self.reg[reg_b]
        elif op == "CMP":
            if self.reg[reg_a] < self.reg[reg_b]:
                self.flags = ltf
            elif self.reg[reg_a] > self.reg[reg_b]:
                self.flags = gtf
            else:
                self.flags = etf
        else:
            raise Exception("Unsupported ALU operation")

    def trace(self):
        """
        Print out the CPU state.
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

    def run(self):
        """
        Run the CPU.
        """
        running = True

        while running:
            command = self.ram[self.pc]
            op_a = self.ram_read(self.pc + 1)
            op_b = self.ram_read(self.pc + 2)
            alu = (command >> 5) & 0b001
            sets_pointer = (command >> 4) & 0b0001

            # LDI
            if command == 0b10000010:
                self.reg[self.ram_read(self.pc+1)] = self.ram_read(self.pc + 2)

            # PRN
            elif command == 0b01000111:
                print(self.reg[self.ram_read(self.pc + 1)])

            # MUL
            elif command == 0b10100010:
                self.alu("MUL", self.ram_read(self.pc + 1),
                         self.ram_read(self.pc + 2))
            elif command == 0b10100000:
                self.alu("ADD", op_a, op_b)

            # HLT
            elif command == 0b00000001:
                running = False

            # PUSH
            elif command == 0b01000101:
                self.reg[7] -= 1
                self.ram_write(self.reg[7], self.reg[op_a])

            # POP
            elif command == 0b01000110:
                value = self.ram[self.reg[7]]
                target_reg_address = self.ram[self.pc + 1]
                self.reg[target_reg_address] = value
                self.reg[7] += 1

            # CALL
            elif command == 0b01010000:
                self.reg[7] -= 1
                self.ram_write(self.reg[7], self.pc+2)
                self.pc = self.reg[op_a]

            # RET
            elif command == 0b00010001:
                self.pc = self.ram_read(self.reg[7])
                self.reg[7] += 1

            # CMP
            elif command == 0b10100111:
                self.alu("CMP", op_a, op_b)

            # JMP
            elif command == 0b01010100:
                self.pc = self.reg[op_a]

            # JEQ
            elif command == 0b01010101:
                if self.flags & etf:
                    self.pc = self.reg[op_a]
                else:
                    self.pc += 2

            # JNE
            elif command == 0b01010110:
                if not self.flags & etf:
                    self.pc = self.reg[op_a]
                else:
                    self.pc += 2
            else:
                running = False

            if command >> 4 & 0b0001:
                continue
            else:
                self.pc += (command >> 6) + 1