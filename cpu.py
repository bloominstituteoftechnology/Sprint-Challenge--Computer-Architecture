"""CPU functionality."""

import sys


class CPU:
    """Main CPU class."""

    # by default I want to make RAM 256 bytes this is the default for the ls-8
    # I also wanted to leave the option open for increasing the ram_size to
    # something larger to hold larger programs/stacks
    def __init__(self, DBG=False, ram_size=256):
        """Construct a new CPU."""
        # set to NOP by default
        self.pc = 0

        # global toggle settings
        self.running = False

        # the IR register is just going to hold a copy of the PC reg
        self.ir = 0

        # init the execution interupt registry
        # self.ie = 0

        # stack pointer set to the 7th registry
        self.sp = 7

        # general pourpose registry for holding the arguments for the operations
        self.reg = [0] * 8

        # setting the stack pointer to the defualt location
        self.reg[self.sp] = 244

        # flag register
        self.fl = 0

        # initalize the hash table that will be acting as ram
        self.RAM = {k: None for k in range(ram_size)}

        # gloabal equals flag
        #self.equals = 0b001
        #self.greater = 0b010
        #self.lessthan = 0b100

        # global debugging toggle
        self.DBG = DBG

        self.clock = 0

    def ram_read(self, address):
        return self.RAM[address]

    def ram_write(self, address, value):
        self.RAM[address] = value
        return

    ###########################################################################
    #                        Instructions                                     #
    ###########################################################################
    JMP = 0b01010100
    LDI = 0b10000010
    NOP = 0b00000000
    HLT = 0b00000001
    PRN = 0b01000111
    ST = 0b10000100

    # sc ops
    JEQ = 0b01010101
    JNE = 0b01010110

    # alu operations
    ADD = 0b10100000
    MLP = 0b10100010
    DIV = 0b10100011
    DEC = 0b01100110
    INC = 0b01100101
    CMP = 0b10100111
    AND = 0b10101000
    NOT = 0b01101001
    SUB = 0b10100001
    SHL = 0b10101100
    SHR = 0b10101101
    MUL = 0b10100010
    MOD = 0b10100100
    OR = 0b10101010

    # stack
    PUSH = 0b01000101
    POP = 0b01000110

    # coroutines
    RET = 0b00010001
    CALL = 0b01010000

    ###########################################################################
    #                        Instructions Functions                           #
    ###########################################################################

    # stacks

    def pop(self):
        """take the value from the top of the stack and load it into the
        register that is specified byself.pc+ 1
        """
        stack_value = self.ram_read(self.reg[SP])
        register_number = self.ram_read(self.pc + 1)
        self.reg[register_number] = stack_value
        self.reg[SP] += 1
        self.pc += 2
        return

    def push(self):
        """loads the args from the ram usingself.pc+ 1,2 respectivly
        then write the value from the register to the top of the stack then
        decrement the stack and advance the pc"""
        # get the register from ram
        self.reg[SP] -= 1
        stack_address = self.reg[SP]
        register_number = self.ram_read(self.pc + 1)
        value = self.reg[register_number]
        self.ram_write(stack_address, value)
        self.pc += 2
        return

    # subroutines
    def call(self):
        self.reg[SP] -= 1
        stack_address = self.reg[SP]
        returned_address = self.pc + 2
        self.ram_write(stack_address, returned_address)
        register_number = self.ram_read(self.pc + 1)
        self.pc = self.reg[register_number]
        return

    def ret(self):
        self.pc = self.ram_read(self.reg[SP])
        self.reg[SP] += 1
        return

    # other operators
    def prn(self):
        print(self.reg[self.ram_read(self.pc + 1)])
        self.pc += 2
        return

    def nop(self):
        # still going to want to advance the pc
        self.pc += 1
        return

    def hlt(self):
        exit(1)
        # no need to adnvance theself.pcbecause the progam is finished
        return

    def jmp(self) -> None:
        # load the address to jump to
        reg = self.ram_read(self.pc + 1)
        # set the new location for pc
        self.pc = self.reg[reg]

        return

    def ldi(self):
        self.reg[self.ram_read(self.pc + 1)] = self.ram_read(self.pc + 2)
        self.pc += 3
        return

    def st(self):
        """Takes the value stored in reg_a and writes it to the address that is
        stored in reg_b then increments theself.pcby three.
        """
        reg_a, reg_b = self.ram_read(self.pc + 1), self.ram_read(self.pc + 2)
        self.ram_write(self.reg[reg_b], self.reg[reg_a])
        self.pc += 3
        return

    # alu instructions

    def add(self):
        reg_a = self.ram_read(self.pc + 1)
        reg_b = self.ram_read(self.pc + 2)
        self.alu(self.ir, reg_a, reg_b)
        self.pc += 3
        return

    def sub(self):
        reg_a = self.ram_read(self.pc + 1)
        reg_b = self.ram_read(self.pc + 2)
        self.alu(self.ir, reg_a, reg_b)
        self.pc += 3
        return

    def mul(self):
        reg_a = self.ram_read(self.pc + 1)
        reg_b = self.ram_read(self.pc + 2)
        self.alu(self.ir, reg_a, reg_b)
        self.pc += 3
        return

    def div(self):
        """Divides the value of reg_a by the value of the reg_b then store the
        result in reg_a"""

        reg_a = self.ram_read(self.pc + 1)
        reg_b = self.ram_read(self.pc + 2)
        self.alu(self.ir, reg_a, reg_b)
        self.pc += 3
        return

    def mod(self):
        reg_a = self.ram_read(self.pc + 1)
        reg_b = self.ram_read(self.pc + 2)
        self.reg[reg_a] %= self.reg[reg_b]
        self.pc += 3
        return

    def jeq(self):
        if not self.fl & 0b1:
            self.pc += 2
        elif self.fl & 0B1:
            reg_a = self.ram_read(self.pc + 1)
            self.pc = self.reg[reg_a]
        return

    def jne(self):
        if self.fl & 0b1:
            self.pc += 2
        elif not self.fl & 0b0:
            reg_a = self.ram_read(self.pc + 1)
            self.pc = self.reg[reg_a]
        return

    def shl(self):
        # get both of the registers
        reg_a = self.ram_read(self.pc + 1)
        reg_b = self.ram_read(self.pc + 2)
        # shift the reg_a by the amount stored in reg_b
        self.reg[reg_a] <<= self.reg[reg_b]
        # advance pc
        self.pc += 3
        return

    def shr(self):
        # get both of the registers
        reg_a = self.ram_read(self.pc + 1)
        reg_b = self.read_ram(self.pc + 2)
        # shift the value in the first registry by the value in the second
        # registry and store the result in the reg_a
        self.reg[reg_a] >>= self.reg[reg_b]
        # advance pc
        self.pc += 3
        return

    def cmp(self) -> None:
        """This Function takes the regerister arguments and sets the flag register
        accordingly see the spec for a breakdown on the flags
        """
        reg_a = self.ram_read(self.pc + 1)
        reg_b = self.ram_read(self.pc + 2)
        value_a = self.reg[reg_a]
        value_b = self.reg[reg_b]
        if value_a == value_b:
            self.fl = 0b1
        elif value_a > value_b:
            self.fl = 0b10
        elif value_b > value_a:
            self.fl = 0b100
        self.pc += 3
        return

    ###########################################################################
    #                 CPU DEBUGGING FUNCTIONS                                 #
    ###########################################################################

    def trace(self):
        print(f"""
        pc: {self.pc}
        main loop iter: {self.clock}
        ir: {self.ir}
       self.pc+ 1: {self.RAM[self.pc + 1]}
       self.pc+ 2: {self.RAM[self.pc + 2]}
        registry values:\n{self.reg}\n
        stack(top):\n{self.ram_read(self.reg[self.sp])}

        """)

    def load(self, fn):
        """Loads a .ls8 file from disk and runs it

        Args:
            fn: the name of the file to load into memory
        """
        address = 0
        with open(fn, 'rt') as f:
            for line in f:
                try:
                    line = line.split("#", 1)[0]
                    line = int(line, base=2)
                    self.RAM[address] = line
                    address += 1
                except ValueError:
                    pass

    ###########################################################################
    #                              MAIN                                       #
    ###########################################################################
    def alu(self, operation, reg_a, reg_b):
        """ALU operations."""

        if operation == self.ADD:
            self.reg[reg_a] += self.reg[reg_b]
        elif operation == self.SUB:
            self.reg[reg_a] -= self.reg[reg_b]
        elif operation == self.MUL:
            self.reg[reg_a] *= self.reg[reg_b]
        elif operation == self.DIV:
            self.reg[reg_a] /= self.reg[reg_b]
        else:
            raise Exception("ALU operation not supported")

    def run(self):
        """Starts the main execution of the program"""
        self.running = True

        # a simple count to figure out how many times my cpu has cycled
        self.clock = 0

        self.dispatch = {
            self.ADD: self.add,
            self.SUB: self.sub,
            self.MUL: self.mul,
            self.DIV: self.div,
            self.JEQ: self.jeq,
            self.JNE: self.jne,
            self.CMP: self.cmp,
            self.MOD: self.mod,  # end alu instructions
            self.LDI: self.ldi,
            self.PRN: self.prn,
            self.HLT: self.hlt,
            self.NOP: self.nop,  # end other instructions
            self.PUSH: self.push,
            self.POP: self.pop,  # end stack instructions
            self.CALL: self.call,
            self.RET: self.ret,  # end subroutine instructions
        }
        while self.running:

            self.clock += 1
            if self.DBG:
                print("CLK: {}".format(self.clock))
                breakpoint()

            self.ir = self.ram_read(self.pc)
            self.dispatch[self.ir]()

        return None


if __name__ == '__main__':
    try:
        cpu = CPU(DBG=False)
        cpu.load('sctest.ls8')
        cpu.run()
    except Exception as e:
        pass
