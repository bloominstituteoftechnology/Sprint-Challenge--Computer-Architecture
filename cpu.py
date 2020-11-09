"""CPU functionality."""

import sys
import os.path
#these are from the ls8-cheatsheet
HLT = 0b00000001
LDI = 0b10000010
PRN = 0b01000111
MUL = 0b10100010

PUSH = 0b01000101
POP = 0b01000110

CALL = 0b01010000
RET = 0b00010001
ADD = 0b10100000
#these are for the sprint
CMP = 0b10100111
JMP = 0b01010100
JEQ = 0b01010101
JNE = 0b01010110

class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.ram = [0] * 256 # 256 ram can only be as 0-355
        self. pc = 0 #program counter
        self.reg = [0] * 8 # 8 bit regester
        self.mar = 0 #memory address register memory address to read or write
        self.mdr = 0 #memory data register holds the value to write or read
        self.flag = 0 #flag register hold flag status
        self.halted = False
        self.running = True

        # SPpoints at the value at the top of the stack (most recently pushed), or at address F4 ifempty.
        self.reg[7] = 0xF4  # 244 # int('F4', 16)
        self.ir = 0
        # Setup Branch Table
        self.branchtable = {}
        self.branchtable[HLT] = self.hlt
        self.branchtable[LDI] = self.ldi
        self.branchtable[PRN] = self.prn
        self.branchtable[MUL] = self.mul
        self.branchtable[PUSH] = self.push
        self.branchtable[POP] = self.pop
        self.branchtable[CALL] = self.call
        self.branchtable[ADD] = self.add
        self.branchtable[RET] = self.ret
        self.branchtable[JMP] = self.jmp
        self.branchtable[CMP] = self.cmp
        self.branchtable[JEQ] = self.jeq
        self.branchtable[JNE] = self.jne

        # Property wrapper

    @property
    def sp(self):
        return self.reg[7]

    @sp.setter
    def sp(self, a):
        self.reg[7] = a & 0xFF



    @property
    def operand_a(self):
        return self.ram_read(self.pc + 1)

    @property
    def operand_b(self):
        return self.ram_read(self.pc + 2)

    @property
    def instruction_size(self):
        return ((self.ir >> 6) & 0b11) + 1

    @property
    def instruction_sets_pc(self):
        return ((self.ir >> 4) & 0b0001) == 1



    def ram_read(self, mar):
        if mar >= 0 and mar < len(self.ram):
            return self.ram[mar]
        else:
            print(f"Error:{mar}")
            return -1

    def ram_write(self, mar, mdr):
        if mar >= 0 and mar < len(self.ram):
            self.ram[mar] = mdr & 0xFF
        else:
            print(f"Error:{mdr}")

    def load(self, file_name):
        """Load a program into memory."""

        address = 0

        file_path = os.path.join(os.path.dirname(__file__), file_name)
        try:
            with open(file_path) as f:
                for line in f:
                    num = line.split("#")[0].strip()  # "10000010"
                    try:
                        instruction = int(num, 2)
                        self.ram[address] = instruction
                        address += 1
                    except:
                        continue
        except:
            print(f'Could not find file named: {file_name}')
            sys.exit(1)


    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
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

    def run(self):
        """Run the CPU."""


        while not self.halted:
            #the next instruction
            self.ir = self.ram_read(self.pc)

            # does the instruction
            if self.ir in self.branchtable:
                self.branchtable[self.ir]()
            else:
                print(f"Error: not in {self.ir} in branch table.")
                sys.exit(1)

            # add ti counter if necessary
            if not self.instruction_sets_pc:
                self.pc += self.instruction_size

    def hlt (self):
        self.halted = True

    def ldi (self):
        self.reg[self.operand_a] = self.operand_b

    def prn(self ):
        print(self.reg[self.operand_a])

    def mul(self):
        self.reg[self.operand_a] *= self.reg[self.operand_b]

    def push(self, reg_num, b=None):
        self.sp -= 1
        self.mdr = self.reg[reg_num]
        self.ram_write(self.sp, self.mdr)

    def pop(self, dest_reg_num, b=None):
        self.mdr = self.ram_read(self.sp)
        self.reg[dest_reg_num] = self.mdr
        self.sp += 1

    def call(self, dest_reg_num, b=None):
        self.sp -=1
        self.ram_write(self.sp, self.pc + self.instruction_size())
        self.pc = self.reg[dest_reg_num]

    def ret(self, a=None, b=None):
        self.mdr = self.ram_read(self.sp)
        self.pc = self.mdr
        self.sp += 1

    def add(self):
        self.reg[self.operand_a] += self.reg[self.operand_b]

    def cmp(self):
        if self.reg[self.operand_a] < self.reg[self.operand_b]:
            self.flag = 0b00000100 # If registerA is less than registerB, set the Less-than `L` flag to 1

        elif self.reg[self.operand_a] > self.reg[self.operand_b]:
            self.flag = 0b00000010 #If registerA is greater than registerB, set the Greater-than `G` flag to 1
        else:
            self.flag = 0b00000001# otherwise set it to 0.
    def jeq(self):
        if self.flag == 0b00000001:#If `equal` flag is set (true), jump to the address stored in the given register.
            self.jmp()
        else:
            self.pc += self.instruction_size

    def jmp(self):#Jump to the address stored in the given register.Set the `PC` to the address stored in the given register.
        self.pc = self.reg[self.operand_a]

    def jne(self):#If `E` flag is clear (false, 0), jump to the address stored in the given register.
        if self .flag != 0b00000001:
            self.jmp()
        else:
            self.pc += self.instruction_size