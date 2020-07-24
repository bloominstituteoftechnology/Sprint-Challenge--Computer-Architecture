import sys

# op_HLT = 0b00000001
# op_LDI = 0b10000010
# op_PRN = 0b01000111
# op_MUL = 0b10100010
# op_PUSH = 0b01000101
# op_POP = 0b01000110
# op_CALL = 0b01010000
# op_RET = 0b00010001
# op_ADD = 0b10100000
# op_SUB = 0b10100001
# op_DIV = 0b10100011
# op_CMP = 0b10100111
# op_JMP = 0b01010100
# op_JEQ = 0b01010101


class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.reg = [0] * 8
        self.ram = [0] * 256
        self.pc = 0
        self.running = False
        self.SP = 7
        self.reg[self.SP] = 0xF4
        self.FL = 0b00000000
        self.branchtable = {
            0b00000001: self.op_HLT,
            0b10000010: self.op_LDI,
            0b01000111: self.op_PRN,
            0b10100010: self.op_MUL,
            0b01000101: self.op_PUSH,
            0b01000110: self.op_POP,
            0b01010000: self.op_CALL,
            0b00010001: self.op_RET,
            0b10100000: self.op_ADD,
            0b10100001: self.op_SUB,
            0b10100011: self.op_DIV,
            0b10100111: self.op_CMP,
            0b01010100: self.op_JMP,
            0b01010101: self.op_JEQ,
            0b01010110: self.op_JNE
        }

    def load(self):
        """Load a program into memory."""

        address = 0
        
        if len(sys.argv) < 2:
            print("Please pass in a second filename: python first_filename.py second_filename.py")
            sys.exit()
        try:
            file_name = sys.argv[1]
            with open(file_name) as file:
                for line in file:
                    split_line = line.split('#')[0]
                    command = split_line.strip()

                    if command == '':
                        continue

                    instruction = int(command, 2)
                    self.ram[address] = instruction

                    address += 1

        except FileNotFoundError:
            print(f'{sys.argv[0]}: {sys.argv[1]} file was not found')
            sys.exit()

        
    def ram_read(self, MAR):
        return self.ram[MAR] 

    def ram_write(self, MAR, MDR):
        self.ram[MAR] = MDR

    def alu(self, op, reg_a, reg_b=None):
        """ALU operations."""

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        elif op == "MUL":
            self.reg[reg_a] *= self.reg[reg_b]
        elif op == "SUB":
            self.reg[reg_a] -= self.reg[reg_b]
        elif op == "DIV":
            if reg_b == 0:
                print("Cannot divide by zero.")
                self.branchtable[0b00000001]() # halt
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
        
        self.running = True

        while self.running:
            ir = self.ram[self.pc]
            if ir in self.branchtable:
                function = self.branchtable[ir]
                function()
            else:
                print("Unknown instruction", ir)
                self.running = False
    
    def op_HLT(self):
        '''
        halt the CPU and exit the emulator
        '''
        self.running = False

    def op_LDI(self):
        '''
        sets specified register to a specified value
        '''
        operand_a = self.ram[self.pc+1]
        operand_b = self.ram[self.pc+2]
        self.reg[operand_a] = operand_b
        self.pc += 3

    def op_PRN(self):
        '''
        prints specified value
        '''
        operand_a = self.ram[self.pc+1]
        print(self.reg[operand_a])
        self.pc += 2

    def op_MUL(self):
        '''
        multiplies values using alu
        '''
        operand_a = self.ram[self.pc+1]
        operand_b = self.ram[self.pc+2]
        self.alu('MUL', operand_a, operand_b)
        self.pc += 3
    
    def op_ADD(self):
        '''
        adds values using alu
        '''
        operand_a = self.ram[self.pc+1]
        operand_b = self.ram[self.pc+2]
        self.alu('ADD', operand_a, operand_b)
        self.pc += 3

    def op_PUSH(self):
        '''
        adds to stack
        '''
        # Decrement the SP.
        # Copy the value in the given
        # register to the address pointed to by SP.
        self.reg[self.SP] -= 1
        reg_num = self.ram[self.pc+1]
        value = self.reg[reg_num]
        address = self.reg[self.SP]
        self.ram[address] = value

        self.pc += 2

    def op_POP(self):
        '''
        removes from stack
        '''
        # Copy the value from the address pointed
        # to by SP to the given register.
        # Increment SP.
        reg_num = self.ram[self.pc+1]
        address = self.reg[self.SP]
        value = self.ram[address]
        self.reg[reg_num] = value
        self.reg[self.SP] += 1

        self.pc += 2

    def op_CALL(self):
        '''
        jumps to address
        '''
        # compute return address
        return_addr = self.pc + 2

        # push on the stack
        self.reg[self.SP] -= 1
        self.ram[self.reg[self.SP]] = return_addr

        # set the PC to the value in the given register
        reg_num = self.ram[self.pc+1]
        dest_addr = self.reg[reg_num]
        self.pc = dest_addr

    def op_RET(self):
        '''
        returns back to where it was called from
        '''
        # pop return address from top of stack
        return_addr = self.ram[self.reg[self.SP]]
        self.reg[self.SP] += 1

        # set the pc
        self.pc = return_addr

    def op_SUB(self):
        '''
        adds values using alu
        '''
        operand_a = self.ram[self.pc+1]
        operand_b = self.ram[self.pc+2]
        self.alu('SUB', operand_a, operand_b)
        self.pc += 3

    def op_DIV(self):
        '''
        adds values using alu
        '''
        operand_a = self.ram[self.pc+1]
        operand_b = self.ram[self.pc+2]
        self.alu('DIV', operand_a, operand_b)
        self.pc += 3

    def op_CMP(self):

        # Compares the values in two registers.
        # increment PC by 3
        operand_a = self.ram[self.pc+1]
        operand_b = self.ram[self.pc+2]
        self.alu('CMP', operand_a, operand_b)
        self.pc += 3
    
    def op_JMP(self):
        # Jump to the address in the given reg
        reg_num = self.ram[self.pc+1]
        # Set the PC to the address in the given reg
        self.pc = self.reg[reg_num]

    def op_JEQ(self):
        # If equal flag is true
        # jump to the address in the given reg
        reg_num = self.ram[self.pc+1]
        if self.FL & 0b00000001 == 1:
            self.pc = self.reg[reg_num]
        else:
            self.pc += 2

    def op_JNE(self):
        # If Equals flag is false
        # jump to the address in the given reg
        reg_num = self.ram[self.pc+1]
        if self.FL & 0b00000001 == 0:
            self.pc = self.reg[reg_num]
        else:
            self.pc += 2