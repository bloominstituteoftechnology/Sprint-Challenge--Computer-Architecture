import sys

# define instructions
HLT = 0b00000001
LDI = 0b10000010
PRN = 0b01000111
MUL = 0b10100010
ADD = 0b10100000

SUB = 0b10100001
PUSH = 0b01000101
POP = 0b01000110
CALL = 0b01010000
RET = 0b00010001

CMP = 0b10100111
JMP = 0b01010100
JEQ = 0b01010101
JNE = 0b01010110



# SP position
SP = 7


class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.ram = [0] * 256
        self.reg = [0] * 8
        self.running = True
        self.reg[SP] = 0xf4
        self.pc = 0
        self.flag = 0b00000000 # equality flag

        # branchtable
        self.branchtable = {}
        self.branchtable[HLT] = self.hlt
        self.branchtable[LDI] = self.ldi
        self.branchtable[PRN] = self.prn
        self.branchtable[PUSH] = self.push
        self.branchtable[POP] = self.pop
        self.branchtable[CALL] = self.call
        self.branchtable[RET] = self.ret
        self.branchtable[JMP] = self.jmp
        self.branchtable[JEQ] = self.jeq
        self.branchtable[JNE] = self.jne

        # ALU branchtable
        self.alu_branchtable = {}
        self.alu_branchtable[ADD] = self.add
        self.alu_branchtable[SUB] = self.sub
        self.alu_branchtable[MUL] = self.mul
        self.alu_branchtable[CMP] = self.cmp

    def load(self, a_file):
        """load a program into memory."""

        address = 0

        program = []
        with open(a_file, 'r') as f:
            for line in f:
                # slice lines until comment char if any are found
                line = line[:line.find('#')].strip()
                if line not in ('', '\n', '\r\n'):
                    # convert to int then back to binary for ls8 to read
                    program.append(format(int(line, 2), '#010b'))
                    
        for instruction in program:
            self.ram[address] = int(instruction, 2)
            address += 1




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
        """run the CPU."""
        
        while self.running:
            IR = self.ram[self.pc]
       
            num_args = IR >> 6

            op_a = self.ram_read(self.pc+1) 
            op_b = self.ram_read(self.pc+2)


            if ((IR >> 5) & 0b001 == 1): 
                self.alu(IR, op_a, op_b)

            else:
                self.branchtable[IR](op_a, op_b)

            if not ((IR >> 4) & 0b0001 == 1):
                self.pc += 1 + num_args

    def ram_read(self, address):
        """
        Read into the RAM at the given address and return what is stored.
        """
        return self.ram[address]

    def ram_write(self, value, address):
        """
        Write given value into RAM at given address.
        """
        self.ram[address] = value

    def ldi(self, address, val):
        """Load an immediate into a given register."""
        self.reg[address] = val

    def prn(self, address, op_b):
        """Print value at a given register."""
        print(self.reg[address])

    def hlt(self, op_a, op_b):
        """Stop program and exit simulator."""
        sys.exit(0)
        


    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        self.alu_branchtable[op](reg_a, reg_b)

    def add(self, reg_a, reg_b):
        """
        ALU Instruction.
        Adds two registers and stores in reg_a.
        """
        self.reg[reg_a] += self.reg[reg_b]

    def sub(self, reg_a, reg_b):
        """
        ALU Instruction.
        Subtracts two registers and stores in reg_a.
        """
        self.reg[reg_a] -= self.reg[reg_b]

    def mul(self, reg_a, reg_b):
        """
        multiply two registers, store in reg_a.
        """
        self.reg[reg_a] *= self.reg[reg_b]



    def push(self, address, op_b):
        """push value in register onto stack."""
        self.reg[SP] -= 1

        # get value from register
        value = self.reg[address]

        # store it on stack
        top_of_stack_addr = self.reg[SP]
        self.ram[top_of_stack_addr] = value

    def pop(self, address, op_b):
        """pop value at top of stack to given register."""
        # get value from top of stack
        top_of_stack_addr = self.reg[SP]
        value = self.ram[top_of_stack_addr]

        # get reg number and store value
        self.reg[address] = value

        self.reg[SP] += 1



    def call(self, subroutine, op_b):
        """call subroutine at given register."""
        # push return address
        ret_addr = self.pc + 2
        self.reg[SP] -= 1
        self.ram[self.reg[SP]] = ret_addr

        self.pc = self.reg[subroutine] # call subroutine

    def ret(self, op_a, op_b):
        """return from subroutine."""
        # pop return addr off the stack
        ret_addr = self.ram[self.reg[SP]]
        self.reg[SP] += 1

        self.pc = ret_addr # set PC



    def jmp(self, address, op_b):
        """jump to address in given register."""
        self.pc = self.reg[address]

    def cmp(self, reg_a, reg_b):
        """
        compare two registers, set self.fl.
        """
        valueA = self.reg[reg_a]
        valueB = self.reg[reg_b]

        if valueA == valueB:
            
            self.flag = 0b00000001 # set equal flag to 1

        elif valueA < valueB:
            
            self.flag = 0b00000100 # set less-than flag to 1

        elif valueA > valueB:
            
            self.flag = 0b00000010 # set greater-than flag to 1


    def jeq(self, address, op_b):
        """If equal flag is True, jump to address in given register."""
        
        if self.flag & 1 == 1: # if equality flag is true
            self.jmp(address, op_b)
        else: # increment
            self.pc += 2


    def jne(self, address, op_b):
        """If equal flag is clear, jump to address in given register."""
        
        if self.flag & 1 == 0: # check if equality flag is false
            self.jmp(address, op_b)
        else: # increment
            self.pc += 2
