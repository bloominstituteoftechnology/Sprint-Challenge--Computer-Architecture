"""CPU functionality."""

import sys


LDI = 0b10000010
PRN = 0b01000111
HLT = 0b00000001
MUL = 0b10100010
SP = 7
PUSH = 0b01000101
POP = 0b01000110


CMP = 0b10100111
JMP = 0b01010100
JNE = 0b01010110
JEQ = 0b01010101


class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        # Add list properties to the CPU class to hold 256 bytes of memory
        # and 8 general-purpose registers.
        self.reg = [0]*8  # R0-R7 fixed number fo registers.
        self.ram = [0]*256
        self.pc = 0  # Program Counter, address of the currently executing instruction
        self.flag = 0B00000000

    def load(self):
        """Load a program into memory."""
        try:
            address = 0  # holds the memory address we're reading or writing
            with open(sys.argv[1]) as file:
                for line in file:
                    t = line.split('#')
                    n = t[0].strip()
                    if n == "":
                        continue

                    try:
                        n = int(n, 2)
                    except ValueError:
                        print(f"Invalid number '{n}'")
                        sys.exit(1)

                    self.ram[address] = n
                    address += 1

        except FileNotFoundError:
            print(f"File not found: {sys.argv[1]}")
            sys.exit()

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        # elif op == "SUB": etc
        elif op == "CMP":
            # Compare the values in two registers.
            # If they are equal, set the Equal E flag to 1, otherwise set it to 0.
            if self.reg[reg_a] == self.reg[reg_b]:
                self.flag = 0b00000001

             # If registerA is less than registerB, set the Less-than L flag to 1, otherwise set it to 0.
            if self.reg[reg_a] < self.reg[reg_b]:
                self.flag = 0b00000100

             # If registerA is greater than registerB, set the Greater-than G flag to 1, otherwise set it to 0.
            if self.reg[reg_a] > self.reg[reg_b]:
                self.flag = 0b00000010

        else:
            raise Exception("Unsupported ALU operation")

    # ram_read() should accept the address to read and return the value stored there.
    def ram_read(self, address):
        return self.ram[address]
        # Basically access the whole ram(i like to think of it as a list or an array) and find the one
        # and find the one with the address

    # ram_write() should accept a value to write, and the address to write it to.
    def ram_write(self, address, value):
        self.ram[address] = value
        # Basically access the whole ram(i like to think of it as a list or an array) and find the one
        # and rewrites the address with the new value.
        # idk if i understand when these will be helpful

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
        self.running = True
        while self.running:
            # Instruction register copy of the currently-executing instruction
            ir = self.ram_read(self.pc)
            # Some instructions requires up to the next two bytes of data after the PC in memory
            # to perform operations on. Sometimes the byte value is a register number,
            # other times it's a constant value (in the case of LDI). Using ram_read(),
            # read the bytes at PC+1 and PC+2 from RAM into variables operand_a and operand_b
            # in case the instruction needs them.
            operand_a = self.ram_read(self.pc + 1)
            operand_b = self.ram_read(self.pc + 2)

            if ir == HLT:
                # exit the loop if a HLT instruction is encountered,
                self.running = False
                # it also needs to update the counter but how many bits it take.
                self.pc += 1
                # ie - this only take one line of code so increment by 1 -- idk if this logic works for all of it
            elif ir == LDI:
                # This instruction sets a specified register to a specified value.
                # Machine code: // what register i am going to edit. operand_a
                # ```
                # 10000010 00000rrr iiiiiiii // what value i am going to put there. operand_b (value)
                # 82 0r ii
                self.reg[operand_a] = operand_b
                self.pc += 3

            elif ir == PRN:
                # Print numeric value stored in the given register.
                # Print to the console the decimal integer value that is stored in the given register.
                # Machine code: register and operand_a
                # 01000111 00000rrr
                # 47 0r
                print(self.reg[operand_a])
                self.pc += 2

            elif ir == MUL:
                # MUL registerA registerB
                # Multiply the values in two registers together and store the result in registerA.
                # Machine code:
                # 10100010 00000aaa 00000bbb
                # A2 0a 0b
                # use operand because we need to compute 2 variabls
                # mulitiplyt the two numbers together and save it to register a
                self.reg[operand_a] = self.reg[operand_a] * \
                    self.reg[operand_b]
                # upadate the program counter
                self.pc += 3

             #  R7 is reserved as the stack pointer (SP)
            elif ir == PUSH:
                # Decrement SP
                self.reg[SP] -= 1
                # Get the reg num to push
                register_num = self.ram_read(self.pc + 1)
                # Get the value to push
                value = self.reg[register_num]
                # Copy the value to the SP address
                top_of_stack_addr = self.reg[SP]
                self.ram[top_of_stack_addr] = value
                # increment program counter to put program back on track
                self.pc += 2

            elif ir == POP:
                reg_num = self.ram_read(self.pc + 1)

                # Get the top of stack addr
                top_of_stack_addr = self.reg[SP]

                # Get the value at the top of the stack
                value = self.ram_read(top_of_stack_addr)

                # Store the value in the register
                self.reg[reg_num] = value

                # Increment the SP
                self.reg[SP] += 1
                # increment program counter to put program back on track
                self.pc += 2

                # CMP
            elif ir == CMP:
                # CMP registerA registerB
                # use the "CMP " from the AlU and just keep track of the pc
                self.alu("CMP", operand_a, operand_b)
                self.pc += 3

                # JMP register
            elif ir == JMP:

                # Jump to the address stored in the given register.
                reg_num = self.ram_read(self.pc + 1)

                # Set the PC to the address stored in the given register.
                self.pc = self.reg[reg_num]

                # JNE
            elif ir == JNE:
                # If Equal flag is clear (false, 0), jump to the address stored in the given register.
                if self.flag != 0b00000001:
                    reg_num = self.ram_read(self.pc + 1)
                    self.pc = self.reg[reg_num]
                    # update program counter
                else:
                    self.pc += 2

                # JEQ register
            elif ir == JEQ:
                # If equal flag is set (true), jump to the address stored in the given register.
                if self.flag == 0b00000001:
                    reg_num = self.ram_read(self.pc + 1)
                    self.pc = self.reg[reg_num]
                # update program counter
                else:
                    self.pc += 2


# FLAG DETAILS

# The flags register FL holds the current flags status. These flags can change based on the operands given to the CMP opcode.

# The register is made up of 8 bits. If a particular bit is set, that flag is "true".

# FL bits: 00000LGE

# L Less-than: during a CMP, set to 1 if registerA is less than registerB, zero otherwise.
# G Greater-than: during a CMP, set to 1 if registerA is greater than registerB, zero otherwise.
# E Equal: during a CMP, set to 1 if registerA is equal to registerB, zero otherwise.
