import sys
import time
from key_interr import KBHit


class CPU:
    """Main CPU class."""

    def __init__(self):
        self.ram = [0] * 256
        self.reg = [0] * 8
        self.pc = 0
        self.__instruction_register__ = 0
        self.running = False
        self.im = 5  # iterrupt mask register
        self.isr = 6  # interrupt status register
        self.sp = 7  # our stack pointer starts at the top of a 0-7 index
        self.fl = 0b00000000  # all flags set to false on initialization
        self.reg[7] = 0xf4  # initialize stack pointer to RAM address f4
        self.__init_non_alu_opcodes__()  # initialize opcode map

    def __init_non_alu_opcodes__(self):
        self.branch_table = {
            0b01010000: self.CALL,
            0b00000001: self.HLT,
            0b01010010: self.INT,
            0b00010011: self.IRET,
            0b01010101: self.JEQ,
            0b01011010: self.JGE,
            0b01010111: self.JGT,
            0b01011001: self.JLE,
            0b01011000: self.JLT,
            0b01010100: self.JMP,
            0b01010110: self.JNE,
            0b10000011: self.LD,
            0b10000010: self.LDI,
            0b00000000: self.NOP,
            0b01000110: self.POP,
            0b01001000: self.PRA,
            0b01000111: self.PRN,
            0b01000101: self.PUSH,
            0b00010001: self.RET,
            0b10000100: self.ST,
        }

    # map the ALU opcodes to their commands
    ALU = {
        0b10100000: 'ADD',
        0b10101000: 'AND',
        0b10100111: 'CMP',
        0b01100110: 'DEC',
        0b10100011: 'DIV',
        0b01100101: 'INC',
        0b10100100: 'MOD',
        0b10100010: 'MUL',
        0b01101001: 'NOT',
        0b10101010: 'OR',
        0b10101100: 'SHL',
        0b10101101: 'SHR',
        0b10100001: 'SUB',
        0b10101011: 'XOR',
    }
    # map the ALU commands to their operations
    ALU_OP = {
        'ADD': lambda x, y: x + y,
        'AND': lambda x, y: x & y,
        'CMP': lambda x, y: 1 if x == y else 2 if x > y else 4,
        'DEC': lambda x, y: x - 1,
        'DIV': lambda x, y: x // y,
        'INC': lambda x, y: x + 1,
        'MOD': lambda x, y: x % y,
        'MUL': lambda x, y: x * y,
        'NOT': lambda x, y: ~x,
        'OR': lambda x, y: x | y,
        'SHL': lambda x, y: x << y,
        'SHR': lambda x, y: x >> y,
        'SUB': lambda x, y: x - y,
        'XOR': lambda x, y: x ^ y,
    }
    # add the ability to load a file as a second argument from the command line

    def load(self, filename):
        """Load a program into memory."""
        file_path = sys.argv[1]
        program = open(f"{file_path}", "r")
        address = 0
        for line in program:
            if line[0] == "0" or line[0] == "1":
                command = line.split("#", 1)[0]
                self.ram[address] = int(command, 2)
                address += 1

    def ram_read(self, mar):
        """Returns a byte from ram."""
        self.mar = mar
        self.mdr = self.ram[self.mar]
        return self.mdr

    def ram_write(self, mar, mdr):
        """Writes a byte to ram."""
        self.mar = mar
        self.mdr = mdr
        self.ram[self.mar] = self.mdr

    def alu(self, op, reg_a, reg_b):
        try:
            x = self.reg[reg_a]
            y = self.reg[reg_b] if reg_b is not None else None
            result = self.ALU_OP[op](x, y)

            if op == 'CMP':
                self.fl = result
            else:
                self.reg[reg_a] = result
                self.reg[reg_b] &= 0xFF
        except Exception:
            raise SystemError("Unsupported ALU operation")

    def run(self):
        """Run the CPU."""
        self.running = True
        old_time = new_time = time.time()
        kb = KBHit()
        # kb = KBHit()
        while self.running:
            if self.reg[self.im]:
                self.check_inter()

            new_time = time.time()
            if new_time - old_time > 1:
                self.reg[self.isr] |= 0b00000001
                old_time = new_time

            if kb.kbhit():
                c = kb.getch()
                self.ram_write(self.sp, ord(c[0]))
                self.reg[self.isr] |= 0b00000010

            # initialize intruction register and operands (if there are any)
            self.ir = self.ram_read(self.pc)
            if self.ir & 0b100000 > 0:
                # ALU operation
                self.alu(self.ALU[self.ir], self.operand_a, self.operand_b)
            else:
                # non-ALU opcode
                self.branch_table[self.ir]()

            # if instruction does not modify program counter
            if self.ir & 0b10000 == 0:
                # move to next instruction
                self.pc += 1

        kb.set_normal_term()

    def trace(self):
        """
        Handy function to print out the CPU state. You might want to call this
        from run() if you need help debugging.
        """

        print(f"TRACE: %02X | %02X %02X %02X |" % (
            self.pc,
            # self.fl,
            self.ram_read(self.pc),
            self.ram_read(self.pc + 1),
            self.ram_read(self.pc + 2)
        ), end='')

        for i in range(8):
            print(" %02X" % self.reg[i], end='')

        print()

########
    def check_inter(self):
        interrupts = self.reg[self.im] & self.reg[self.isr]
        for interrupt in range(8):
            bit = 1 << interrupt
            #if an interrupt is triggered
            if interrupts & bit:
                # save the old interrupt state
                self.old_im = self.reg[self.im]
                # disable interrupts
                self.reg[self.im] = 0
                # clear the interrupt
                self.reg[self.isr] &= (255 ^ bit)
                # decrement the stack pointer
                self.reg[self.sp] -= 1
                # push the pc to the stack
                self.ram_write(self.reg[self.sp], self.pc)
                #decrement the stack pointer
                self.reg[self.sp] -= 1
                # push the flags to the stack
                self.ram_write(self.reg[self.sp], self.fl)
                # push the registers to the stack R0-R6
                for i in range(7):
                    self.reg[self.sp] -= 1
                    self.ram_write(self.reg[self.sp], self.reg[i])
                self.pc = self.ram[0xF8 + interrupt]
                # break out and stop checking interrupts
                break

##-------------BRANCH TABLE--------------------------------##
    def CALL(self):  # Opcode: 01010000 'CALL register'  calls a subroutine
        self.reg[self.sp] -= 1
        self.ram_write(self.reg[self.sp], self.pc + 1)
        self.pc = self.reg[self.operand_a]

    def HLT(self):  # Opcode: 00000001 'HLT' stops the cpu and exits the emulator
        self.running = False
        # self.pc += 1

    def INT(self):  # Opcode: 01010010 'INT register'  issues the interrupt # stored in the register
        self.reg[self.IS] != (1 << self.reg[self.operand_a])
        self.pc += 1

    def IRET(self):  # Opcode: 00010011 'IRET' the return from an interrupt handler
        # pop R6-R0
        for i in range(6, -1, -1):
            self.reg[i] = self.ram_read(self.reg[self.sp])
            self.reg[self.sp] += 1
        # pop the flags
        self.fl = self.ram_read(self.reg[self.sp])
        self.reg[self.sp] += 1
        # pop program counter
        self.pc = self.ram_read(self.reg[self.sp])
        self.reg[self.sp] += 1
        # re enable the interrupts
        self.reg[self.im] = self.old_im

    def JEQ(self):  # Opcode: 01010101 'JEQ register' Jump if the equal flag is set to true
        if self.fl & 0b1:
            self.pc = self.reg[self.operand_a]
        else:
            self.pc += 1

    def JGE(self):  # Opcode: 01011010 'JGE register' Jump if the greater than or equal flag is set to true
        if self.fl & 0b11:
            self.pc = self.reg[self.operand_a]
        else:
            self.pc += 1

    def JGT(self):  # Opcode: 01010111 'JGT register' Jump if the greater than flag is set to true
        if self.fl & 0b10:
            self.pc = self.reg[self.operand_a]
        else:
            self.pc += 1

    def JLE(self):  # Opcode: 01011001 'JLE register' Jump if the less than or equal flag is set to true
        if self.fl & 0b101:
            self.pc = self.reg[self.operand_a]
        else:
            self.pc += 1

    def JLT(self):  # Opcode: 01011000 'JLT register' Jump if the less than flag is set to true
        if self.fl & 0b100:
            self.pc = self.reg[self.operand_a]
        else:
            self.pc += 1

    def JMP(self):  # Opcode: 01010100 'JMP register' Jump to the address in the given register
        self.pc = self.reg[self.operand_a]

    def JNE(self):  # Opcode: 01010110 'JNE register' Jump if the not equal flag is set to false
        if not self.fl & 0b1:
            self.pc = self.reg[self.operand_a]
        else:
            self.pc += 1

    def LD(self):  # Opcode: 10000011 'LD registerA registerB' Load reg a with value stored in reg b mem address
        self.reg[self.operand_a] = self.ram_read(self.reg[self.operand_b])

    # Opcode: 10000010 'LDI registerA registerB' Set the value of a register to an integer.
    def LDI(self):
        self.reg[self.operand_a] = self.operand_b

    def NOP(self):  # Opcode: 00000000 'NOP'  No operation
        pass

    def POP(self):  # Opcode: 01000110 'POP register' Pop the top of the stack (value into register)
        self.reg[self.operand_a] = self.ram_read(self.reg[self.sp])
        self.reg[self.sp] += 1

    def PRA(self):  # Opcode: 01001000 'PRA register' Print the alpha (character) value stored in the register
        print(chr(self.reg[self.operand_a]), end='', flush=True)

    def PRN(self):  # Opcode: 01000111 'PRN register' Print the number value stored in the register
        print(self.reg[self.operand_a])

    def PUSH(self):  # Opcode: 01000101 'PUSH register' Push the value in the register to the stack
        self.reg[self.sp] -= 1
        self.ram_write(self.reg[self.sp], self.reg[self.operand_a])

    def RET(self):  # Opcode: 00010001 'RET' Return from a subroutine
        self.pc = self.ram_read(self.reg[self.sp])
        self.reg[self.sp] += 1

    def ST(self):  # Opcode: 10000100 'ST registerA registerB' Store the value in reg b to the address of reg a
        self.ram_write(self.reg[self.operand_a], self.reg[self.operand_b])

##----------------------End of BRANCH TABLE----------------------------##

    @property
    def ir(self):
        return self.__instruction_register__

    @ir.setter
    def ir(self, opcode):
        self.__instruction_register__ = opcode
        # load potential operands
        self.operand_a = self.ram_read(
            self.pc + 1) if opcode >> 6 > 0 else None
        self.operand_b = self.ram_read(
            self.pc + 2) if opcode >> 6 > 1 else None
        # move program counter past any operands
        self.pc += (opcode >> 6)

    @ir.deleter
    def ir(self):
        self.__instruction_register__ = 0
