"""CPU functionality."""

import sys

class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""

        # registers R0 thru R7
        # R5 is reserved as the interrupt mask (IM)
        # R6 is reserved as the interrupt status (IS)
        # R7 is reserved as the stack pointer (SP)
        self.registers = [0b0] * 8

        # internal registers
        self.pc = 0 # PC: Program Counter, address of the currently executing instruction
        self.ir = None # IR: Instruction Register, contains a copy of the currently executing instruction
        self.mar = None # MAR: Memory Address Register, holds the memory address we're reading or writing
        self.mdr = None # MDR: Memory Data Register, holds the value to write or the value just read
        self.fl = None # FL: Flags
        self.eq = None # equal flag
        self.lt = None # less than flag
        self.gt = None # greater than flag
        self.spl = None # stack pointer location
        #
        self.ram = [0b0] * 0xFF
        self.spl = 8 - 1
        self.registers[self.spl] = 0xF4

        # opcodes
        self.OPCODES = {0b10000010: 'LDI',
                        0b01000111: 'PRN',
                        0b00000001: 'HLT',
                        0b10100010: 'MUL',
                        0b01000110: 'POP',
                        0b01000101: 'PUSH',
                        0b10000100: 'ST',
                        0b10100111: 'CMP',
                        0b01010100: 'JMP',
                        0b01010110: 'JNE',
                        0b01010101: 'JEQ',
        }

    def load(self, filename: str):
        """Load a program into memory."""

        try:
            with open(filename, 'r') as f:
                lines = (line for line in f.readlines() if not (line[0]=='#' or line[0]=='\n'))
                program = [int(line.split('#')[0].strip(), 2) for line in lines]

            address = 0

            for instruction in program:
                self.ram[address] = instruction
                address += 1
        except FileNotFoundError as e:
            print(e)
            sys.exit()

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        # addition
        if op == "ADD":
            self.registers[reg_a] += self.registers[reg_b]

        # multiplication
        elif op == "MUL":
            self.registers[reg_a] *= self.registers[reg_b]

        elif op == 'CMP':

            a = self.registers[reg_a]
            b = self.registers[reg_b]

            if a==b:
                self.eq, self.lt, self.gt = (1, 0, 0)
            elif a<b:
                self.eq, self.lt, self.gt = (0, 1, 0)
            elif a>b:
                self.eq, self.lt, self.gt = (0, 0, 1)
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
            print(" %02X" % self.registers[i], end='')

        print()

    def run(self):
        """Run the CPU."""
        running = True
        while running:
            # self.trace()
            self.ir = self.ram[self.pc]
            try:
                op = self.OPCODES[self.ir]
                # do LDI
                if op == 'LDI':
                    reg = self.ram[self.pc+1]
                    val = self.ram[self.pc+2]
                    self.registers[reg] = val
                    self.pc += 3

                # do Print
                elif op == 'PRN':
                    reg = self.ram[self.pc+1]
                    val = self.registers[reg]
                    print(f"hex val: {val:x}\tdec val: {val}\tbin val: {val:b}")
                    self.pc += 2

                # pass to alu
                elif op == 'ADD' or op == 'MUL' or op == 'CMP':
                    reg_a = self.ram[self.pc+1]
                    reg_b = self.ram[self.pc+2]
                    self.alu(op, reg_a, reg_b)
                    self.pc += 3

                # push
                elif op == 'PUSH':

                    reg = self.ram[self.pc + 1]
                    val = self.registers[reg]
                    #if val in self.OPCODES.keys():
                    #    print("hi")
                    #Decrement the SP.
                    self.registers[self.spl] -= 1
                    # Copy the value in the given register to the address pointed to by SP.
                    self.ram[self.registers[self.spl]] = val
                    self.pc += 2

                # pop
                elif op == 'POP':
                    reg = self.ram[self.pc + 1]
                    val = self.ram[self.registers[self.spl]]
                    # Copy the value from the address pointed to by SP to the given register.
                    self.registers[reg] = val
                    # Increment SP.
                    self.registers[self.spl] += 1
                    self.pc += 2

                # ST
                elif op == 'ST':
                    # Store value in registerB in the address stored in registerA.
                    # This opcode writes to memory.
                    reg_a = self.ram[self.pc + 1]
                    reg_b = self.ram[self.pc + 2]
                    address_a = self.registers[reg_a]
                    val_b = self.registers[reg_b]
                    self.ram[addres_a] = val_b
                    self.pc += 2

                elif op == 'JMP':
                    # Jump to the address stored in the given register.
                    self.reg = self.ram[self.pc + 1]
                    val = self.registers[self.reg]
                    # Set the PC to the address stored in the given register.
                    self.pc = val

                elif op == 'JEQ':
                    if self.eq == 1:
                        self.reg = self.ram[self.pc + 1]
                        val = self.registers[self.reg]
                        self.pc = val
                    else:
                        self.pc += 2

                elif op == 'JNE':
                    if self.eq == 0:
                        self.reg = self.ram[self.pc + 1]
                        val = self.registers[self.reg]
                        self.pc = val
                    else:
                        self.pc += 2
                # exit
                elif op == 'HLT':
                    running = False
                    # self.pc += 1 # i don't know if it makes sense to do this.


            except KeyError as e:
                print(f"unknown command {self.ir}")
                self.pc += 1
        pass

    def ram_read(self, location):
        """ read from ram

            accept the address to read and return the value stored there
        """
        return self.ram[location]

    def ram_write(self, location, value):
        """ write to ram

        accept a value to write, and the address to write it to.
        """
        self.ram[location] = value
        pass
