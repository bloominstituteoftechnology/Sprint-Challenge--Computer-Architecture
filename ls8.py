"""CPU functionality."""

import sys

LDI = 0b10000010
PRN = 0b01000111
HLT = 0b00000001
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

class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.ram = [0] * 256
        self.register = [0] * 8
        self.pc = 0
        self.register[7] = 255
        self.sp = self.register[7]
        self.flag = 0b00000000

    def ram_read(self, address):
        return self.ram[address]

    def raw_write(self, address, value):
        self.ram[address] = value

    def load(self):
        """Load a program into memory."""

        address = 0

        if len(sys.argv) != 2:
            print(f"usage: {sys.argv[0]} filename")
            sys.exit(1)

        try:
            with open(sys.argv[1]) as f:
                for line in f:
                    num = line.split('#', 1)[0]
                    if num.strip() == '':
                        continue
                    self.ram[address] = int(num, 2)
                    address += 1

        except FileNotFoundError:
            print(f"{sys.argv[0]}: {sys.argv[1]} not found")
            sys.exit(2)
                    



    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            self.register[reg_a] += self.register[reg_b]
        
        elif op == "CMP":
            value_1 = self.register[reg_a]
            value_2 = self.register[reg_b]
            if value_1 > value_2:  
                self.flag = 0b00000010
            elif value_1 == value_2:  
                self.flag = 0b00000001
            elif value_1 < value_2:  
                self.flag = 0b00000100
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

    def run(self):
        """Run the CPU."""
        running = True

        while running:
            IR = self.ram_read(self.pc)
            operand_a = self.ram_read(self.pc + 1)
            operand_b = self.ram_read(self.pc + 2)
            
            # LDI: Set the value of a register to an integer.

            if IR == LDI:
                self.register[operand_a] = operand_b
                self.pc += 3

            # HLT: Halt the CPU (and exit the emulator).

            elif IR == HLT:
                running = False
                self.pc += 1

            # PRN: Print numeric value stored in the given register.
            # Print to the console the decimal integer value that is stored in the given register.

            elif IR == PRN:
                print( self.register[operand_a] )
                self.pc += 2

            # MUL: This is an instruction handled by the ALU.
            # Multiply the values in two registers together and store the result in registerA.

            elif IR == MUL:
                self.register[operand_a] *= self.register[operand_b]
                self.pc += 3

            # PUSH: Push the value in the given register on the stack.
            # Decrement the SP.
            # Copy the value in the given register to the address pointed to by SP.

            elif IR == PUSH:
                self.sp -= 1
                regnum = operand_a
                val = self.register[regnum]
                self.ram[self.sp] = val
                self.pc += 2

            # POP: Pop the value at the top of the stack into the given register. 
            # Copy the value from the address pointed to by SP to the given register.
            # Increment SP.

            elif IR == POP:
                val = self.ram[self.sp]
                regnum = operand_a
                self.register[regnum] = val
                self.sp += 1
                self.pc += 2

            # CALL: The PC is set to the address stored in the given register. 
            # We jump to that location in RAM and execute the first instruction in the subroutine. 
            # The PC can move forward or backwards from its current location.

            elif IR == CALL:
                self.sp -= 1
                self.ram[self.sp] = self.pc + 2 
                regnum = operand_a
                self.pc = self.register[regnum]

            # RET: Pop the value from the top of the stack and store it in the PC.

            elif IR == RET:
                self.pc = self.ram[self.sp]
                self.sp += 1

            # ADD: Add the value in two registers and store the result in registerA.

            elif IR == ADD:
                self.register[operand_a] += self.register[operand_b]
                self.pc += 3

            elif IR == CMP:
                self.alu("CMP", operand_a, operand_b)
                self.pc += 3

            # JMP: Jump to the address stored in the given register.
            # Set the PC to the address stored in the given register.

            elif IR == JMP:
                address = self.register[operand_a]
                self.pc = address

            # JEQ: If equal flag is set (true), jump to the address stored in the given register.

            elif IR == JEQ:
                if self.flag == 0b00000001:
                    address = self.register[operand_a]
                    self.pc = address
                else:
                    self.pc += 2

            # JNE: If E flag is clear (false, 0), jump to the address stored in the given register.

            elif IR == JNE: 
                if self.flag & 0b00000001 == 0b00000000:
                    address = self.register[operand_a]
                    self.pc = address
                else:
                    self.pc += 2 