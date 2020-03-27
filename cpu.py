
import sys
from asm import OPCODES

# TODO
# run program commenting out lines that do stuff on the well_code to see what
# opcodes I need to implement;

class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        # initialize ram
        self.ram =  [0b00000000]*256
        # initialize register
        self.reg = [0b00000000]*8


        self._MAR_ = 1
        self._MDR_ = 1

        self.PC = 0
        self.IR = None

        self.FL = 0b00000000


    def load(self, filename=None):
        """Load a program into memory."""

        if filename:
            program = []
            f = open(filename, "r")
            for x in f:
                # ignore comments and newlines
                x = x.split('#')[0].strip()
                if x:
                    # cast binary string to int and append
                    # print(int(x,2))
                    program.append(int(x,2))
        
        else:
            # For now, we've just hardcoded a program:

            program = [
                # From print8.ls8
                0b10000010, # LDI R0,8
                0b00000000,
                0b00001000,
                0b01000111, # PRN R0
                0b00000000,
                0b00000001, # HLT
            ]

        address = 0

        for instruction in program:
            self.ram[address] = instruction
            address += 1

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]

        elif op == "LDI":
            self.reg[reg_a] = reg_b

        elif op == "PRN":
            print(self.reg[reg_a])
        
        elif op == "PRA":
            # print(reg_a)
            # print(type(reg_a))
            ascii = chr(self.reg[reg_a])
            print(ascii)
        
        elif op == "CMP":
            if self.reg[reg_a] > self.reg[reg_b]:
                self.FL = 0b00000010 #| self.Fl
            if self.reg[reg_a] < self.reg[reg_b]:
                self.FL = 0b00000100 #| self.FL
            if self.reg[reg_a] == self.reg[reg_b]:
                self.FL = 0b00000001 #| self.FL
        
        elif op == "JMP":
            self.PC = self.reg[reg_a]
        
        elif op == "JEQ":
            if self.FL == 0b00000001:
                self.PC = self.reg[reg_a]
            else:
                self.PC += 2
        
        elif op == "JNE":
            if bin(self.FL)[-1] == '0':
                self.PC = self.reg[reg_a]
            else:
                self.PC += 2

        #elif op == "SUB": etc
        else: 
            raise Exception(f"Unsupported ALU operation: {op}")

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

        jumps = ['JMP', 'JEQ', 'JNE']

        while True:

            opcode = self.ram_read(self.PC)

            instruction = None

            # find instruction and num_args matching opcode
            for key, value in OPCODES.items():
                # opcode formatted from number to string representing 8 bits
                if value['code'] == format(opcode, '#010b')[2:]:
                    instruction = key
                    # print('instruction: ', instruction)
                    num_args = value['type']
                    # print('Num_args: ', num_args)
                    break
            
            if instruction == 'LDI':
                num_args = 2

            # if instruction not in OPCODES, stop running
            if instruction == None:
                print('invalid opcode')
                return
            
            # if HLT, stop running
            if instruction == 'HLT':
                print('program halted')
                return

            self.IR = instruction

            args = [None]*2

            for i in range(num_args):
                arg = self.ram_read(self.PC+i+1)
                args[i] = arg

            # print('argument 1: ', args[0])
            # print('argument 2: ', args[1])

            # run subroutine
            self.alu(self.IR, args[0], args[1])


            if self.IR not in jumps:
            # Increment pc counter to move to next spot of RAM
                self.PC += (1+num_args)

    def ram_read(self, address = None):
        # if no address passed, set address to MAR
        if address == None:
            address = self._MAR_

        # check address is valid
        if address > 255 or address < 0:
            raise ValueError('Ram address must be in range [0-255]')

        byte = self.ram[address]
        # value = int("".join(str(x) for x in byte), 2) 

        return byte

    def ram_write(self, address=None, value=None,):
        # if no value passed, set value to MDR
        if value == None:
            value = self._MDR_
        # if no address passed, set address to MAR
        if address == None:
            address = self._MAR_

        # write value
        self.ram[address] = value
