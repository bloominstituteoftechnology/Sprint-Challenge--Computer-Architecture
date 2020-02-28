"""CPU functionality."""

import sys

class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.ram = [0] * 256
        self.reg  = [0] * 8 
        self.reg[7] = 244 # R7 is the SP
        self.pc = 0
        self.fl = 0b00000000
        self.halted = False
        self.ir = 0

    def load(self, filename):
        """Load an ls8 program into memory."""

        path = 'examples/' + filename
        address = 0

        with open(path) as f:
            for line in f:
                # skip empty lines and comments
                if line[0].isnumeric()==False:
                    continue
                self.ram[address] = int(line[0:8], 2)
                address += 1 

    def add(self, reg_a, reg_b):
        self.reg[reg_a] = self.reg[reg_a] + self.reg[reg_b]

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        #elif op == "SUB": etc
        elif op == "CMP":
            '''
            Compare the values in two registers.

            * If they are equal, set the Equal `E` flag to 1, otherwise set it to 0.
            
            * If registerA is less than registerB, set the Less-than `L` flag to 1,
            otherwise set it to 0.

            * If registerA is greater than registerB, set the Greater-than `G` flag
            to 1, otherwise set it to 0.

            `FL` bits: `00000LGE`

            Machine code:
            10100111 00000aaa 00000bbb
            A7 0a 0b
            '''
            
            if self.reg[reg_a] < self.reg[reg_b]:
                self.fl = 0b00000100 # 4 decimal
            elif self.reg[reg_a] > self.reg[reg_b]:
                self.fl = 0b00000010 #2 decimal
            elif self.reg[reg_a] == self.reg[reg_b]:
                self.fl = 0b00000001 #1 decimal
                # print(f"Equal flag: {self.fl}")
        elif op == "AND":
            '''
            Bitwise-AND the values in registerA and registerB, then store the result in 
            registerA.

            10101000 00000aaa 00000bbb
            A8 0a 0b
            '''
            self.reg[reg_a] = self.reg[reg_a] & self.reg[reg_b]
        elif op == "OR":
            '''
            Perform a bitwise-OR between the values in registerA and registerB, storing the
            result in registerA.

            10101010 00000aaa 00000bbb
            AA 0a 0b
            '''
            self.reg[reg_a] = self.reg[reg_a] | self.reg[reg_b]
        elif op == "XOR":
            '''
            Perform a bitwise-XOR between the values in registerA and registerB, storing the
            result in registerA.

            10101011 00000aaa 00000bbb
            AB 0a 0b
            '''
            self.reg[reg_a] = self.reg[reg_a] ^ self.reg[reg_b]
        elif op == "NOT":
            '''
            Perform a bitwise-NOT on the value in a register.

            01101001 00000rrr
            69 0r
            '''
            self.reg[reg_a] = ~self.reg[reg_a]
        elif op == "SHL":
            '''
            Shift the value in registerA left by the number of bits specified in registerB,
            filling the low bits with 0.

            10101100 00000aaa 00000bbb
            AC 0a 0b
            '''
            self.reg[reg_a] = self.reg[reg_a] << self.reg[reg_b]
        elif op == "SHR":
            '''
            Shift the value in registerA right by the number of bits specified in registerB,
            filling the high bits with 0.

            10101101 00000aaa 00000bbb
            AD 0a 0b
            '''
            self.reg[reg_a] = self.reg[reg_a] >> self.reg[reg_b]
        elif op == "MOD":
            '''
            Divide the value in the first register by the value in the second,
            storing the _remainder_ of the result in registerA.

            If the value in the second register is 0, the system should print an
            error message and halt.

            10100100 00000aaa 00000bbb
            A4 0a 0b
            '''
            if self.reg[reg_b] == 0:
                print(f"ZeroDivisionError: integer division or modulo by zero")
                self.hlt()
            else:
                self.reg[reg_a] = self.reg[reg_a] % self.reg[reg_b]
        else:
            raise Exception("Unsupported ALU operation")

    def call(self, operand_a):
        '''
        Calls a subroutine (function) at the address stored in the register.

        Machine code:

        01010000 00000rrr
        50 0r
        '''
        
        # The address of the ***instruction*** _directly after_ `CALL` is pushed onto 
        # the stack. This allows us to return to where we left off when the subroutine 
        # finishes executing.
        
        # decrement the SP
        self.reg[7] -= 1
        # put the value in the specified register in address pointed to by SP
        self.ram[self.reg[7]] = self.pc + 2
        
        # The PC is set to the address stored in the given register. We jump to that 
        # location in RAM and execute the first instruction in the subroutine. The PC 
        # can move forward or backwards from its current location.
        self.pc = self.reg[operand_a]
    
    def jmp(self, register_address):
        '''
        Jump to the address stored in the given register.

        Set the `PC` to the address stored in the given register.

        Machine code:
        01010100 00000rrr
        54 0r
        '''

        self.pc = self.reg[register_address]

    def jeq(self, register_address):
        '''
         If `equal` flag is set (true), jump to the address stored in the given register.

        Machine code:
        01010101 00000rrr
        55 0r
        '''
        if self.fl == 1:
            self.pc = self.reg[register_address]
        else: 
            self.pc += 2
    
    def jne(self, register_address):
        '''
        `JNE register`

        If `E` flag is clear (false, 0), jump to the address stored in the given
        register.

        Machine code:
        01010110 00000rrr
        56 0r
        '''

        if self.fl != 1:
            self.pc = self.reg[register_address]
            # print(f"Jumping to {self.pc}")
        
        else:
            self.pc += 2

    def ret(self):
        '''
        Return from subroutine.

        Machine Code:
        00010001
        11
        '''

        # Pop the value from the top of the stack and store it in the `PC`.

        #Copy the value from the address pointed to by `SP` to the given register.
        self.pc = self.ram[self.reg[7]]
        # Increment `SP`.
        self.reg[7] += 1

    def trace(self):
        """
        Handy function to print out the CPU state. You might want to call this
        from run() if you need help debugging.
        """

        print(f"TRACE: {self.pc+1} | {self.ram_read(self.pc)} {self.ram_read(self.pc + 1)} {self.ram_read(self.pc + 2)} | ", end = '')

        for i in range(8):
            print(f" {self.reg[i]} ", end = '')

        print()

    def hlt(self):
        self.halted = True
    
    def increment_pc(self, value):
        self.pc += value

    def ldi(self, register, value):
        self.reg[register] = value

    def mul(self, reg_a, reg_b):
        self.reg[reg_a] = self.reg[reg_a] * self.reg[reg_b]

    def prn(self, register_num):
        # print(f"Register number: {register_num}")
        print(self.reg[register_num])
    
    def push(self, register):
        '''
        Push the value in the given register on the stack.
        '''
        # decrement the SP
        self.reg[7] -= 1
        # put the value in the specified register in address pointed to by SP
        self.ram[self.reg[7]] = self.reg[register]
        # print(f"Top of the stack is {self.ram[self.reg[7]]} at address {self.reg[7]}")

    def pop(self, register):
        '''
        Pop the value at the top of the stack into the given register.
        '''
        #Copy the value from the address pointed to by `SP` to the given register.
        self.reg[register] = self.ram[self.reg[7]]
        # Increment `SP`.
        self.reg[7] += 1

    def ram_read(self, address):
        # address = self.pc
        return self.ram[address]

    def ram_write(self, value, address):
        self.ram[address] = value

    def run(self):
        """Run the CPU."""
    
        while self.halted == False:
        # for i in range(50):
            self.ir = self.ram_read(self.pc)
            operand_a = self.ram_read(self.pc + 1)
            operand_b = self.ram_read(self.pc + 2)
            
            # self.trace()

            if self.ir == 130:
                self.ldi(operand_a, operand_b)
            
            elif self.ir == 71:
                self.prn(operand_a)

            elif self.ir == 1:
                self.hlt()

            elif self.ir == 162:
                self.mul(operand_a, operand_b)

            elif self.ir == 69:
                self.push(operand_a)

            elif self.ir == 70:
                self.pop(operand_a)

            elif self.ir == 80:
                self.call(operand_a)

            elif self.ir == 17:
                self.ret()
            
            elif self.ir == 160:
                self.add(operand_a, operand_b)

            elif self.ir == 167:
                self.alu("CMP", operand_a, operand_b)
            
            elif self.ir == 168:
                self.alu("AND", operand_a, operand_b)

            elif self.ir == 170:
                self.alu("OR", operand_a, operand_b)
            
            elif self.ir == 171:
                self.alu("XOR", operand_a, operand_b)

            elif self.ir == 105:
                self.alu("NOT", operand_a)

            elif self.ir == 172:
                self.alu("SHL", operand_a, operand_b)

            elif self.ir == 173:
                self.alu("SHR", operand_a, operand_b)

            elif self.ir == 164:
                self.alu("MOD", operand_a, operand_b)

            elif self.ir == 84:
                self.jmp(operand_a)

            elif self.ir == 85:
                self.jeq(operand_a)

            elif self.ir == 86:
                self.jne(operand_a)

            if self.ir !=80 and self.ir != 17 and self.ir != 84 and self.ir != 85 and self.ir != 86:
                self.pc += ((self.ir >> 6)+1) # use bitshift to incremenet PC past operands
                # print(f"PC incremented {((self.ir >> 6)+1)}")