"""CPU functionality."""

import sys
SP = 7
class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.reg = [0] * 8
        self.pc = 0
        self.ram = [0] * 256
        self.reg = [0] * 8
        self.reg[7] = 0XF4
        self.sp = self.reg[7]
        self.flag = 0b00000000

    def load(self):
        """Load a program into memory."""

        address = 0
        #Un-hardcode
        
        with open(sys.argv[1]) as f:
            address = 0
            for line in f:
                value = line.split("#")[0].strip()
                if value == '':
                    continue
                v = int(value, 2)
                self.ram[address] = v
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
            if self.reg[reg_a] == self.reg[reg_b]:
                self.flag = 0b00000001
            elif self.reg[reg_a] > self.reg[reg_b]:
                self.flag = 0b10000010
            else:
                self.flag = 0b00000100
        else:
            raise Exception("Unsupported ALU operation")


    def ram_read(self, address):
        return self.ram[address]

    def ram_write(self, value, address):
        self.ram[address] = value

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
        HLT = 0b00000001
        LDI = 0b10000010
        PRN = 0b01000111
        MUL = 0b10100010
        PUSH = 0b01000101
        POP = 0b01000110
        CALL = 0b01010000
        RET = 0b00010001
        ADD = 0b10100000
        CMP = 0b10100111
        JMP = 0b01010100
        JEQ = 0b01010101
        JNE = 0b01010110
        running = True

        while running:
            instruction = self.ram_read(self.pc)
            opr_a = self.ram_read(self.pc + 1)
            opr_b = self.ram_read(self.pc + 2)
            if instruction == HLT:
                running = False
                self.pc +=1

            elif instruction == LDI:
                self.reg[opr_a] = opr_b
                self.pc += 3 

            elif instruction == PRN:
                print(self.reg[opr_a])
                self.pc += 2

            elif instruction == MUL:
                product = self.reg[opr_a] * self.reg[opr_b]
                self.reg[opr_a] = product
                self.pc += 3

            elif instruction == ADD:
                added = self.reg[opr_a] + self.reg[opr_b]
                self.reg[opr_a] = added
                self.pc += 3 

            elif instruction == PUSH:
                # decriment the stack pointer
                self.sp -= 1
                # This will determine which registrar is pushing their value to the stack:
                reg_slot = self.ram[self.pc+1]
                val = self.reg[reg_slot]
                top_of_stack_addr = self.sp
                self.ram[top_of_stack_addr] = val
                self.pc += 2

            elif instruction == POP:
                # This will determine which registrar will get popped from the stack:
                reg_slot = self.ram[self.pc+1]
                # now  the value of the registrar is updated
                # at the reg_slot with its assigned value:
                value = self.ram[self.sp]
                self.reg[reg_slot] = value
                # now will incriment the stack pointer:
                self.sp += 1
                # 2 bit operation
                self.pc += 2
            
            elif instruction == CMP:
                self.alu('CMP', opr_a, opr_b)
                self.pc += 3

            elif instruction == JMP:
                break
                self.pc == self.reg[opr_a]
            
            elif instruction == JEQ:
                if (self.flag & HLT) == 1:
                    self.pc = self.reg[opr_a]
                else:
                    self.pc += 2

            elif instruction == JNE:
                if (self.flag & HLT) == 0:
                    self.pc = self.reg[opr_a]
                else:
                    self.pc += 2

            # If instruction unknown, print location for bug fixing
            else:
                print(f"Unknown instruction {instruction} at address {self.pc}")
                sys.exit(1)