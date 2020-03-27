"""CPU functionality."""

import sys

# #List operations
LDI  = 0b10000010
PRN  = 0b01000111
MUL  = 0b10100010
PUSH = 0b01000101
POP  = 0b01000110
HLT  = 0b00000001
CALL = 0b01010000
RET  = 0b00010001
ADD  = 0b10100000
JMP  = 0b01010100
JEQ  = 0b01010101
JNE  = 0b01010110
CMP  = 0b10100111

class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.registers = [0] * 8  # r0 - r7
        self.running = False
        self.ram = [0] * 256
        self.pc = 0
        self.sp = 7
        self.flag = 0b00000000

    def ram_read(self, MAR):
        """Read the RAM. MAR = memory address register"""
        try:
            return self.ram[MAR]
        except IndexError:
            print("index out of range for RAM read")

    def ram_write(self, MDR, MAR):
        """write to the RAM. MDR = Memory Data Register"""
        try:
            self.ram[MAR] = MDR
        except IndexError:
            print("index out of range for RAM write")

    def increment_pc(self, op_code):
        add_to_pc = (op_code >> 6) + 1
        self.pc += add_to_pc

    def load(self, filename):
        """Load a program into memory."""

        address = 0

        try:
            with open(filename) as f:
                for line in f:
                    comment_split = line.split('#')
                    number = comment_split[0].strip()

                    if number == "":
                        continue  # ignore blank lines

                    val = int(number, 2)

                    self.ram_write(val, address)

                    address += 1

        except FileNotFoundError:
            print(f"{sys.argv[0]}: {filename} not found")
            sys.exit(2)

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            self.registers[reg_a] += self.registers[reg_b]
        # elif op == "SUB": etc
        elif op == "MUL":
            self.registers[reg_a] = self.registers[reg_a] * \
                self.registers[reg_b]
        elif op == "CMP":
            if self.registers[reg_a] == self.registers[reg_b]:
                self.flag = HLT
            elif self.registers[reg_a] > self.registers[reg_b]:
                self.flag = LDI
            else:
                self.flag = 0b00000100
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
            self.ram_read(self.pc + 1),
            self.ram_read(self.pc + 2)
        ), end='')

        for i in range(8):
            print(" %02X" % self.reg[i], end='')

        print()


########### ADDED TO CLEAN UP RUN. RAN OUT OF TIME #########
    # def ldi_op(self, op_code):
    #     address = self.ram_read(self.pc + 1)
    #     data = self.ram_read(self.pc + 2)
    #     self.registers[address] = data
    #     self.increment_pc(op_code)
    
    # def htl_op(self, op_code):
    #     self.running = False
    #     sys.exit(1)

    # def prn_op(self, op_code):
    #     address_a = self.ram_read(self.pc + 1)
    #     print(self.registers[address_a])
    #     self.increment_pc(op_code)

    # def comp_op(self, op_code):
    #     address_a = self.ram_read(self.pc + 1)
    #     address_b = self.ram_read(self.pc + 2)
    #     self.alu('CMP', address_a, address_b)
    #     self.increment_pc(op_code)

    # def add_op(self, op_code):
    #     address_a = self.ram_read(self.pc + 1)
    #     address_b = self.ram_read(self.pc + 2)
    #     self.alu('ADD', address_a, address_b)
    #     self.increment_pc(op_code)

    # def mul_op(self, op_code):
    #     address_a = self.ram_read(self.pc + 1)
    #     address_b = self.ram_read(self.pc + 2)
    #     self.alu('MUL', address_a, address_b)
    #     self.increment_pc(op_code)

    # def jmp_op(self, op_code):
    #     register_address = self.ram_read(self.pc + 1)
    #     self.pc = self.registers[register_address]

    # def jeq_op(self, op_code):
    #     register_address = self.ram_read(self.pc + 1)
    #     # check if equal is true
    #     if self.flag == HLT:
    #         self.pc = self.registers[register_address]
    #     # else increment by op code
    #     else:
    #         self.increment_pc(op_code)

    # def jne_op(self, op_code):
    #     register_address = self.ram_read(self.pc + 1)
    #     if self.flag != HLT:
    #         self.pc = self.registers[register_address]
    #     else:
    #         self.increment_pc(op_code)

    # def push_op(self, op_code):
    #     register_address = self.ram_read(self.pc + 1)
    #     val = self.registers[register_address]
    #     self.registers[self.sp] -= 1  # decrement the stack pointer
    #     self.ram[self.registers[self.sp]] = val
    #     self.increment_pc(op_code)

    # def pop_op(self, op_code):
    #     register_address = self.ram_read(self.pc + 1)
    #     val = self.ram[self.registers[self.sp]]
    #     self.registers[register_address] = val
    #     self.registers[self.sp] += 1
    #     self.increment_pc(op_code)

    # def call_op(self, op_code):
    #     self.registers[self.sp] -= 1
    #     self.ram[self.registers[self.sp]] = self.pc + 2
    #     address_of_subroutine = self.ram[self.pc + 1]
    #     self.pc = self.registers[address_of_subroutine]

    # def ret_op(self, op_code):
    #     self.pc = self.ram[self.registers[self.sp]]
    #     self.registers[self.sp] += 1

    # def operation_switch(self, arg):
    #     operations = {
    #         LDI: self.ldi_op(arg),
    #         PRN: self.prn_op(arg),
    #         MUL: self.mul_op(arg),
    #         PUSH: self.push_op(arg),
    #         POP: self.pop_op(arg),
    #         HLT: self.htl_op(arg),
    #         CALL: self.call_op(arg),
    #         RET: self.ret_op(arg),
    #         ADD: self.add_op(arg),
    #         JMP: self.jmp_op(arg),
    #         JEQ: self.jeq_op(arg),
    #         JNE: self.jne_op(arg),
    #         CMP: self.comp_op(arg),
    #     }

    #     return operations.get(arg, print("error fetching arg"))

    def run(self):
        """Run the CPU."""
        self.running = True

        while self.running:
            op_code = self.ram_read(self.pc)

            # self.operation_switch(op_code)

            # HLT
            if op_code == HLT:
                self.running = False
                sys.exit(1)
            # LDI
            elif op_code == LDI:
                address = self.ram_read(self.pc + 1)
                data = self.ram_read(self.pc + 2)
                self.registers[address] = data
                self.increment_pc(op_code)
            # PRN
            elif op_code == PRN:
                address_a = self.ram_read(self.pc + 1)
                print(self.registers[address_a])
                self.increment_pc(op_code)
                pass
            # CMP
            elif op_code == CMP:
                address_a = self.ram_read(self.pc + 1)
                address_b = self.ram_read(self.pc + 2)
                self.alu('CMP', address_a, address_b)
                self.increment_pc(op_code)
            # ADD
            elif op_code == ADD:
                address_a = self.ram_read(self.pc + 1)
                address_b = self.ram_read(self.pc + 2)
                self.alu('ADD', address_a, address_b)
                self.increment_pc(op_code)
            # MUL
            elif op_code == MUL:
                address_a = self.ram_read(self.pc + 1)
                address_b = self.ram_read(self.pc + 2)
                self.alu('MUL', address_a, address_b)
                self.increment_pc(op_code)
            # JMP
            elif op_code == JMP:
                register_address = self.ram_read(self.pc + 1)
                self.pc = self.registers[register_address]
            # JEQ
            elif op_code == JEQ:
                register_address = self.ram_read(self.pc + 1)
                if self.flag == HLT:
                    self.pc = self.registers[register_address]
                else:
                    self.increment_pc(op_code)
            # JNE
            elif op_code == JNE:
                register_address = self.ram_read(self.pc + 1)
                if self.flag != HLT:
                    self.pc = self.registers[register_address]
                else:
                    self.increment_pc(op_code)
            # PUSH
            elif op_code == PUSH:
                register_address = self.ram_read(self.pc + 1)
                val = self.registers[register_address]
                self.registers[self.sp] -= 1
                self.ram[self.registers[self.sp]] = val
                self.increment_pc(op_code)
            # POP
            elif op_code == POP:
                register_address = self.ram_read(self.pc + 1)
                val = self.ram[self.registers[self.sp]]
                self.registers[register_address] = val
                self.registers[self.sp] += 1
                self.increment_pc(op_code)
            # CALL
            elif op_code == CALL:
                self.registers[self.sp] -= 1
                self.ram[self.registers[self.sp]] = self.pc + 2
                address_of_subroutine = self.ram[self.pc + 1]
                self.pc = self.registers[address_of_subroutine]
            # RET
            elif op_code == RET:
                self.pc = self.ram[self.registers[self.sp]]
                self.registers[self.sp] += 1
            else:
                print('here is the else')
