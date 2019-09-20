"""CPU functionality."""

import sys

#List operations
OP1 = 0b10000010 #LDI
OP2 = 0b01000111 #PRN 0b01000111
OP3 = 0b10100010 #MUL
OP4 = 0b01000101 #PUSH
OP5 = 0b01000110 #POP
OP6 = 0b00000001 #HALT
OP7 = 0b01010000 #CALL
OP8 = 0b00010001 #RET
OP9 = 0b10100000 #ADD
OP10 = 0b01010100 #JMP - Jumps PC to an address in given register
OP11 = 0b01010101 #JEQ - If 'E' flag is set (true, 1), jump to given register
OP12 = 0b01010110 #JNE - If 'E' flag is clear (false, 0), jump to given register
OP13 = 0b10100111 #CMP - Compares values and sets flags

#check for file arg
if len(sys.argv) < 2:
    print("usage: ls8.py *insert a program file name as argument*")
    sys.exit(1)


class CPU:
    """Main CPU class."""

    def __init__(self, reg = [0] * 8, ram = [0] * 256, pc = 0):
        """Construct a new CPU."""
        self.reg = reg
        self.ram = ram
        self.pc = pc # memory address starting at 0 when initialized
        self.sp = 244 # top of an empty stack is 244 in ram... grows downward
        self.running = True
        
        # Setup Branch Table
        self.branchtable = {}
        self.branchtable[OP1] = self.ldi
        self.branchtable[OP2] = self.custom_print
        self.branchtable[OP3] = self.multiply
        self.branchtable[OP4] = self.push
        self.branchtable[OP5] = self.pop
        self.branchtable[OP6] = self.hlt
        self.branchtable[OP7] = self.call
        self.branchtable[OP8] = self.ret
        self.branchtable[OP9] = self.add

        self.branchtable[OP10] = self.jmp
        self.branchtable[OP11] = self.jeq
        self.branchtable[OP12] = self.jne
        self.branchtable[OP13] = self.cmp
        #Setup Equal flag and related functions
        self.E = 0  # 0 == false, 1 == true
        self.L = 0 # 1 == regA.value < regB.value
        self.G = 0 # 1 == regA.value > regB.value

    def jmp(self):
        jump_address = self.reg[self.ram[self.pc + 1]]
        self.pc = jump_address

    def cmp(self):
        # compare two values in regA + regB , set flag accordingly
        
        # RESET Flags for subsequent compares ?
        self.E = 0
        self.L = 0
        self.G = 0

        value_one = self.reg[self.ram[self.pc + 1]]
        value_two = self.reg[self.ram[self.pc + 2]]

        if value_one == value_two:
            self.E = 1
        elif value_one < value_two:
            self.L = 1
        elif value_one > value_two:
            self.G = 1

    
    def jne(self):
        if self.E == 0:
            self.pc = self.reg[self.ram[self.pc + 1]]
        else:
            self.pc += 2

    def jeq(self):
        if self.E == 1:
            self.pc = self.reg[self.ram[self.pc + 1]]
        else:
            self.pc += 2

    def add(self):
        self.reg[self.ram[self.pc + 1]] += self.reg[self.ram[self.pc + 2]]
        
    def call(self):

        # custom push functionality
        next_address = self.pc + 2
        self.sp -= 1
        self.ram[self.sp] = next_address
        # set pc
        address = self.reg[self.ram[self.pc + 1]]
        self.pc = address

    def ret(self):
        next_address = self.ram[self.sp]
        self.sp += 1

        self.pc = next_address

    def push(self):
        reg_address = self.ram[self.pc + 1]
        self.sp -= 1
        value = self.reg[reg_address]
        self.ram[self.sp] = value

    def pop(self):
        pop_value = self.ram[self.sp]
        reg_address = self.ram[self.pc + 1]
        self.reg[reg_address] = pop_value
        self.sp += 1

    def ldi(self):
        operand_a = self.ram[self.pc + 1] # targeted register
        operand_b = self.ram[self.pc + 2] # value to load
        self.reg[operand_a] = operand_b
        # self.pc += 3

    def custom_print(self):
        operand_a = self.ram[self.pc + 1]
        print(self.reg[operand_a])

    def hlt(self):
        self.running = False

    def multiply(self):
        operand_a = self.ram[self.pc + 1]
        operand_b = self.ram[self.pc + 2]
        self.alu("MUL", operand_a, operand_b)


    def load(self):
        """Load a program into memory."""
        program = []

        with open(sys.argv[1]) as f:
            for line in f:
                comment_split = line.split('#')
                num = comment_split[0].strip()
                try:
                    program.append(int(num, 2))
                except ValueError:
                    pass

        address = 0

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

        for instruction in program:
            self.ram[address] = instruction
            address += 1


    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            print("ADDING NOW")
            self.reg[reg_a] += self.reg[reg_b]
        elif op == "MUL":
            self.reg[reg_a] *= self.reg[reg_b]
        #elif op == "SUB": etc
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


    def ram_read(self, memory_address_register):
        value = self.ram[memory_address_register]
        return value

    def ram_write(self, memory_data_register, memory_address_register):
        self.ram[memory_address_register] = memory_data_register


    def run(self):
        """Run the CPU."""
        # running = True
        while self.running:

            ir = self.pc
            op = self.ram[ir]
            instruction_size = ((op & 11000000) >> 6) + 1
            pc_set_flag = (op & 0b00010000) # applies a mask to get pc_set bit
            self.branchtable[op]()
            # if op == 0b10000010: # LDI (load into register a value)
            #     operand_a = self.ram[self.pc + 1] # targeted register
            #     operand_b = self.ram[self.pc + 2] # value to load
            #     self.reg[operand_a] = operand_b
            #     # self.pc += 3

            # if op == 0b10100010: # MUL (call alu with paramaters)
            #     operand_a = self.ram[self.pc + 1]
            #     operand_b = self.ram[self.pc + 2]
            #     self.alu("MUL", operand_a, operand_b)
            #     # self.pc += 3

            # elif op == 0b01000111: # PRN (print value from given register address)
            #     operand_a = self.ram[self.pc + 1]
            #     print(self.reg[operand_a])
            #     # self.pc += 2

            # if op == 0b01000101: # PUSH (push value from register onto stack)
            #     reg_address = self.ram[self.pc + 1]
            #     self.sp -= 1
            #     value = self.reg[reg_address]
            #     self.ram[self.sp] = value

            # elif op == 0b01000110: # POP
            #     pop_value = self.ram[self.sp]
            #     reg_address = self.ram[self.pc + 1]
            #     self.reg[reg_address] = pop_value
            #     self.sp += 1

            # elif op == 0b00000001: # HLT (halt cpu)
            #     self.running = False
            
            # else:
            #     pass
            if pc_set_flag != 0b00010000:
                self.pc += instruction_size
                

