import sys
HLT = 0b00000001
LDI = 0b10000010
PRN = 0b01000111
MUL = 0b10100010
PUSH = 0b01000101
POP = 0b01000110
CALL = 0b01010000  # 00000rrr
RET = 0b00010001
ADD = 0b10100000  # 00000aaa 00000bbb
CMP = 0b10100111
JMP = 0b01010100
JNE = 0b01010110
JEQ = 0b01010101


class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.pc = 0
        self.fl = 0b00000000  # default flag
        self.ram = [0] * 256
        self.reg = [0] * 9

        self.reg[7] = self.ram[0xF4]
        self.sp = self.reg[7]  # sp == stackpointer

        self.branchtable = {}
        self.branchtable[LDI] = self.handle_ldi
        self.branchtable[PRN] = self.handle_prn
        self.branchtable[MUL] = self.handle_mul
        self.branchtable[PUSH] = self.handle_push
        self.branchtable[POP] = self.handle_pop
        self.branchtable[CALL] = self.handle_call
        self.branchtable[RET] = self.handle_ret
        self.branchtable[ADD] = self.handle_add

        # Sprint challenge: add CMP, JMP, JEQ and JNE
        self.branchtable[CMP] = self.handle_cmp
        self.branchtable[JMP] = self.handle_jmp
        self.branchtable[JEQ] = self.handle_jeq
        self.branchtable[JNE] = self.handle_jne

    def handle_jne(self):
        """
        """
        register = self.ram[self.pc + 1]
        if (self.fl & HLT) == 0:
            self.pc = self.reg[register]
        else:
            self.pc += 2

    def handle_jeq(self):
        """

        """
        register = self.ram[self.pc + 1]
        if (self.fl & HLT) > 0:
            self.pc = self.reg[register]
        else:
            self.pc += 2

    def handle_cmp(self):
        """

        """
        register1 = self.ram[self.pc + 1]
        register2 = self.ram[self.pc + 2]
        self.alu("CMP", register1, register2)
        self.pc += 3

    def handle_jmp(self):
        """ 
        """
        register = self.ram[self.pc + 1]
        self.pc = self.reg[register]

    def handle_ldi(self):
        register = self.ram[self.pc + 1]
        value = self.ram[self.pc + 2]

        self.pc += 3
        self.reg[register] = value

    def handle_prn(self):
        register = self.ram[self.pc + 1]
        print(f"======\nPrinting {self.reg[register]} \n====")
        self.pc += 2

    def handle_mul(self):
        register1 = self.ram[self.pc + 1]
        register2 = self.ram[self.pc + 2]
        self.alu("MUL", register1, register2)
        self.pc += 3

    def handle_add(self):
        register1 = self.ram[self.pc + 1]
        register2 = self.ram[self.pc + 2]
        self.alu("ADD", register1, register2)
        self.pc += 3

    def handle_push(self):
        self.sp -= 1
        register = self.ram[self.pc + 1]
        self.ram[self.sp] = self.reg[register]
        print(
            f"Pushing {self.reg[register]} from register {register} onto the Stack")
        self.pc += 2

    def handle_pop(self):
        register = self.ram[self.pc + 1]
        print(
            f"Popping {self.ram[self.sp]} off the stack into register {register}")
        self.reg[register] = self.ram[self.sp]
        self.sp += 1
        self.pc += 2

    def handle_call(self):
        print("CALL")
        # Push next instruction location to stack
        self.sp -= 1
        return_address = self.pc + 2
        self.ram[self.sp] = return_address
        print(f"Saving return address of {return_address} to stack")

        register = self.ram[self.pc + 1]
        print(f"Jumping from {self.pc} to {self.reg[register]}")
        self.pc = self.reg[register]

    def handle_ret(self):
        print("RET")
        self.pc = self.ram[self.sp]
        self.sp += 1

    def ram_read(self, pc):
        print(self.ram[pc])

    def ram_write(self, value):
        self.ram.append(value)

    def load(self):
        """Load a program into memory."""

        address = 0
        filename = sys.argv[1]
        program = []

        with open(filename) as f:
            for line in f:
                n = line.split('#')
                n[0] = n[0].strip()

                if n[0] == '':
                    continue
                val = int(n[0], 2)
                program.append(val)

        for instruction in program:
            self.ram[address] = instruction
            address += 1

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            print(f"REG = {self.reg}")
            # print(f"Adding reg[{reg_a}] which is {self.reg[reg_a]} with reg[{reg_b}] which is {self.reg[reg_b]}")
            self.reg[reg_a] += self.reg[reg_b]

        elif op == "MUL":
            print(f"REG = {self.reg}")
            print(
                f"Multiplying reg[{reg_a}] which is {self.reg[reg_a]} with reg[{reg_b}] which is {self.reg[reg_b]}")
            self.reg[reg_a] = self.reg[reg_a] * self.reg[reg_b]

        elif op == "CMP":

            register_a = self.reg[reg_a]
            register_b = self.reg[reg_b]
            if register_a == register_b:
                self.fl = 0b00000001
            elif register_a > register_b:
                self.fl = 0b00000010
            elif register_a < register_b:
                self.fl = 0b00000100

        else:
            raise Exception("That's wrong, yo")

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
        running = True

        while running:
            # print(f"\nLine {self.pc + 1}: {bin(self.ram[self.pc])}")
            if self.ram[self.pc] == HLT:
                running = False
            else:
                self.branchtable[self.ram[self.pc]]()

        pass
