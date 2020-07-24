"""CPU functionality."""

import sys
from glob import glob


class Stack():
    def __init__(self, num: int) -> None:
        """set the size of the with the num argument"""
        if num <= 256:
            self.stack = [None] * num
        else:
            raise ValueError("STACK.size > 256")
        return None

    def push(self, value):
        self.stack.append(value)
        return

    def pop(self):
        if self.size() > 0:
            return self.stack.pop()
        else:
            return None

    def size(self):
        return len(self.stack)


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
        self.ie = 0

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
        self.equals = 0b001
        self.greater = 0b010
        self.lessthan = 0b100

        # global debugging toggle
        self.DBG = DBG

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

    # gates

    def aand(self):
        """Function takes the value of one register and &'s it then sores the
        result in reg_a
        """
        # get the first register pointer
        reg_a = self.read_ram(self.pc + 1)
        # get the second register pointer
        reg_b = self.read_ram(self.pc + 2)
        # store the value of reg_a & reg_b
        self.reg[reg_a] = self.reg[reg_a] & self.reg[reg_b]
        # increment pc
        self.pc += 3
        return

    def oor(self):
        # get the registry pointers
        reg_a = self.read_ram(self.pc + 1)
        reg_b = self.ram_read(self.pc + 2)
        # store the result of reg_a | reg_b in reg_a
        self.reg[reg_a] = self.reg[reg_a] | self.reg[reg_b]
        # increment pc
        self.pc += 3
        return

    def nnot(self):
        # get the register pointer
        rp = self.read_ram(self.pc + 1)
        # set the register equal to its not'ed self
        self.reg[rp] = ~(self.reg[rp])
        # increment pc
        self.pc += 2
        return

    # stacks

    def pop(self):
        """take the value from the top of the stack and load it into the
        register that is specified by pc + 1
        """
        value = self.ram_read(self.reg[self.sp])
        self.reg[self.ram_read(self.pc + 1)] = value
        self.reg[self.sp] += 1
        self.pc += 2
        return

    def push(self):
        """loads the args from the ram using pc + 1,2 respectivly
        then write the value from the register to the top of the stack then
        decrement the stack and advance the pc"""
        # get the register from ram
        reg_a = self.ram_read(self.pc + 1)
        self.reg[self.sp] -= 1
        self.ram_write(self.reg[self.sp], self.reg[reg_a])

        self.pc += 2
        return

    # subroutines
    def call(self):
        # get the register of the address that we need to jump to
        reg = self.ram_read(self.pc + 1)
        # get the address that we are going to be changing to
        address = self.reg[reg]
        # decrement the stack pointer
        self.reg[self.sp] -= 1
        # push the address of the next instruction to the stack
        self.ram_write(self.reg[self.sp], self.pc + 2)

        self.pc = address
        return

    def ret(self):
        # pull the returna address off of the stack and store it in a var
        address = self.ram_read(self.reg[self.sp])
        # increment the stack pointer
        self.reg[self.sp] += 1
        # set the current pc == to the return address
        self.pc = address
        return

    # other operators
    def prn(self):
        # load the register to print
        reg_a = self.ram_read(self.pc + 1)
        # print the register
        print(self.reg[reg_a])
        # advance the pc
        self.pc += 2
        return

    def nop(self):
        # still going to want to advance the pc
        self.pc += 1
        return

    def hlt(self):
        self.running = False
        # no need to adnvance the pc because the progam is finished
        return

    def jmp(self) -> None:
        # load the address to jump to
        reg = self.ram_read(self.pc + 1)
        # set the new location for pc
        self.pc = self.reg[reg]

        return

    def ldi(self):
        reg_a = self.ram_read(self.pc + 1)
        interger = self.ram_read(self.pc + 2)
        self.reg[reg_a] = interger
        self.pc += 3
        return

    def st(self):
        """Takes the value stored in reg_a and writes it to the address that is
        stored in reg_b then increments the pc by three.
        """
        reg_a, reg_b = self.ram_read(self.pc + 1), self.ram_read(self.pc + 2)
        self.ram_write(self.reg[reg_b], self.reg[reg_a])
        self.pc += 3
        return

    # alu instructions

    def add(self):
        reg_a = self.ram_read(self.pc + 1)
        reg_b = self.ram_read(self.pc + 2)
        self.reg[reg_a] += self.reg[reg_b]

    def sub(self):
        reg_a = self.ram_read(self.pc + 1)
        reg_b = self.ram_read(self.pc + 2)
        self.reg[reg_a] -= self.reg[reg_b]
        self.pc += 3
        return

    def mul(self):
        reg_a = self.ram_read(self.pc + 1)
        reg_b = self.ram_read(self.pc + 2)
        self.reg[reg_a] = self.reg[reg_a] * self.reg[reg_b]
        self.pc += 3
        return

    def div(self):
        """Divides the value of reg_a by the value of the reg_b then store the
        result in reg_a"""

        reg_a = self.ram_read(self.pc + 1)
        reg_b = self.ram_read(self.pc + 2)
        self.reg[reg_a] = self.reg[reg_a] / self.reg[reg_b]
        self.pc += 3
        return

    def mod(self):
        reg_a = self.ram_read(self.pc + 1)
        reg_b = self.ram_read(self.pc + 2)

        self.reg[reg_a] %= self.reg[reg_b]
        self.pc += 3
        return

    def jeq(self):
        # if the CMP flag is set to E
        if self.fl & self.equals:
            # make a jump to the address in reg
            self.jmp()
        return

    def jne(self):
        # if the E flag is clear meaning that the fl == 0 then jump to the
        # address held in the reg
        if not self.fl & self.equals:
            self.jmp()

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
        reg_a, reg_b = self.ram_read(self.pc + 1), self.ram_read(self.pc + 2)
        # adding the compare operation
        self.fl = self.fl & 0x11111000
        if self.reg[reg_a] < self.reg[reg_b]:
            self.fl = self.fl | self.lessthan
        elif self.reg[reg_a] > self.reg[reg_b]:
            self.fl = self.fl | self.greater
        else:
            self.fl = self.fl | self.equals
        self.pc += 3
        return None

    def inc(self):
        # this function increments a register
        rp = self.read_ram(self.pc + 1)
        self.reg[rp] += 1
        self.pc += 2
        return

    def dec(self):
        # this function decrements a register
        rp = self.read_ram(self.pc + 1)
        self.reg[rp] -= 1
        self.pc += 2
        return

    ###########################################################################
    #                 CPU DEBUGGING FUNCTIONS                                 #
    ###########################################################################

    def trace(self):
        """
        Handy function to print out the CPU state. You might want to call this
        from run() if you need help debugging.
        """

        print(self.pc, self.ir, self.fl, self.ie, self.ram_read(self.pc),
              self.ram_read(self.pc + 1), self.ram_read(self.pc + 2))

        for i in range(8):
            print(" %02X" % self.reg[i], end='')

        print()

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

    def run(self):
        """Starts the main execution of the program"""
        self.running = True

        # a simple count to figure out how many times my cpu has cycled
        clock = 0

        self.dispatch = {
            self.SHL: self.shl,
            self.SHR: self.shr,
            self.ADD: self.add,
            self.SUB: self.sub,
            self.MUL: self.mul,
            self.DIV: self.div,
            self.INC: self.inc,
            self.DEC: self.dec,
            self.JEQ: self.jeq,
            self.JNE: self.jne,
            self.CMP: self.cmp,
            self.MOD: self.mod,  # end alu instructions
            self.AND: self.aand,
            self.OR: self.oor,
            self.NOT: self.nnot,  # end gate instructions
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
            clock += 1
            if self.DBG:
                print("CLK: {}".format(clock))
                breakpoint()

            instr = self.ram_read(self.pc)
            self.ir = instr
            self.dispatch[instr]()

        return None


if __name__ == '__main__':
    cpu = CPU()
    tests = glob("./ls8/examples/*.ls8")
    for test in tests:
        try:
            print("test: {}".format(test))
            # test should be the name of the file
            cpu.load(test)
            cpu.run() | print("failed")
        except Exception as e:
            print(e)
