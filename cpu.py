"""CPU functionality."""

import sys
print('SYSTEM_ARGS:', sys.argv)


class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.ram = [0] * 256
        self.reg = [0] * 8
        self.pc = 0
        self.sp = 7
        self.flag = [0]*8

    def load(self):
        """Load a program into memory."""

        address = 0

        # For now, we've just hardcoded a program:

        # program = [
        #     # From print8.ls8
        #     0b10000010, # LDI R0,8
        #     0b00000000,
        #     0b00001000,
        #     0b01000111, # PRN R0
        #     0b00000000,
        #     0b00000001, # HLT
        # ]

        # for instruction in program:
        #     self.ram[address] = instruction
        #     address += 1

        with open(sys.argv[1]) as f:
            for line in f:
                string_val = line.split("#")[0].strip()
                if string_val == '':
                    continue
                v = int(string_val, 2)
                # print(v)
                self.ram[address] = v
                address += 1

    def ram_read(self, MAR):
        return self.ram[MAR]

    def ram_write(self, MAR, MDR):
        self.ram[MAR] = MDR

    def alu(self, op, opr_a, opr_b):
        """ALU operations."""
        # Flags
        self.flag[5]  # Lesser
        self.flag[6]  # Greater
        self.flag[7]  # Equal

        if op == "ADD":
            self.reg[opr_a] += self.reg[opr_b]
        # elif op == "SUB": etc
        elif op == "MUL":
            self.reg[opr_a] *= self.reg[opr_b]
        elif op == "CMP":
            # if opr_a less than opr_b
            if self.reg[opr_a] < self.reg[opr_b]:
                self.flag[5] = 1
                self.flag[6] = 0
                self.flag[7] = 0
            # if opr_a greater than opr_b
            elif self.reg[opr_a] > self.reg[opr_b]:
                self.flag[5] = 0
                self.flag[6] = 1
                self.flag[7] = 0
            # if both values are equal
            elif self.reg[opr_a] == self.reg[opr_b]:
                self.flag[5] = 0
                self.flag[6] = 0
                self.flag[7] = 1

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

    def run(self):
        """Run the CPU."""

        LDI = 0b10000010
        PRN = 0b01000111
        HLT = 0b00000001
        MUL = 0b10100010
        PUSH = 0b01000101
        POP = 0b01000110
        ADD = 0b10100000
        CALL = 0b01010000
        RET = 0b00010001
        CMP = 0b10100111
        JMP = 0b01010100
        JEQ = 0b01010101
        JNE = 0b01010110

        halted = False

        while not halted:
            instruction = self.ram_read(self.pc)
            opr_a = self.ram_read(self.pc + 1)
            opr_b = self.ram_read(self.pc + 2)

            if instruction == LDI:
                self.reg[opr_a] = opr_b
                self.pc += 3

            elif instruction == PRN:
                print(self.reg[opr_a])
                self.pc += 2

            elif instruction == HLT:
                self.pc += 1
                halted = True

            elif instruction == ADD:
                # self.reg[opr_a] = self.reg[opr_a]+self.reg[opr_b]
                self.alu("ADD", opr_a, opr_b)
                self.pc += 3

            elif instruction == MUL:
                # self.reg[opr_a] = self.reg[opr_a]*self.reg[opr_b]
                self.alu("MUL", opr_a, opr_b)
                self.pc += 3

            elif instruction == PUSH:
                # shorthand
                # self.sp -= 1
                # MDR = self.reg[opr_a]
                # self.ram_write(self.sp, MDR)
                # self.pc += 2

                # a more detailed approach for clarity
                # grab the values we are putting on the reg
                val = self.reg[opr_a]
                # Decrement the SP.
                self.reg[self.sp] -= 1
                # Copy/write value in given register to address pointed to by SP. ram_write(mar, mdr)
                self.ram_write(self.reg[self.sp], val)
                # Increment PC by 2
                self.pc += 2

            elif instruction == POP:
                # shorthand
                # MDR = self.ram_read(self.sp)
                # self.reg[opr_a] = MDR
                # self.sp += 1
                # self.pc += 2

                # a more detailed approach for clarity
                # grab values we are putting on the reg
                val = self.ram[self.reg[self.sp]]
                self.reg[opr_a] = val
                self.reg[self.sp] += 1
                self.pc += 2

            elif instruction == CALL:

                # address of IR pushed into stack
                # pc is set to address stored in reg
                val = self.pc + 2
                reg_index = opr_a
                subroutine_addr = self.reg[reg_index]
                self.reg[self.sp] -= 1
                self.ram[self.reg[self.sp]] = val
                self.pc = subroutine_addr

            elif instruction == RET:

                # return for subroutine/function
                # Pop the value from the top of the stack and store it in pc
                ret_addr = self.reg[self.sp]
                self.pc = self.ram_read(ret_addr)
                self.reg[self.sp] += 1

            elif instructions == CMP:
                # This compares 2 values
                self.alu("CMP", opr_a, opr_b)
                self.pc += 3

            else:
                print(
                    f'unknown instruction {instruction} at address {self.pc}')
                sys.exit(1)
