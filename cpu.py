"""CPU functionality."""

import sys


class CPU:
    """Main CPU class."""

    def __init__(self):
        self.ram = [0] * 256
        self.reg = [0] * 8
        self.pc = 0
        # self.SP = 7
        # self.reg[7] = 255
        self.fl = None
		self.SP = 0xF4
		self.reg[7] = self.ram[self.SP]

		# Instruction table
        self.in_table = {}
        self.in_table[0b00000001] = self.hlt
        self.in_table[0b10000010] = self.ldi
        self.in_table[0b01000111] = self.prn
        self.in_table[0b01000101] = self.push
        self.in_table[0b01000110] = self.pop
        self.in_table[0b01010000] = self.call
        self.in_table[0b00010001] = self.ret
        self.in_table[0b01010100] = self.jmp
        self.in_table[0b01010101] = self.jeq
        self.in_table[0b01010110] = self.jne

    def ram_read(self, MAR):
        return self.ram[MAR]

    def ram_write(self, MAR, MDR):
        self.ram[MAR] = MDR

	def hlt(self):
        sys.exit()

    def ldi(self, operand_a, operand_b):
		self.reg[operand_a] = operand_b
        self.pc += 3

    def prn(self, operand_a):
		print(self.reg[operand_a])
		self.pc += 2

    def push(self, operand_a):
		# 1. Decrement the 'SP'
		# 2. copy the value in the given register to the address pointed to by 'SP'
		self.SP -= 1
        value = self.ram[operand_a]
        
        
        self.ram[self.SP] = value
        self.pc += 2

    def pop(self, operand_a):
		# 1. copy the value from the address pointed to by 'SP' to the given register
		# 2. Increment 'SP'
		
        value = self.ram[self.SP]
		self.reg[operand_a] = value
		self.SP += 1
        self.pc += 2
        
        
    def call(self, operand_a):
		# 1. The address ***instructions*** _directly after_ 'CALL' is pushed onto the stack
		# This allow us to return to where we left off when subroutine finishes executing.
		# 2. The PC is set to the address stored in the given register. We jump to first location in RAM
		# and execute the first instruction in the subroutine. The PC  can move forward/backward from its current location
		self.SP -= 1
		next_instruction_address = self.pc + 2
		self.ram[self.SP] = next_instruction_address
		address_to_jump_to = self.ram[operand_a]
		# set PC to the address to jump to
		self.SP = address_to_jump_to

    

    def jmp(self, operand_a):
		self.pc = self.reg[operand_a]

    def jeq(self):

    def jne(self):


    def load(self):
        """Load a program into memory."""

        address = 0

        
        try:
            # open the program specified by the second command line argument
            with open(sys.argv[1]) as f:
                # for each line in the file
                for line in f:
                    # check if it starts with a binary number
                    if line[0].startswith('0') or line[0].startswith('1'):
                        # only use the first (non-commented) part of the instruction
                        binary = line.split("#")[0]
                        # remove any white space
                        binary = binary.strip()
                        # convert to binary and store it in RAM
                        self.ram[address] = int(binary, 2)
                        address += 1
        except:
            print(f"{sys.argv[0]}: {sys.argv[1]} not found.")

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        # elif op == "SUB": etc
        elif op == "MUL":
            self.reg[reg_a] *= self.reg[reg_b]
        else:
            raise Exception("Unsupported ALU operation")

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
        HLT = 0b00000001
        LDI = 0b10000010
        PRN = 0b01000111
        MUL = 0b10100010
        PUSH = 0b01000101
        POP = 0b01000110
        CALL = 0b01010000
        RET = 0b00010001
        JMP = 0b01010100

        running = True

        while running:
            # It needs to read the memory address that's stored in register `PC`, and store
            # that result in `IR`, the _Instruction Register_. This can just be a local
            # variable in `run()`.
            IR = self.ram[self.pc]

            # Using `ram_read()`,read the bytes at `PC+1` and `PC+2` from RAM into variables
            # `operand_a` and `operand_b` in case the instruction needs them.
            operand_a = self.ram_read(self.pc + 1)
            operand_b = self.ram_read(self.pc + 2)

            # Then, depending on the value of the opcode, perform the actions needed for the
            # instruction per the LS-8 spec. Maybe an `if-elif` cascade...? There are other
            # options, too.

            # Implement a system stack per the spec. Add `PUSH` and `POP` instructions. Read
            # the beginning of the spec to see which register is the stack pointer.
            if IR == HLT:
                running = False
            elif IR == LDI:
                self.reg[operand_a] = operand_b
                self.pc += 3
            elif IR == PRN:
                print(self.reg[operand_a])
                self.pc += 2
            elif IR == MUL:
                self.alu("MUL", operand_a, operand_b)
                self.pc += 3
            
            elif IR == RET:
                self.SP = self.reg[7]
                address_to_return_to = self.ram[self.SP]
                self.reg[7] = (self.SP + 1) % 255
                self.pc = address_to_return_to
            elif IR == JMP:
                register_address = self.ram[self.pc + 1]
                address_to_jump_to = self.reg[register_address]
                self.pc = address_to_jump_to
            else:
                print("Unknown instruction.")
        


