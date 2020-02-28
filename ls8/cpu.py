"""CPU functionality."""

import sys


PRN = 0b01000111
LDI = 0b10000010
HLT = 0b00000001
MUL = 0b10100010
PUSH = 0b01000101
POP = 0b01000110
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
                # create 8 registers 1 - 8, need IM, IS, SP
        self.reg = [0b0] * 8

        # internal registers
        self.pc = 0  # Program counter = address of current_instructions
        self.ir = 0  # instruction register = contains copy of current_instructions
        self.mar = 0  # memory address register = holds memory address read/write
        self.mdr = 0  # memory data register = holds the value to write or value to read
        self.fl = 0  # flags

        self.ram = [0] * 256

        self.sp = 7
        self.program_file = ''
        self.running = True
        self.op_pc = False
        self.equal = 0

        # branch table for O(1) during run()

    def ram_read(self, addr):
        return self.ram[addr]

    def ram_write(self, addr, value):
        self.ram[addr] = value

    def load(self):
        """Load a program into memory."""
        try:
            address = 0
            with open(self.program_file) as f:
                for line in f:
                    line = line.split("#")[0]  # this handles comments
                    line = line.strip()

                    # if line is blank, keep going
                    if line == "":
                        continue
                    
                    # base 2
                    val = int(line, 2)
                    self.ram[address] = val  # add back to memory at address
                    address += 1  # increment'

        except FileNotFoundError:
            print(f"{sys.argv[0]}: {sys.argv[1]} not found")
            sys.exit(2)

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""
        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        elif op == "MUL":
            self.reg[reg_a] *= self.reg[reg_b]
        
        # Move cmp here
        
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

    def handle_ldi(self, op_a, op_b):
        self.reg[op_a] = op_b
        self.op_pc = False

        if not self.op_pc:
            self.pc += 3

    def handle_prn(self, op_a, op_b):
        print(self.reg[op_a])

        self.op_pc = False
        if not self.op_pc:
            self.pc += 2

    def handle_mul(self, op_a, op_b):
        self.alu("MUL", op_a, op_b)
        self.op_pc = False

        if not self.op_pc:
            self.pc += 3

    def handle_add(self, op_a, op_b):
        self.alu("ADD", op_a, op_b)
        self.op_pc = False

        if not self.op_pc:
            self.pc += 3

    def handle_push(self, op_a, op_b):
        reg = self.ram_read(self.pc + 1)
        val = self.reg[reg]
        self.reg[self.sp] -= 1
        self.ram_write(self.reg[self.sp], val)

        self.op_pc = False
        if not self.op_pc:
            self.pc += 2

    def handle_pop(self, op_a, op_b):
        reg = self.ram_read(self.pc + 1)
        val = self.ram_read(self.reg[self.sp])
        self.reg[reg] = val
        self.reg[self.sp] += 1

        self.op_pc = False
        if not self.op_pc:
            self.pc += 2

    def handle_halt(self, op_a, op_b):
        self.running = False

    def handle_call(self, op_a, op_b):
        reg = self.ram_read(self.pc + 1)
        self.reg[self.sp] -= 1
        self.ram_write(self.reg[self.sp], self.pc + 2)
        self.pc = self.reg[reg]
        self.op_pc = True

        if not self.op_pc:
            self.pc += 2

    def handle_ret(self, op_a, op_b):
        self.pc = self.ram_read(self.reg[self.sp])
        self.reg[self.sp] += 1
        self.op_pc = True

        if not self.op_pc:
            self.pc += 1

    def handle_cmp(self, op_a, op_b): # should move to alu
        if self.reg[op_a] == self.reg[op_b]:
            self.equal = 1
        else:
            self.equal = 0

        self.op_pc = False

        if not self.op_pc:
            self.pc += 3

    def handle_jmp(self, op_a, op_b):
        self.pc = self.reg[op_a]

        self.op_pc = True

        if not self.op_pc:
            self.pc += 2

    def handle_jeq(self, op_a, op_b):
        if self.equal == 1:
            self.pc = self.reg[op_a]
            self.op_pc = True

        if not self.op_pc:
            self.pc += 2

    def handle_jne(self, op_a, op_b):
        if self.equal == 0:
            self.pc = self.reg[op_a]
            self.op_pc = True

        if not self.op_pc:
            self.pc += 2

    def run(self):
        """Run the CPU."""
        IR = self.ram[self.pc]

        if len(sys.argv) != 2:
            print("usage: cpy.py filename")
            sys.exit(1)

        # Get program from file
        self.program_file = sys.argv[1]
        # Load program into memory
        self.load()

        while self.running:
            # Use program counter to get current instruction
            IR = self.ram[self.pc]
            # Get operands for the instruction
            operand_a = self.ram_read(self.pc + 1)
            operand_b = self.ram_read(self.pc + 2)

            # Invoke the method responsible for handling this instruction
            if IR in self.branch_table:
                self.branch_table[IR](operand_a, operand_b)
            else:
                # print(IR)
                print(f"Error: Instruction {IR} not found")
                sys.exit(1)

# Place methods on branch_table key=>pay dictionary to enable O(1) access inside run() loop
        self.branch_table = {}
        self.branch_table[PRN] = self.handle_prn
        self.branch_table[LDI] = self.handle_ldi
        self.branch_table[ADD] = self.handle_add
        self.branch_table[HLT] = self.handle_halt
        self.branch_table[MUL] = self.handle_mul
        self.branch_table[PUSH] = self.handle_push
        self.branch_table[POP] = self.handle_pop
        self.branch_table[CALL] = self.handle_call
        self.branch_table[RET] = self.handle_ret
        self.branch_table[CMP] = self.handle_cmp
        self.branch_table[JEQ] = self.handle_jeq
        self.branch_table[JMP] = self.handle_jmp
        self.branch_table[JNE] = self.handle_jne
