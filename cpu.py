"""CPU functionality."""

import sys


class CPU:
    """Main CPU class."""

    def __init__(self):

        #ram, register, and program counter
        self.ram = [0] * 256
        self.reg = [0] * 8  # R0 - R7
        self.pc = 0
        self.flag = 0
        self.running = True

        #ALU units
        self.ADD = 0b10100000
        self.SUB = 0b10100001
        self.MUL = 0b10100010
        self.DIV = 0b10100011
        self.MOD = 0b10100100

        self.INC = 0b01100101
        self.DEC = 0b01100110

        self.CMP = 0b10100111

        self.AND = 0b10101000
        self.NOT = 0b01101001
        self.OR  = 0b10101010
        self.XOR = 0b10101011
        self.SHL = 0b10101100
        self.SHR = 0b10101101

        # branch table
        self.HLT = 0b00000001
        self.LDI = 0b10000010
        self.PRN = 0b01000111
        self.PUSH = 0b01000101
        self.POP = 0b01000110
        self.RET = 0b00010001
        self.CALL = 0b01010000
        self.JMP = 0b01010100
        self.JEQ = 0b01010101
        self.JNE = 0b01010110
        self.branchtable = {
            self.HLT: self.handle_HLT,
            self.LDI: self.handle_LDI,
            self.PRN: self.handle_PRN,
            self.PUSH: self.handle_PUSH,
            self.POP: self.handle_POP,
            self.RET: self.handle_RET,
            self.CALL: self.handle_CALL,
            self.JMP: self.handle_JMP,
            self.JEQ: self.handle_JEQ,
            self.JNE: self.handle_JNE,
        }
        self.stack_pointer = 0xf4
        self.reg[7] = self.stack_pointer

    # branch table methods
    def handle_HLT(self, *args):
        self.running = False

    def handle_LDI(self, op_a, op_b):
        self.reg[op_a] = op_b
        self.pc += 3

    def handle_PRN(self, op_a, op_b):
        print(self.reg[op_a])
        self.pc += 2

    def handle_MUL(self, op_a, op_b):
        self.alu('MUL', op_a, op_b)

    def handle_PUSH(self, a=None, b=None):
        # decrement stack pointer
        self.reg[self.stack_pointer] -= 1

        # get register number and stored value
        reg_num = a
        
        val = self.reg[reg_num]

        # store value
        self.ram[self.reg[self.stack_pointer]] = val
        self.pc += 2

    def handle_POP(self, a=None, b=None):
        # get value
        address = self.reg[self.stack_pointer]
        val = self.ram[address]

        # store at specified register
        reg_num = a
        self.reg[reg_num] = val

        # increment stack_pointer and pc
        self.reg[self.stack_pointer] += 1
        self.pc +=2

    def handle_RET(self, a=None, b=None):
        # pop return address from top of stack
        return_address = self.ram[self.reg[self.stack_pointer]]
        self.reg[self.stack_pointer] += 1

        # Set the pc
        self.pc = return_address


    def handle_CALL(self, a, b=None):
        # Get the address directly after the call
        return_address = self.pc + 2

        # Push on the stack
        self.reg[self.stack_pointer] -= 1
        self.ram[self.reg[self.stack_pointer]] = return_address

        # Set the PC to the value in the given register
        reg_num = self.ram[self.pc + 1]
        dest_addr = self.reg[reg_num]

        self.pc = dest_addr


    def handle_CMP(self, a, b=None):
        pass


    def handle_JMP(self, a, b=None):
        self.pc = self.reg[a]


    def handle_JEQ(self, a, b=None):
        if self.flag & 0b00000001:
            self.pc = self.reg[a]
        else:
            self.pc += 2


    def handle_JNE(self, a, b=None):
        if self.flag != 0b00000001:
            self.pc = self.reg[a]
        else:
            self.pc += 2


    def load(self, filename):
        """Load a program into memory."""

        try:
            print(f"loading {filename}")
            address = 0
            with open(filename, "r") as f:
                for line in f:
                    line = line.split('#')
                    temp = line[0].strip()
                    if temp:
                        value = int(temp, 2)
                        self.ram[address] = value
                        address += 1
        
        except FileNotFoundError:
            print(f"{sys.argv[0]}: {filename} not found")

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == self.ADD:  # ADD
            self.reg[reg_a] += self.reg[reg_b]
        elif op == self.MUL:  # MUL
            self.reg[reg_a] *= self.reg[reg_b]
        elif op == self.SUB:  # SUB
            self.reg[reg_a] -= self.reg[reg_b]
        elif op == self.DIV:  # DIV
            self.reg[reg_a] /= self.reg[reg_b]
        elif op == self.MOD:
            self.reg[reg_a] %= self.reg[reg_b]
        elif op == self.CMP:  # CMP
            if self.reg[reg_a] < self.reg[reg_b]:
                self.flag = 0b00000100
            elif self.reg[reg_a] > self.reg[reg_b]:
                self.flag = 0b00000010
            elif self.reg[reg_a] == self.reg[reg_b]:
                self.flag = 0b00000001
            else:
                self.flag = 0b00000000
        elif op == self.AND:
            self.reg[reg_a] &= self.reg[reg_b]
        elif op == self.OR:
            self.reg[reg_a] |= self.reg[reg_b]
        elif op == self.XOR:
            self.reg[reg_a] ^= self.reg[reg_b]
        elif op == self.NOT:
            self.reg[reg_a] = ~self.reg[reg_b]
        elif op == self.SHL:
            self.reg[reg_a] <<= self.reg[reg_b]
        elif op == self.SHR:
            self.reg[reg_a] >>= self.reg[reg_b]
        else:
            raise Exception("Unsupported ALU operation")

    def trace(self, LABEL=str()):
        """
                        Handy function to print out the CPU state. You might want to call this
                        from run() if you need help debugging.
                        """

        print(
            f"{LABEL} %02X | %02X %02X %02X |"
            % (
                self.pc,
                self.ram_read(self.pc),
                self.ram_read(self.pc + 1),
                self.ram_read(self.pc + 2),
            ),
            end='',
        )
        for i in range(8):
            print(" %02i" % self.reg[i], end='')
        print(" | Stack:", end="")

        for i in range(240, 244):
            print(" %02i" % self.ram_read(i), end="")

        print()

    def ram_read(self, MAR):
        """Should accept the address to read and return the value stored there"""
        return self.ram[MAR]

    def ram_write(self, MDR, MAR):
        """Should accept a value to write, and the address to write it to."""
        self.ram[MAR] = MDR


    def run(self):
        """Run the CPU."""

        while self.running:
            # read the memory address that's stored in register PC and store that result in IR, Instruction Register
            ir = self.ram[self.pc]
            register_a = self.ram_read(self.pc + 1)
            register_b = self.ram_read(self.pc + 2)
            use_alu = (ir & 0b00100000) >> 5

            if use_alu:
                self.alu(ir, register_a, register_b)
                self.pc += 3
            elif self.branchtable.get(ir):
                self.branchtable[ir](register_a, register_b)
            else:
                print("Unknown instruction")
                self.running = False

            # self.trace()