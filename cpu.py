  
"""CPU functionality."""

import sys

HLT = 0b00000001
LDI = 0b10000010 
PRN = 0b01000111
MUL = 0b10100010
ADD = 0b10100000
PUSH = 0b01000101
POP = 0b01000110
CALL = 0b01010000 
RET = 0b00010001
CMP = 0b10100111
JMP = 0b01010100
JEQ = 0b01010101
JNE = 0b01010110

class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.reg = [0] * 8
        self.ram = [0] * 256
        self.reg[7] = 0xF4
        self.pc = 0
        self.halted = False
        self.FL = 0b00000000
    
    #* Memory address
    def ram_read(self, address):
        return self.ram[address]
    #* Memory data
    def ram_write(self, address, value):
        self.ram[address] = value

    def load(self, filename):
        """Load a program into memory."""
        
        address = 0
        with open(filename) as fp:
            for line in fp:
                line_split = line.split("#")
                num = line_split[0].strip()
                if num == '':
                    continue
                value = int(num, 2)
                self.ram[address] = value
                address += 1


    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        elif op == "MUL":
            self.reg[reg_a] *= self.reg[reg_b]
        elif op == "CMP":
            if self.reg[reg_a] < self.reg[reg_b]:
                self.FL = 0b00000100
            if self.reg[reg_a] > self.reg[reg_b]:
                self.FL = 0b00000010
            if self.reg[reg_a] == self.reg[reg_b]:
                self.FL = 0b00000001
            
        # elif op == "SUB": etc
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
        
        while not self.halted:

            instruction_to_execute = self.ram_read(self.pc)
            operand_a = self.ram_read(self.pc +1)
            operand_b = self.ram_read(self.pc +2)
            self.execute_instruction(instruction_to_execute, operand_a, operand_b)
    

    def execute_instruction(self, IR, operand_a, operand_b):
        if IR == HLT:
            self.halted = True
            self.pc += 1
        elif IR == LDI:
            self.reg[operand_a] = operand_b
            self.pc +=3
        elif IR == PRN:
            print(self.reg[operand_a])
            self.pc += 2
        elif IR == ADD:
            self.alu("ADD", operand_a, operand_b)
            self.pc += 3
        elif IR == MUL:
            self.alu("MUL", operand_a, operand_b)
            self.pc += 3
        
        elif IR == PUSH:

            self.reg[7] -=1
            SP = self.reg[7] 
            value = self.reg[operand_a] 
            self.ram[SP] = value
            self.pc +=2
        
        elif IR == POP:
            SP = self.reg[7]
            value = self.ram[SP]
            self.reg[operand_a] = value
            self.reg[7] +=1
            self.pc +=2
        
        elif IR == CALL:
            self.reg[7] -=1
            self.ram[self.reg[7]] = self.pc + 2 
            addressOfNextInstruction = self.ram_read(self.pc +1)
            self.pc = self.reg[addressOfNextInstruction]
        
        elif IR == RET:
            SP = self.reg[7]
            addressToReturnTo = self.ram[SP]
            self.reg[7] +=1 
            self.pc = addressToReturnTo
        
        elif IR == CMP:
            self.alu("CMP", operand_a, operand_b)
            self.pc += 3
        
        elif IR == JMP:
            register_number = self.ram_read(self.pc + 1)
            self.pc = self.reg[register_number]
        
        elif IR == JEQ:
            if self.FL == 0b00000001:
                register_number = self.ram_read(self.pc +1)
                self.pc = self.reg[register_number]
            else:
                self.pc += 2
        
        elif IR == JNE:
            if self.FL != 0b00000001:
                register_number = self.ram_read(self.pc +1)
                self.pc = self.reg[register_number]
            else:
                self.pc += 2
        else:
            print("I don't know what to do")
            sys.exit(1)