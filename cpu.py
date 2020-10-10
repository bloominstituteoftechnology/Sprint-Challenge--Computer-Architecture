import sys 

LDI = 0b10000010
halted = 0b00000001
PRN = 0b01000111
MUL = 0b10100010
NOP = 0b00000000
POP = 0b01000110
RET = 0b00010001
CALL = 0b01010000
PUSH = 0b01000101
SP = 0b00000111
ADD = 0b10100000
SUB = 0b10100001
CMP = 0b10100111
EQ = 0b00000111
JMP = 0b01010100
JEQ = 0b01010101
JNE = 0b01010110
DIV = 0b10100011

class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.ram  = [0] * 256     #Creating the RAM by specs 
        self.reg= [0] * 8         #Creating the registers R0-R7
        self.pc = 0               #Creating the Program Ccounter 
        self.FL = 0b00000000
        self.ie = 0
        self.SP = 7
        self.halted = False

    def ram_read(self, address):
        return self.ram[address]
    def ram_write(self, val, address):
        self.ram[address] = val


    def alu(self, op, reg_a, reg_b):

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        elif op == "MUL":
            self.reg[reg_a] *= self.reg[reg_b]
        elif op == "SUB":
            self.reg[reg_a] -= self.reg[reg_b]
        elif op == "DIV":
            if reg_b == 0:
                print("Cannot divide by zero.")
                self.halted = True
                sys.exit(1)
            else:
                self.reg[reg_a] = self.reg[reg_a] // self.reg[reg_b]
        elif op == "CMP":
            # 00000LGE Flags
            if self.reg[reg_a] == self.reg[reg_b]:
                # set the E flag to 1
                self.FL = 0b00000001
            elif self.reg[reg_a] < self.reg[reg_b]:
                # set the L flag to 1
                self.FL = 0b00000100
            elif self.reg[reg_a] > self.reg[reg_b]:
                # set the G flag to 1
                self.FL = 0b00000010
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
            self.ram_read(self.pc+ 1),
            self.ram_read(self.pc+ 2)
        ), end='')

        for i in range(8):
            print(" %02X" % self.reg[i], end='')

        print()





    def load(self,filename):

        address = 0

        with open(filename) as file:
            for line in file:
                comment_split = line.split("#")
                num = comment_split[0].strip()
                if num == '':  # ignore blanks
                    continue
                val = int(num, 2)
                self.ram_write(val, address)
                address += 1


    def run(self):

        while not self.halted:
            IR = self.ram_read(self.pc)
            operand_a = self.ram_read( self.pc + 1 )
            operand_b = self.ram_read( self.pc + 2 )
            self.exe_instruction(IR, operand_a, operand_b)

    def exe_instruction(self, IR, operand_a, operand_b):

        if IR == halted:
            #print("HALT APPLICATION")
            self.halted = True
            sys.exit(1)
        elif IR == LDI:
            #print("LDI")
            self.reg[operand_a] = operand_b
            self.pc += 3
        elif IR == PRN:
            print(self.reg[operand_a])
            self.pc += 2
        elif IR == MUL:
            self.alu('MUL', operand_a, operand_b)
            self.pc += 3
        elif IR == ADD:
            self.alu('ADD', operand_a, operand_b)
            self.pc += 3
        elif IR == PUSH:
            #add to stack
            self.reg[self.SP] -= 1
            reg_num = self.ram[self.pc + 1]
            value = self.reg[reg_num]
            address = self.reg[self.SP]
            self.ram[address] = value

            self.pc += 2
        elif IR == POP:
            #remove form stack 

            # Increment SP.
            reg_num = self.ram[self.pc + 1]
            address = self.reg[self.SP]
            value = self.ram[address]
            self.reg[reg_num] = value
            self.reg[self.SP] += 1

            self.pc += 2

        elif IR == CALL:
            #jump to address
            return_addr = self.pc + 2

        # push on the stack
            self.reg[self.SP] -= 1
            self.ram[self.reg[self.SP]] = return_addr

        # set the PC to the value in the given register
            reg_num = self.ram[self.pc + 1]
            dest_addr = self.reg[reg_num]
            self.pc = dest_addr
        elif IR == RET:
                  # pop return address from top of stack
            return_addr = self.ram[self.reg[self.SP]]
            self.reg[self.SP] += 1

        # set the pc
            self.pc = return_addr

        elif IR == SUB:
            self.alu('SUB', operand_a, operand_b)
            self.pc += 3
        elif IR == DIV:
            self.alu('DIV', operand_a, operand_b)
            self.pc += 3
        elif IR == CMP:
            self.alu('CMP', operand_a, operand_b)
            self.pc += 3
        elif IR == JMP:
            reg_num = self.ram[self.pc + 1]
            self.pc = self.reg[reg_num]
        elif IR == JEQ:
            reg_num = self.ram[self.pc + 1]
            if self.FL & 0b00000001 == 1:
                self.pc = self.reg[reg_num]
            else:
                self.pc += 2
        elif IR == JNE:
            reg_num = self.ram[self.pc + 1]
            if self.FL & 0b00000001 == 0:
                self.pc = self.reg[reg_num]
            else:
                self.pc += 2