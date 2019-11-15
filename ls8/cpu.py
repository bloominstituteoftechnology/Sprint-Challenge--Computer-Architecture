"""CPU functionality."""

import sys

class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.ram = [0] * 256
        self.pc = 0
        self.reg = [0] * 8
        self.flag = {}
        self.HLT = 0b00000001
        self.PRN = 0b01000111
        self.LDI = 0b10000010
        self.MUL = 0b10100010
        self.POP = 0b01000110
        self.PUSH = 0b01000101
        self.CALL = 0b01010000
        self.RET = 0b00010001
        self.ADD = 0b10100000
        self.CMP = 0b10100111
        self.JMP = 0b01010100
        self.JEQ = 0b01010101
        self.JNE = 0b01010110
        self.SP = 7
        self.reg[self.SP] = 0xF4

    def ram_read(self, MAR):
        value = self.ram[MAR]
        return value

    def ram_write(self, address, MDR):
        self.ram[address] = MDR

    def load(self):
        """Load a program into memory."""

        address = 0

        if len(sys.argv) != 2:
            print("usage: ls8.py filename")
            sys.exit(1)

        progname = sys.argv[1]

        with open(progname) as f:
            for line in f:
                line = line.split("#")[0]
                line = line.strip()
                
                if line == '':
                    continue

                val = int(line, 2)
                # print(val)

                self.ram[address] = val
                address += 1

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == self.ADD:
            self.reg[reg_a] += self.reg[reg_b]
        elif op == self.MUL:
            self.reg[reg_a] *= self.reg[reg_b]
        #elif op == "SUB": etc
        elif op == self.CMP:
            if self.reg[reg_a] == self.reg[reg_b]:
                self.flag['E'] = 1
            else:
                self.flag['E'] = 0
            if self.reg[reg_a] > self.reg[reg_b]:
                self.flag['G'] = 1
            else:
                self.flag['G'] = 0
            if self.reg[reg_a] < self.reg[reg_b]:
                self.flag['L'] = 1
            else:
                self.flag['L'] = 0
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
        halted = False
        while not halted:
            IR = self.ram[self.pc]
            operand_a = self.ram_read(self.pc + 1)
            operand_b = self.ram_read(self.pc + 2)
            
            
            if IR == self.LDI:
                # Set the value of a register to an integer.
                register_index = operand_a
                self.reg[register_index] = operand_b
                self.pc += 3
            # elif IR == OPCODES.NOP.code:
            #     self.pc += 1
            elif IR == self.PRN:
                # Print numeric value stored in the given register.
                # Print to the console the decimal integer value that is stored in the given
                # register.
                reg_index = operand_a
                value = int(self.reg[reg_index])
                print(f'{value}')
                self.pc += 2
            elif IR == self.MUL:
                self.alu(self.MUL, operand_a, operand_b)
                self.pc += 3
            elif IR == self.ADD:
                self.alu(self.ADD, operand_a, operand_b)
                self.pc += 3
            elif IR == self.HLT:
                halted = True
                self.pc += 1
            elif IR == self.CMP:
                self.alu(self.CMP, operand_a, operand_b)
                self.pc += 3
            elif IR == self.POP:
                #pop
                val = self.ram[self.reg[self.SP]]
                reg_num = operand_a
                self.reg[reg_num] = val

                self.reg[self.SP] += 1

                self.pc += 2
            elif IR == self.CALL:
                return_address = self.pc + 2
                self.reg[self.SP] -= 1
                self.ram[self.reg[self.SP]] = return_address

                reg_num = operand_a
                self.pc = self.reg[reg_num]
            elif IR == self.RET:
                self.pc = self.ram[self.reg[self.SP]]
                self.reg[self.SP] += 1
            elif IR == self.JMP:
                reg_num = operand_a
                self.pc = self.reg[reg_num]
            elif IR == self.JEQ:
                if self.flag['E'] == 1:
                    #jump to address stored in given register
                    reg_num = operand_a
                    self.pc = self.reg[reg_num]
                else:
                    self.pc += 2
            elif IR == self.JNE:
                if self.flag['E'] == 0:
                    reg_num = operand_a
                    self.pc = self.reg[reg_num]
                else:
                    self.pc += 2
            elif IR == self.PUSH:
                #push
                self.reg[self.SP] -= 1
                reg_num = operand_a
                reg_val = self.reg[reg_num]
                self.ram[self.reg[self.SP]] = reg_val

                self.pc += 2
            else:
                print(f'Unknown instruction at index {self.pc}')
                self.pc += 1