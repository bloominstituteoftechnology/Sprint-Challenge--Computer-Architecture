"""CPU functionality."""

import sys

LDI     = 0b10000010
PRN     = 0b01000111
HLT     = 0b00000001
MUL     = 0b10100010
PUSH    = 0b01000101
POP     = 0b01000110
CALL    = 0b01010000
RET     = 0b00010001
ADD     = 0b10100000

CMP     = 0b10100111
JMP     = 0b01010100
JEQ     = 0b01010101
JNE     = 0b01010110

class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.reg = [0] * 8
        self.ram = [0] * 256
        self.pc = 0
        self.fl = 0
        self.running = False
        self.sp = 7
        
        self.reg[self.sp] = 0xF4

        self.branchtable = {}
        self.branchtable[HLT] = self.handle_HLT
        self.branchtable[LDI] = self.handle_LDI
        self.branchtable[PRN] = self.handle_PRN
        self.branchtable[MUL] = self.handle_MUL
        self.branchtable[PUSH] = self.handle_PUSH
        self.branchtable[POP] = self.handle_POP
        self.branchtable[CALL] = self.handle_CALL
        self.branchtable[RET] = self.handle_RET
        self.branchtable[ADD] = self.handle_ADD
        self.branchtable[CMP] = self.handle_CMP
        self.branchtable[JMP] = self.handle_JMP
        self.branchtable[JEQ] = self.handle_JEQ
        self.branchtable[JNE] = self.handle_JNE

    def ram_read(self, mar):
        return self.ram[mar]
    
    def ram_write(self, mar, mdr):
        self.ram[mar] = mdr

    def load(self):
        """Load a program into memory."""
        if (len(sys.argv)) != 2:
            print("Remember to pass the second file name")
            print("Usage: python3 ls8.py <second_file_name.py>")
            sys.exit()

        try:
            address = 0
            with open(sys.argv[1]) as f:
                for line in f:
                    split = line.split("#")
                    value = split[0].strip()
                    if value == "":
                        continue
                    
                    try:
                        instruction = int(value, 2)
                    except ValueError:
                        print(f'Invalid number {value}')
                        sys.exit(1)
                    
                    self.ram[address] = instruction
                    address += 1
        except FileNotFoundError:
            print(f'Error from {sys.argv[0]}: {sys.argv[1]} not found')
            sys.exit()

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]

        elif op == "MUL":
            self.reg[reg_a] *= self.reg[reg_b]

        elif op == "CMP":
            # from spec -> `FL` bits: `00000LGE`
            if self.reg[reg_a] < self.reg[reg_b]:
                # set the L flag to 1
                self.fl = 0b00000100
            elif self.reg[reg_a] > self.reg[reg_b]:
                # set the G flag to 1
                self.fl = 0b00000010
            elif self.reg[reg_a] == self.reg[reg_b]:
                # set the E flag to 1
                self.fl = 0b00000001

        else:
            raise Exception("Unsupported ALU operation")

    def run(self):
        self.running = True

        while self.running:
            ir = self.ram[self.pc]
            if ir in self.branchtable:
                execute = self.branchtable[ir]
                execute()
            else:
                print("Unknown instruction")
                self.running = False

    def handle_HLT(self):
        self.running = False

    def handle_LDI(self):
        operand_a = self.ram[self.pc + 1]
        operand_b = self.ram[self.pc + 2]
        self.reg[operand_a] = operand_b
        self.pc += 3

    def handle_PRN(self):
        operand_a = self.ram[self.pc + 1]
        print(self.reg[operand_a])
        self.pc += 2

    def handle_MUL(self):
        operand_a = self.ram[self.pc + 1]
        operand_b = self.ram[self.pc + 2]
        self.alu("MUL", operand_a, operand_b)
        self.pc += 3

    def handle_PUSH(self):
        self.reg[self.sp] -= 1
        register_idx = self.ram[self.pc + 1]
        register_value = self.reg[register_idx]
        address = self.reg[self.sp]
        self.ram[address] = register_value
        self.pc += 2

    def handle_POP(self):
        register_idx = self.ram[self.pc + 1]
        address = self.reg[self.sp]
        value = self.ram[address]
        self.reg[register_idx] = value
        self.reg[self.sp] += 1
        self.pc += 2

    def handle_CALL(self):
        address_to_return = self.pc + 2
        self.reg[self.sp] -= 1
        self.ram[self.reg[self.sp]] = address_to_return

        register_idx = self.ram[self.pc + 1]
        destination = self.reg[register_idx]
        self.pc = destination

    def handle_RET(self):
        address_to_return = self.ram[self.reg[self.sp]]
        self.reg[self.sp] += 1
        self.pc = address_to_return

    def handle_ADD(self):
        operand_a = self.ram[self.pc + 1]
        operand_b = self.ram[self.pc + 2]
        self.alu("ADD", operand_a, operand_b)
        self.pc += 3

    def handle_CMP(self):
        operand_a = self.ram[self.pc + 1]
        operand_b = self.ram[self.pc + 2]
        self.alu("CMP", operand_a, operand_b)
        self.pc += 3

    def handle_JMP(self):
        register_idx = self.ram[self.pc + 1]
        self.pc = self.reg[register_idx]

    def handle_JEQ(self):
        register_idx = self.ram[self.pc + 1]
        if self.fl & 0b00000001 == 1:
            self.pc = self.reg[register_idx]
        else:
            self.pc += 2

    def handle_JNE(self):
        register_idx = self.ram[self.pc + 1]
        if self.fl & 0b00000001 == 0:
            self.pc = self.reg[register_idx]
        else:
            self.pc += 2