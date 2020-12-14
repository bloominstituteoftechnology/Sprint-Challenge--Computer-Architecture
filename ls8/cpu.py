"""CPU functionality."""

import sys

HLT = 0b00000001
LDI = 0b10000010
PRN = 0b01000111

# ALU
ADD = 0b10100000
SUB = 0b10100001
MUL = 0b10100010
DIV = 0b10100011

# Stack
POP = 0b01000110
PUSH = 0b01000101

# Subroutine
CALL = 0b01010000
RET = 0b00010001

SP = 7                          # stack pointer

# SPRINT
CMP = 0b10100111
JMP = 0b01010100
JEQ = 0b01010101
JNE = 0b01010110

# STRETCH
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
        """
        Construct a new CPU.
        Add list properties to the `CPU` class to hold 256 bytes of memory and 8
        general-purpose registers.
        """
        self.ram = [0] * 256                # 256 bytes of memory
        self.registers = [0] * 8            # 8 registers
        self.registers[7] = 0xF4
        self.pc = 0                         # program counter

        self.halted = False

        self.flag = 0
        self.inst_set_pc = False

        #  Table for fast lookups
        self.execute = {
            HLT: self.execute_HLT
            LDI: self.execute_LDI
            PRN: self.execute_PRN

            PUSH: self.execute_PUSH
            POP: self.execute_POP

            CALL: self.execute_CALL
            RET: self.execute_RET

            ADD: self.execute_ADD
            SUB: self.execute_SUB
            MUL: self.execute_MUL
            DIV: self.execute_DIV

            CMP: self.execute_CMP
            JMP: self.execute_JMP
            JEQ: self.execute_JEQ
            JNE: self.execute_JNE

            AND: self.execute_AND
            OR: self.execute_OR
            XOR: self.execute_XOR
            NOT: self.execute_NOT
            SHL: self.execute_SHL
            SHR: self.execute_SHR
            MOD: self.execute_MOD
        }


    def bit_mask(self):
        return ((self.ir >> 6) & 0b11) + 1

    def instruction_sets_pc(self):
        return ((self.ir >> 4) & 0b0001) == 1

    def load(self, filename):
        """Load a program into memory."""

        address = 0
        # open the file
        with open(filename) as my_file:
            # go through each line to parse and get
            # the instruction
            for line in my_file:
                # try and get the instruction/operand in the line
                comment_split = line.split("#")
                maybe_binary_number = comment_split[0].strip()
                if maybe_binary_number = '':
                    continue
                val = int(maybe_binary_number, 2)

                self.ram[address] = val
                address += 1


    def alu(self, op, reg_a, reg_b):
        """
        ALU operations
        Arithmetic Logic Operator
        """
        ops = {
            "ADD": lambda x, y: x + y,
            "SUB": lambda x, y: x - y,
            "MUL": lambda x, y: x * y,
            "DIV": lambda x, y: x / y,
            # Day 2 notes !!!
            "AND": lambda x, y: x & y,
            "OR": lambda x, y: x | y,
            "XOR": lambda x, y: x ^ y,
            "NOT": lambda x, y: x ~ y,
            "SHL": lambda x, y: x << y,
            "SHR": lambda x, y: x >> y,
            "MOD": lambda x, y: x % y,

        }
            try:
                if op in ops:
                    self.registers[reg_a] = ops[op](self.registers[reg_a], self.registers[reg_b])
                    return self.registers[reg_a]
                elif op = "CMP":
                """
                Compare the values in two registers.

                * If they are equal, set the Equal `E` flag to 1, otherwise set it to 0.
                * If registerA is less than registerB, set the Less-than `L` flag to 1,
                otherwise set it to 0.
                * If registerA is greater than registerB, set the Greater-than `G` flag
                to 1, otherwise set it to 0.
                """
                    if self.registers[reg_a] < self.registers[reg_b]:
                        self.flag = 0b00000100
                    elif self.registers[reg_a] < self.registers[reg_b]:
                        self.flag = 0b00000010
                    else:
                        self.flag = 0b00000001
        else:
            raise Exception("Unsupported ALU operation")

    def ram_read(self, mar):                    # MAR: Memory Address Register
        return self.ram[mar]                    # holds the memory address we're reading or writing

    def ram_write(self, mdr, mar):              # MDR`: Memory Data Register
        self.ram[mar] = mdr                     # holds the value to write or the value just read


    def execute_HLT(self, operand_a, operand_b):                      # Halt program
        """Halt the CPU (and exit the emulator)."""
        print("HLT")
        self.halted = True
        self.pc += self.bit_mask

    def execute_LDI(self, operand_a, operand_b):                     # store value in register
        """Set the value of a register to an integer."""
        print("LDI")
        self.registers[operand_a] = operand_b
        self.pc += self.bit_mask

    def execute_PRN(self, operand_a, operand_b):
        """
        Print numeric value stored in the given register.
        Print to the console the decimal integer value that is stored in the given
        register.
        """
        print("PRN")
        print(self.registers[operand_a])
        self.pc += self.bit_mask

    # ALU operations
    def execute_ADD(self, operand_a, operand_b):
        print("ADD")
        self.alu("ADD", operand_a, operand_b)

    def execute_SUB(self):
        print("SUB")
        self.alu("SUB", operand_a, operand_b)

    def execute_MUL(self, operand_a, operand_b):
        print("MUL")
        self.alu("MUL", operand_a, operand_b)

    def execute_DIV(self, operand_a, operand_b):
        print("DIV")
        self.alu("DIV", operand_a, operand_b)

    # SPRINT
    def execute_CMP(self, operand_a, operand_b):
        print("CMP")
        self.alu("CMP", operand_a, operand_b)

    # ALU STRETCH
    def execute_AND(self, operand_a, operand_b):
        print("AND")
        self.alu("AND", operand_a, operand_b)

    def execute_OR(self, operand_a, operand_b):
        print("OR")
        self.alu("OR", operand_a, operand_b)

    def execute_XOR(self, operand_a, operand_b):
        print("XOR")
        self.alu("XOR", operand_a, operand_b)

    def execute_NOT(self, operand_a, operand_b):
        print("NOT")
        self.alu("NOT", operand_a, operand_b)

    def execute_SHL(self, operand_a, operand_b):
        print("SHL")
        self.alu("SHL", operand_a, operand_b)

    def execute_SHR(self, operand_a, operand_b):
        print("SHR")
        self.alu("SHR", operand_a, operand_b)

    def execute_SHL(self, operand_a, operand_b):
        print("SHL")
        self.alu("SHL", operand_a, operand_b)

    # STACK
    def execute_PUSH(self, operand_a, operand_b):
        """
        Push the value in the given register on the stack.
        1. Decrement the `SP`.
        2. Copy the value in the given register to the address pointed to by
        `SP`.
        """
        print("PUSH")
        self.registers[SP] -= 1                # decrement by 1
        self.ram_write(self.registers[operand_a], self.registers[SP])
        self.pc += self.bit_mask

    def execute_POP(self, operand_a, operand_b):
        """
        Pop the value at the top of the stack into the given register.
        1. Copy the value from the address pointed to by `SP` to the given register.
        2. Increment `SP`.
        """
        print("POP")
        self.registers[operand_a] = self.ram_read(self.registers[SP])
        self.registers[SP] += 1
        self.pc += self.bit_mask

    # SUBROUTINES
    def execute_CALL(self, operand_a, operand_b):
        """
        Calls a subroutine (function) at the address stored in the register.

        1. The address of the ***instruction*** _directly after_ `CALL` is
        pushed onto the stack. This allows us to return to where we left off
        when the subroutine finishes executing.
        2. The PC is set to the address stored in the given register. We jump
        to that location in RAM and execute the first instruction in the subroutine.
        The PC can move forward or backwards from its current location.
        """
        print("CALL")
        self.registers[SP] -= 1
        self.ram_write(self.pc + self.bit_mask, registers[SP])
        self.pc = self.registers[operand_a]

    def execute_RET(self, operand_a, operand_b ):
        """
        Return from subroutine.
        Pop the value from the top of the stack and store it in the `PC`.
        """
        print("RET")
        self.registers[SP] += 1
        self.pc = self.registers[SP]


    # SPRINT
    def execute_JMP(self, operand_a, operand_b):
        """
        Jump to the address stored in the given register.
        Set the `PC` to the address stored in the given register.
        """
        self.pc = self.registers[operand_a]

    def execute_JEQ(self, operand_a, operand_b):
        """
        If `equal` flag is set (true), jump to the address stored in the given register.
        """
        if self.fl == 0b00000001:
            self.execute_JMP()
        else:
            self.pc += self.bit_mask

    def execute_JNE(self, operand_a, operand_b):
        if self.fl != 0b00000001:
            self.execute_JMP()
        else:
            self.pc += self.bit_mask

    # EXECUTE CODE
    def run(self):
        """Run the CPU."""
        while not self.halted:
            self.ir = self.ram_read(self.pc)         # Instruction Register (IR)

            if self.ir in self.execute:
                self.execute[self.ir](operand_a, operand_b)
            else:
                print(f"Error: Could not find instruction: {self.ir}")
                sys.exit(1)

            if not self.instruction_sets_pc:
                self.pc += self.bit_mask


    def trace(self):
        """
        Handy function to print out the CPU state. You might want to call this
        from run() if you need help debugging.
        """
        print(f"pc {bin(self.pc)}")
        print(f"memory address {self.ram_read(self.pc)}")
        print(f"operand_a {self.operand_a}")
        print(f"operand_b {self.operand_b}")


        print(f"TRACE: %02X | %02X %02X %02X |" % (
            self.pc,
            #self.fl,
            #self.ie,
            self.ram_read(self.pc),
            self.operand_a,
            self.operand_b
        ), end='')

        for i in range(8):
            print(" %02X" % self.registers[i], end='')

        print()
