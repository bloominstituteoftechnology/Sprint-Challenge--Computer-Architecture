"""CPU functionality."""

import sys

class CPU:
    """Main CPU class."""

    def __init__(self):

        # registers R0 - R7
        # R5 = interrupt mask (IM)
        # R6 = interrupt status (IS)
        # R7 = stack pointer (SP)

        """Construct a new CPU."""
        self.pc = 0 # counter 
        self.registers = [0b0] * 8 
        self.mdr = None # memory data register 
        self.mar = None # memory address register 
        self.ir = None # instruction register 
        # self.fl = None # flags
        self.gt = None # flag greater than
        self.lt = None # flag less than 
        # self.eq = None # flag equal to 
        self.eq = 0b00000001
        self.spl = None

        self.fl = 0b00000000

        # pointer location 
        self.ram = [0b0] * 0xFF #255 decimal
        self.spl = 8 - 1
        # change to self.ram[]
        self.registers[self.spl] = 0xF4 #244 decimal 
     
    #  self.OPCODES = {0b10000010: 'LDI', 0b01000111: 'PRN', 0b00000001: 'HLT'}
        # update opcodes 
        self.OPCODES = {0b10000010: 'LDI',
                        0b01000111: 'PRN',
                        0b00000001: 'HLT',
                        # alu 
                        0b10100010: 'MUL',
                        # stack 
                        0b01000110: 'POP',
                        0b01000101: 'PUSH',
                        0b10000100: 'ST',

                        # conditional jumps 
                        0b01010110: 'JNE',
                        0b10100111: 'CMP',
                        0b01010100: 'JMP',
                        0b01010101: 'JEQ',
        }

    def load(self,filename:str):
        """Load a program into memory."""

        try:
            with open(filename, 'r') as f:
                # lookout for blanklines and ignore them 
                lines = (line for line in f.readlines() if not (line[0]=='#' or line[0]=='\n'))
                program = [int(line.split('#')[0].strip(),2) for line in lines]

            address = 0

            for instruction in program:
                self.ram[address] = instruction
                address += 1
        
        except FileNotFoundError as e:
            print(e)
            sys.exit()


    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            self.registers[reg_a] += self.registers[reg_b]
        #elif op == "SUB": etc

        elif op == "MUL":
            self.registers[reg_a] *= self.registers[reg_b]

        # compare values in two registers 
        elif op == "CMP":
            a = self.registers[reg_a]
            b = self.registers[reg_b]
            
            if a == b:
                self.fl = self.eq 
            elif a < b:
                self.fl = self.lt
            elif a > b:
                self.fl = self.gt 

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

        # increment dynamically #TODO
      
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

                # do print  
                elif op == 'PRN':
                    reg = self.ram[self.pc+1]
                    val = self.registers[reg]
                    # print(f"hex value: {val:x}\tdec val: {val}")
                    print(val)
                    self.pc += 2

                # to alu

                elif op == 'ADD' or op == 'MUL' or op == 'CMP':
                    reg_a = self.ram[self.pc+1]
                    reg_b = self.ram[self.pc+2]
                    self.alu(op,reg_a,reg_b)
                    self.pc += 3

                # pop
                elif op == 'POP':
                    reg = self.ram[self.pc + 1]
                    val = self.ram[self.registers[self.spl]]
                    # copy value from address pointed by SP 
                    # copy to the stated register 
                    self.registers[reg] = val
                    # increment spl 
                    self.registers[self.spl] += 1
                    self.pc += 2

                # push 

                elif op == 'PUSH':
                    reg = self.ram[self.pc+1]
                    val = self.registers[reg]
                    #decrement pointer 
                    self.registers[self.spl] -= 1
                    # copy register val to the address being pointed to 
                    self.ram[self.registers[self.spl]] = val
                    self.pc += 2

                 # ST
                elif op == 'ST':
                    # Store value in registerB in the address stored in registerA.
                    # write to memory.
                    reg_a = self.ram[self.pc + 1]
                    reg_b = self.ram[self.pc + 2]
                    address_a = self.registers[reg_a]
                    val_b = self.registers[reg_b]
                    self.ram[addres_a] = val_b
                    self.pc += 2

                # JMP
                elif op == 'JMP':
                    # jump to address given in the register 
                    # set PC to that address - JUMP JUMP!
                    reg = self.ram[self.pc +1 ]
                    val = self.registers[reg]
                    self.pc = val

                # JEQ 
                elif op == 'JEQ':
                    # if equal flag = 1, jump to address in given register
                    if self.fl == self.eq:
                        reg = self.ram[self.pc +1]
                        val = self.registers[reg]
                        self.pc = val 
                    # increment away if otherwise 
                    else:
                        self.pc += 2

                # JNE 
                elif op == 'JNE':
                    # if equal flag = 0, jump to address in given register
                    if self.fl != self.eq:
                        reg = self.ram[self.pc + 1]
                        val = self.registers[reg]
                        self.pc = val
                    # increment away if otherwise 
                    else:
                        self.pc += 2
                # EXIT
                elif op == 'HLT':
                    running = False 

            except KeyError as e:
                print(f"unknown command {self.ir}")
                self.pc += 1
        pass 
    

    def ram_read(self,location):
        """
        read from ram
        args: ram location to read from 
        returns: return the stored value
        """
        return self.ram[location]


    def ram_write(location,value):
        """
        write to ram 
        args: location on ram and value to write 
        returns: none 
        """
        self.ram[location] = value 
        pass
