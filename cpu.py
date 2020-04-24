"""CPU functionality."""

import sys
import time

# instructions
ADD =  0b10100000  # 0
AND  = 0b10101000  # 8
CALL = 0b01010000  # 0
CMP  = 0b10100111  # 7
HLT = 0b00000001  # 1
JEQ  = 0b01010101  # 5
JMP = 0b01010100  # 4
JNE  = 0b01010110  # 6
LDI = 0b10000010  # 2
MUL = 0b10100010  # 2
POP = 0b01000110  # 6
PRN = 0b01000111  # 7
PUSH = 0b01000101  # 5
RET = 0b00010001  # 1
ST = 0b10000100  # 4

# Other constants
IM = 5
IS = 6
SP = 7  # stack pointer stored in register at index 7


HARDCODED_PROGRAM = [
            0b10000010, # LDI R0,8
            0b00000000,
            0b00001000,
            0b01000111, # PRN R0
            0b00000000,
            0b00000001, # HLT
        ]

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
        self.mar = None # MAR: Memory Address Register, holds the memory address we are reading/writing
        self.mdr = None # MDR: Memory Data Register, holds the value to write or the value just read
        self.spl = None # stack pointer location
        self.interrupts_enabled = True

        self.equal = 0

        self.ram = [0b0] * 0xFF
        self.spl = 8 - 1
        self.registers[self.spl] = 0xF4

        self.branchtable = {
            ADD:  self.add,
            AND:  self.and_ls8,
            CALL: self.call,
            CMP:  self.cmp_ls8,
            HLT:  self.hlt,
            JMP:  self.jmp,
            JEQ:  self.jeq,
            JNE:  self.jne,
            LDI:  self.ldi,
            MUL:  self.mul,
            POP:  self.pop,
            PRN:  self.prn,
            PUSH: self.push,
            RET:  self.ret,
            ST:   self.st,
        }

    # Emulator functions
    def load(self, program=HARDCODED_PROGRAM):
        """Load program into memory"""

        address = 0
        if program != HARDCODED_PROGRAM:
            try:
                with open(program, 'r') as f:
                    for line in f:
                        instruction = line.split('#')[0].strip()
                        if instruction == '':
                            continue
                        self.ram[address] = int(instruction, base=2)
                        address += 1

            except FileNotFoundError:
                print(f'ERROR: File not found {program}')
                sys.exit(2)
        else:
            for instruction in program:
                self.ram[address] = instruction
                address += 1
    
    def ram_read(self, MAR):
        return self.ram[MAR]

    def ram_write(self, MDR, MAR):
        self.ram[MAR] = self.ram[MDR]

    def invoke_instruction(self):
        if self.ir in self.branchtable:
            self.branchtable[self.ir]()
        else:
            print(f"ERROR: invalid instructions: {bin(self.ir)}")
            sys.exit(1)

    def move_pc(self):
        instruction_sets_pc = ((self.ir << 3) & 255) >> 7
        if not instruction_sets_pc:
            self.pc += (self.num_operands + 1)
    
    def check_interrupts(self):
        masked_interrupts = self.registers[IM] & self.registers[IS]
        for i in range(8):
            interrupt_happened = ((masked_interrupts >> i) & 1) == 1
            if interrupt_happened:
                self.interrupts_enabled = False
                self.registers[IS] = self.registers[IS] & (255 - 2**i)
                self.registers[SP] -= 1
                self.ram[self.reg[SP]] = self.registers[self.pc]
                self.registers[SP] -= 1
                self.ram[self.reg[SP]] = self.registers[self.equal]
                for j in range(7):
                    self.registers[SP] -= 1
                    self.ram[self.registers[SP]] = self.registers[j]
                self.pc = self.ram[0xF8 + i]

    def set_operands(self):
        self.num_operands = self.ir >> 6
        if self.num_operands == 1:
            self.operand_a = self.ram_read(self.pc + 1)
        elif self.num_operands == 2:
            self.operand_a = self.ram_read(self.pc + 1)
            self.operand_b = self.ram_read(self.pc + 2)

    def alu(self, op, reg_a, reg_b=None):
        """ALU operations."""
        if op == "ADD":
            self.registers[reg_a] += self.registers[reg_b]
        elif op == "SUB":
            self.registers[reg_a] -= self.registers[reg_b]
        elif op == "MUL":
            self.registers[reg_a] *= self.registers[reg_b]
        elif op == "CMP":
            if self.registers[reg_a] == self.registers[reg_b]:
                self.equal = self.equal | 0b00000001
            else:
                self.equal = self.equal & 0b11111110

            if self.registers[reg_a] > self.registers[reg_b]:
                self.equal = self.equal | 0b00000010
            else:
                self.equal = self.equal & 0b11111101

            if self.registers[reg_a] < self.registers[reg_b]:
                self.equal = self.equal | 0b00000100
            else:
                self.equal = self.equal & 0b11111011
        elif op == "AND":
            self.registers[reg_a] = self.registers[reg_a] & self.registers[reg_b]
        else:
            raise Exception("ERROR: Invalid ALU operation.")

        self.registers[reg_a] = self.registers[reg_a] & 0xFF

    # Ops
    def ldi(self):
        self.registers[self.operand_a] = self.operand_b

    def prn(self):
        print(self.registers[self.operand_a])

    def hlt(self):
        sys.exit(0)

    def add(self):
        self.alu('ADD', self.operand_a, self.operand_b)

    def mul(self):
        self.alu('MUL', self.operand_a, self.operand_b)

    def st(self):
        self.ram_write(self.reg[self.operand_b], self.reg[self.operand_a])

    def push(self):
        self.registers[SP] -= 1
        self.ram[self.registers[SP]] = self.registers[self.operand_a]
    
    def pop(self):
        if self.registers[SP] > 0xF3:
            print('Error: the stack is empty')
            sys.exit(3)
        else:
            self.registers[self.operand_a] = self.ram[self.registers[SP]]
            self.registers[SP] += 1

    def call(self):
        self.registers[SP] -= 1
        self.ram[self.registers[SP]] = self.pc + 2
        self.jmp()
    
    def ret(self):
        self.pc = self.ram[self.registers[SP]]

    def cmp_ls8(self):
        self.alu('CMP', self.operand_a, self.operand_b)

    def jmp(self):
        self.pc = self.registers[self.operand_a]

    def jeq(self):
        if self.equal & 0b00000001 == 1:
            self.jmp()
        else:
            self.pc += 2
    
    def jne(self):
        if self.equal & 0b00000001 == 0:
            self.jmp()
        else:
            self.pc += 2
    
    def and_ls8(self):
        self.alu('AND', self.operand_a, self.operand_b)

    # Emulator run
    def run(self):
        """Run the CPU"""
        running = True
        interrupt_time = time.time() + 60

        timer = True

        while True:
            if timer:
                if time.time() > interrupt_time:
                    self.reg[6] = self.reg[6] | 0b00000001
                    interrupt_time = time.time() + 60
            if self.interrupts_enabled:
                self.check_interrupts()
            self.ir = self.ram_read(self.pc)
            self.set_operands()
            self.invoke_instruction()
            self.move_pc()
