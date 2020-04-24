"""CPU functionality."""

import sys

class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.ram      = [0] * 256  # 8 bit memory
        self.reg      = [0] * 8  # 8 general-purpose 8-bit numeric registers R0-R7.
        self.IR       = [0] * 1
        self.PC       = 0  # Program Counter, 
        self.MAR      = [0] * 1 # Addr
        self.MDR      = [0] * 1 # Data
        self.FL       = [0] * 8
        
        # R7 is reserved as the stack pointer (SP)
        self.reg[7]= 0xF4
        self.SP  = self.reg[7]
        
        self.L = self.FL[5]
        self.G = self.FL[6]
        self.E = self.FL[7]
        
        global HLT,PRN,LDI,LD,PUSH,POP,CALL,RET,JMP,JNE,JEQ,ST,PRA,IRET
        global MUL,ADD,CMP,AND,OR,XOR,NOT,SHL,SHR,MOD,DEC,INC
        
        HLT  = 0b00000001
        LDI  = 0b10000010
        LD   = 0b10000011
        PRN  = 0b01000111
        MUL  = 0b10100010
        ADD  = 0b10100000
        CMP  = 0b10100111
        PUSH = 0b01000101
        POP  = 0b01000110
        CALL = 0b01010000
        RET  = 0b00010001
        JMP  = 0b01010100
        JNE  = 0b01010110
        JEQ  = 0b01010101
        IRET = 0b00010011
        
        AND  = 0b10100000
        OR   = 0b10101010
        XOR  = 0b10101011
        NOT  = 0b01101001
        SHL  = 0b10101100
        SHR  = 0b10101101
        MOD  = 0b10100100
        DEC  = 0b01100110
        INC  = 0b01100101
        
        ST   = 0b10000100
        PRA  = 0b01001000
        pass

    def load(self,program_filename):
        """Load a program into memory."""
        # Load program into memory
        address = 0 
        with open(program_filename) as f:
            for line in f:
                line = line.split("#")
                line = line[0].strip()
                if line == '':
                    continue
                self.ram[address] = int(line,2)
                address += 1

    
    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        elif op == "SUB": 
            self.reg[reg_a] -= self.reg[reg_b]
        elif op == "MUL":
            self.reg[reg_a] *= self.reg[reg_b]
        elif op == "CMP":
            self.L, self.G,self.E = 0,0,0
            if (self.reg[reg_a] < self.reg[reg_b]):
                self.L = 1
            elif (self.reg[reg_a] > self.reg[reg_b]):
                self.G = 1
            else:
                self.E = 1
        elif op == "INC":
            self.reg[reg_a] += 1
        elif op == "DEC":
            self.reg[reg_a] -= 1
        elif op == "AND":
            self.reg[reg_a] &= self.reg[reg_b]
        elif op == "OR" :
            self.reg[reg_a] |= self.reg[reg_b]
        elif op == "XOR":
            self.reg[reg_a] ^= self.reg[reg_b]
        elif op == "NOT":
            self.reg[reg_a] = ~self.reg[reg_a]
        elif op == "SHL":
            self.reg[reg_a] <<= self.reg[reg_b]
        elif op == "SHR":
            self.reg[reg_a] >>= self.reg[reg_b]
        elif op == "MOD":
            if(self.reg[reg_b] != 0):
                self.reg[reg_a] %= self.reg[reg_b]
            else:
                print("ZeroDivisionError: integer division or modulo by zero")
                return
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
        
    def get_inst_count(self):
            inst_len = ((self.ram[self.PC] & self.IR) >> 6) + 1   # 3
            return inst_len
        
    def run(self):
        """Run the CPU."""
        
        running = True
        
        while running:
            self.IR = self.ram[self.PC]
#         print(bin(self.IR))
#         self.branchtable[bin(self.IR)]()
            inst_count = self.get_inst_count()
            
            if self.IR == LDI: # Save the register
                self.MAR = self.ram[self.PC + 1]
                self.MDR = self.ram[self.PC + 2]
                self.ram_write()
                self.PC += inst_count
            elif self.IR == LD: # Save the register
                reg_a = self.ram[self.PC + 1]
                reg_b = self.ram[self.PC + 2]
                self.reg[reg_a] = self.reg[reg_b]
                self.PC += inst_count

            elif self.IR == PRN: # Print Register 0
                self.MAR = self.ram[self.PC + 1]
                
                self.ram_read()
                print(self.MDR)
                self.PC += inst_count
                
            elif self.IR == PRA:
                self.MAR = self.ram[self.PC + 1]
                self.ram_read()
                print(chr(self.ram[self.MDR]))
                self.PC += inst_count
                
            elif self.IR == MUL: # Print Register 0
                reg_a = self.ram[self.PC + 1]
                reg_b = self.ram[self.PC + 2]
                op = "MUL"
                self.alu(op, reg_a,reg_b)
                self.PC += inst_count
            
            elif self.IR == ADD: # Print Register 0
                reg_a = self.ram[self.PC + 1]
                reg_b = self.ram[self.PC + 2]
                op = "ADD"
                self.alu(op, reg_a,reg_b)
                self.PC += inst_count
            elif self.IR == INC: # Print Register 0
                reg_a = self.ram[self.PC + 1]
                reg_b = None
                op = "INC"
                self.alu(op, reg_a,reg_b)
                self.PC += inst_count
            elif self.IR == DEC: # Print Register 0
                reg_a = self.ram[self.PC + 1]
                reg_b = None
                op = "DEC"
                self.alu(op, reg_a,reg_b)
                self.PC += inst_count
                
#             elif self.IR == ADDI: # Print Register 0
#                 reg_a = self.ram[self.PC + 1]
#                 const = 1
#                 self.reg[reg_a] += const
#                 self.PC += inst_count
                
            elif self.IR == CMP:
                reg_a = self.ram[self.PC + 1]
                reg_b = self.ram[self.PC + 2]
                op = "CMP"
                self.alu(op, reg_a,reg_b)
                self.PC += inst_count
                
            elif self.IR == MOD:
                reg_a = self.ram[self.PC + 1]
                reg_b = self.ram[self.PC + 2]
                op = "MOD"
                self.alu(op, reg_a,reg_b)
                self.PC += inst_count
                
            elif self.IR == PUSH:
                # decrement the stack pointer
                self.SP -= 1
                
                # copy value from register into ram
                self.MAR = self.ram[self.PC + 1]
                self.MDR = self.reg[self.MAR] # this is what we want to push
                
                self.ram[self.SP] = self.MDR # store the value on the stack
                
                
                self.PC += inst_count
                
            elif self.IR == POP:
                # Copy the value from the address pointed to by SP to the given register.
                self.MDR = self.ram[self.SP] # value pointed by SP
                self.MAR = self.ram[self.PC + 1] # address of the register
                
                self.reg[self.MAR] = self.MDR # copy value to the register
                
                # Increment SP
                self.SP += 1
                
                
                self.PC += inst_count
            
            elif self.IR == ST:
                reg_a = self.ram[self.PC + 1]
                reg_b = self.ram[self.PC + 2]
                self.reg[reg_a] = self.reg[reg_b]
                self.PC += inst_count
            
            
            elif self.IR == CALL:
                # compute return address
                self.MAR = self.PC + 2
                
                # push on the stack
                self.SP -= 1
                self.ram[self.SP] = self.MAR
                
                # Set the PC to the value in the given register
                self.MAR = self.ram[self.PC + 1]
                self.PC = self.reg[self.MAR]
                
            elif self.IR == RET:
                # pop return address from top of stack
                self.MAR = self.ram[self.SP]
                self.SP += 1
                
                # Set the pc
                self.PC = self.MAR
                
            elif self.IR == JMP:
                # Jump to the address stored in the given register.
                self.MAR = self.ram[self.PC + 1]
                self.PC = self.reg[self.MAR]
                
            elif self.IR == JNE:
                if self.E == 0:
                    self.MAR = self.ram[self.PC + 1]
                    self.PC = self.reg[self.MAR]
                else:
                    self.PC += inst_count
            elif self.IR == JEQ:
                if self.E == 1:
                    self.MAR = self.ram[self.PC + 1]
                    self.PC = self.reg[self.MAR]
                else:
                    self.PC += inst_count
            elif self.IR == HLT: # Halt command
                running = False

            else:
                print("Unknown instruction")
                running = False

    def ram_read(self):
        """
        Accept the address to read and return the value stored there.
        """
        self.MDR = self.reg[self.MAR]
        return 
    
    def ram_write(self):
        """ 
        Accept a value to write, and the address to write it to.
        """
        self.reg[self.MAR] = self.MDR
        
        
        
        