"""CPU functionality."""

import sys

# HALT = 1
# PRINT_TOM = 2
# PRINT_NUM = 71
# SAVE = 130
# PRINT_REG = 5
# ADD = 6


# HALT = 1
# PRINT_TOM = 2
# PRINT_NUM = 3
# SAVE = 4
# PRINT_REG = 5
# ADD = 6
# MUL = 71
class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        # Add list properties to the CPU class to hold 256 bytes of memory and 8 general-purpose registers.
        self.reg = [0] * 8
        self.pc = 0
        self.ram = [0] * 128
        self.ir = None

    def ram_read(self, index):
        # address
        return self.ram[index]
    def ram_write(self, index, value):
        self.ram[index] = value
        # Memory Address Register (MAR) and the Memory Data Register (MDR).

    def load(self, filename):
        """Load a program into memory."""

        # address = 0

        # # For now, we've just hardcoded a program:

        # program = [
        #     # From print8.ls8
        #     0b10000010, # LDI R0,8
        #     0b00000000,
        #     0b00001000,
        #     0b01000111, # PRN R0
        #     0b00000000,
        #     0b00000001, # HLT
        # ]

        # for instruction in program:
        #     # no need for conversion its done automatically 
        #     # self.ram[address] = instruction
        #     self.ram_write(address, instruction)
        #     address += 1
        try:
            address = 0
            with open(filename) as f:
                for line in f:
                    comment_split = line.split("#")
                    num = comment_split[0].strip()
                    if len(num) == 0:
                        continue
                    value = int(num, 2)
                    self.ram[address] = value
                    address += 1

        except FileNotFoundError:
            print(f"{sys.argv[0]}: {sys.argv[1]} not found")
            sys.exit(2)


    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        elif op == "SUB":
            self.reg[reg_a] -= self.reg[reg_b]
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
        # as the ram digits been converted to decimal
        LDI = 0b10000010
        MUL = 0b10100010 
        PRN = 0b01000111
        HLT = 0b00000001 
        PUSH = 0b01000101
        POP = 0b01000110
        SP = 7
        CALL = 0b01010000
        RET = 0b00010001
        ADD = 0b10100000
        running = True
        # looks like we need a converter for the binary
        while running:
            command = self.ram_read(self.pc)
            # if command == LDI:
            #     self.ir = command
            #     instruction_size = 1
            if command == LDI:
                self.ir = command
                instruction_size = 3
                reg = self.ram[self.pc + 1]
                value = self.ram[self.pc + 2]
                self.reg[reg] = value
            # elif command == PRN:
            #     self.ir = command
            #     instruction_size = 2
            #     num = self.ram[self.pc + 1]
            #     print(num)
            elif command == ADD:
                self.ir = command
                instruction_size = 3
                reg_a = self.ram[self.pc + 1]
                reg_b = self.ram[self.pc + 2]
                self.reg[reg_a] += self.reg[reg_b]
            elif command == MUL:
                self.ir = command
                instruction_size = 3
                reg_a = self.ram[self.pc + 1]
                reg_b = self.ram[self.pc + 2]
                self.reg[reg_a] *= self.reg[reg_b]
            #     # don't understand
            elif command == PRN:
                self.ir = command
                instruction_size = 2
                reg = self.ram[self.pc + 1]
                print(self.reg[reg])
            
            elif command == PUSH:
                instruction_size = 2
                reg = self.ram[self.pc + 1]
                val = self.reg[reg]
                # PUSH
                self.reg[SP] -= 1
                self.ram[self.reg[SP]] = val

            # DECODE
            elif command == POP:
                instruction_size = 2
                reg = self.ram[self.pc + 1]
                val = self.ram[self.reg[SP]]
                # POP
                self.reg[reg] = val
                self.reg[SP] += 1
            elif command == HLT:
                self.ir = command
                instruction_size = 1
                running = False
            elif command == CALL:
                instruction_size = 2
                reg = self.ram[self.pc + 1]
                # CALL
                self.reg[SP] -= 1 # Decrement Stack Pointer
                self.ram[self.reg[SP]] = self.pc + 2 # Push PC + 2 on to the stack
                # set pc to subroutine
                self.pc = self.reg[reg]
                instruction_size = 0

            elif command == RET:
                self.pc = self.ram[self.reg[SP]]
                self.reg[SP] += 1
                instruction_size = 0
            else:
                print(f"Unknown Instruction {command}")
                sys.exit(1)
            # its not inside else but on the same level
            self.pc += instruction_size
