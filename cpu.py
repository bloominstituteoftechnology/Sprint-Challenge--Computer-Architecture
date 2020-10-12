"""CPU functionality."""

import sys
SP = 7
#flags

HLT = 0b00000001
LDI = 0b10000010
PRN = 0b01000111
ADD = 0b10100000
MUL = 0b10100010
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
        self.reg[SP] = 0xF4
        self.ram = [0] * 256
        self.pc = 0
        self.halted = False
        self.FL = 0b00000000

    def ram_read(self, address):
        #print(address)
        return self.ram[address]

    def ram_write():
        self.ram[address] = val
    
    def load(self):
        """Load a program into memory."""
        if len(sys.argv) != 2:
            print("Not enough arguments")
            sys.exit(1)
            

        fname = sys.argv[1]      
        address = 0
        # For now, we've just hardcoded a program:
        try:
            with open(fname) as f:
                for l in f: #for every line in file f
                    instruction = ""
                    for c in l: #for every character in the line
                        if c == '0' or c == '1':
                            instruction += c
                        else:
                            break

                    if len(instruction) > 0:
                        #print(instruction, int(instruction, 2))
                        self.ram[address] = int(instruction, 2)
                        address += 1
        except:
            print("File not found")
            sys.exit(1)

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        #elif op == "SUB": etc
        elif op == "MUL":
            self.reg[reg_a] *= self.reg[reg_b]
        elif op == "CMP":
            self.FL = 0b00000000
            if self.reg[reg_a] == self.reg[reg_b]:
                self.FL = 0b00000001
            elif self.reg[reg_a] > self.reg[reg_b]:
                self.FL = 0b00000010
            elif self.reg[reg_a] < self.reg[reg_b]:
                self.FL = 0b00000100
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
            operand_a = self.ram_read(self.pc + 1)
            operand_b = self.ram_read(self.pc + 2)
            self.execute_instruction(instruction_to_execute, operand_a, operand_b)

    def execute_instruction(self, instruction, operand_a, operand_b):
        #print(f'{bin(instruction)} {bin(operand_a)} {bin(operand_b)}')
        if instruction == HLT:
            self.halted = True
            #self.pc += 1
        elif instruction == LDI:
            self.reg[operand_a] = operand_b
            #self.pc += 3
        elif instruction == PRN:
            print(self.reg[operand_a])
            #self.pc += 2
        elif instruction == ADD:
            self.alu("ADD", operand_a, operand_b)
        elif instruction == MUL:
            self.alu("MUL", operand_a, operand_b)
            #self.pc += 3
        elif instruction == PUSH:
            self.reg[SP] -= 1
            value = self.reg[operand_a]
            self.ram[self.reg[SP]] = value
        elif instruction == POP:
            topvalue = self.ram[self.reg[SP]]
            self.reg[operand_a] = topvalue
            self.reg[SP] += 1
        elif instruction == CALL:
            self.reg[SP] -= 1
            self.ram[self.reg[SP]] = self.pc + 2
            self.pc = self.reg[operand_a]
            return
        elif instruction == RET:
            self.pc = self.ram[self.reg[SP]]
            self.reg[SP] += 1
            return
        elif instruction == CMP:
            self.alu("CMP", operand_a, operand_b)
        elif instruction == JMP:
            self.pc = self.reg[operand_a]
            return
        elif instruction == JEQ:
            #print(bin(self.FL), bin(0b1))
            if self.FL == 0b1:
                #print(bin(self.FL), bin(self.FL << 7))
                self.pc = self.reg[operand_a]
                return
        elif instruction == JNE:
            #print(bin(self.FL), bin(self.FL & 0b001))
            if not (self.FL & 0b001):
                self.pc = self.reg[operand_a]
                return


        instruction = instruction >> 6
        #print(bin(instruction), instruction)
        #if instruction != CALL and instruction != RET:
        self.pc += 1 + instruction
