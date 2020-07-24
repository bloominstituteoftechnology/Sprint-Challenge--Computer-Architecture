"""CPU functionality."""

import sys
import os

division_err = f'Please no division by 0.'

# ALU ops
ADD = 0b10100000
SUB = 0b10100001
MUL = 0b10100010
DIV = 0b10100011
MOD = 0b10100100
INC = 0b01100101
DEC = 0b01100110
CMP = 0b10100111
AND = 0b10101000
NOT = 0b01101001
OR = 0b10101010
XOR = 0b10101011
SHL = 0b10101100
SHR = 0b10101101

# PC mutators
CALL = 0b01010000
RET = 0b00010001
INT = 0b01010010
IRET = 0b00010011
JMP = 0b01010100
JEQ = 0b01010101
JNE = 0b01010110
JGT = 0b01010111
JLT = 0b01011000
JLE = 0b01011001
JGE = 0b01011010

# Other
NOP = 0b00000000
HLT = 0b00000001
LDI = 0b10000010
LD = 0b10000011
ST = 0b10000100
PUSH = 0b01000101
POP = 0b01000110
PRN = 0b01000111
PRA = 0b01001000

equal = 0b00000001
greater_than = 0b00000010
less_than = 0b00000100


class CPU:
    def __init__(self):
        self.reg = [0] * 8
        self.ram = [0] * 256
        self.pc = 0
        self.sp = 7
        self.fl = 0  # b0 0 - l - g - e

        self.reg[self.sp] = 0xF4  # setting stack pointer

        # sets alu operations
        self.alu_ops = {}
        self.alu_ops[ADD] = lambda a, b: a + b
        self.alu_ops[SUB] = lambda a, b: a - b
        self.alu_ops[MUL] = lambda a, b: a * b
        self.alu_ops[DIV] = lambda a, b: division_err if b is 0 else a / b
        self.alu_ops[MOD] = lambda a, b: division_err if b is 0 else a % b
        self.alu_ops[INC] = lambda a: a + 1
        self.alu_ops[DEC] = lambda a: a - 1
        self.alu_ops[CMP] = self.handle_cmp
        self.alu_ops[AND] = lambda a, b: a & b
        self.alu_ops[NOT] = lambda a: ~a
        self.alu_ops[OR] = lambda a, b: a | b
        self.alu_ops[XOR] = lambda a, b: a ^ b
        self.alu_ops[SHL] = lambda a, b: a << b
        self.alu_ops[SHR] = lambda a, b: a >> b

        # sets pc operations that mutate pc
        self.pc_mutators = {}
        self.pc_mutators[CALL] = self.handle_call
        self.pc_mutators[RET] = self.handle_ret
        self.pc_mutators[INT] = None
        self.pc_mutators[IRET] = None
        self.pc_mutators[JMP] = lambda a: self.set_pc(self.reg[a])
        self.pc_mutators[JEQ] = self.handle_jeq
        self.pc_mutators[JNE] = self.handle_jne
        self.pc_mutators[JGT] = None
        self.pc_mutators[JLT] = None
        self.pc_mutators[JLE] = None
        self.pc_mutators[JGE] = None

        # sets pc operations
        self.pc_ops = {}
        self.pc_ops[NOP] = None
        self.pc_ops[HLT] = lambda: sys.exit(1)
        self.pc_ops[LDI] = self.handle_ldi
        self.pc_ops[LD] = None
        self.pc_ops[ST] = None
        self.pc_ops[PUSH] = self.handle_push
        self.pc_ops[POP] = self.handle_pop
        self.pc_ops[PRN] = lambda a: print(self.reg[a])
        self.pc_ops[PRA] = None

    def write_program_to_memory(self, program_file):
        address = 0
        with open(os.path.join(sys.path[0], program_file), 'r') as file:
            for line in file:
                line_value = line.split()

                # if able to change into an integer with a binary value
                # add it to ram
                try:
                    binary_instruction = int(line_value[0], 2)

                    # add file to memory list
                    if address < 257:
                        self.ram[address] = binary_instruction
                        address += 1

                # if tried failed, continue to next line
                except:
                    continue
        if not file:
            print('no data in file')
            program = [HTL]

    def load(self):
        """Load a program into memory."""
        args = sys.argv

        if len(args) > 1:
            program_file = args[1]
            try:
                self.write_program_to_memory(program_file)
            except:
                print('Unexpected error: ', sys.exc_info()[0])
        else:
            print(
                'too few arguments, please choose a program to run: "python3 ls8.py examples/program_file.ls8"')

    def ram_read(self, MAR):
        '''
        Returns what is stored at the given address.
        '''
        if MAR < len(self.ram):
            return self.ram[MAR]
        else:
            raise IndexError

    def ram_write(self, MAR, MDR):
        '''
        Writes the given value to the address given.
        '''
        if MAR < len(self.ram):
            self.ram[MAR] = MDR
        else:
            raise IndexError

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

    def run_op(self, fn, a, b, num_ops):
        if num_ops is 2:
            fn(a, b)
        elif num_ops is 1:
            fn(a)
        else:
            fn()

    def op_sorter(self, determiner, op, a, b, num_ops):
        if determiner is 2:  # it is an alu op
            self.alu(op, a, b, num_ops)

        elif determiner is 1:  # it is a pc mutator
            self.run_op(self.pc_mutators[op], a, b, num_ops)

        else:  # it is a different kin of op
            self.run_op(self.pc_ops[op], a, b, num_ops)

    def alu(self, ops, a, b, num_ops):
        try:
            if ops is CMP:
                self.alu_ops[ops](self.reg[a], self.reg[b])
            elif num_ops is 2:
                self.reg[a] = self.alu_ops[ops](self.reg[a], self.reg[b])
            elif num_ops is 1:
                self.reg[a] = self.alu_ops[ops](self.reg[a])
            else:
                self.reg[a] = self.alu_ops[ops]()
        except Exception:
            print("Unsupported ALU operation")

    def run(self):
        """Run the CPU."""
        operands = 0b11000000
        b_or_c = 0b00110000

        while True:
            # self.trace()
            IR = self.ram_read(self.pc)
            num_operands = (IR & operands) >> 6
            instruction_length = 1 + num_operands
            alu_or_pc = (IR & b_or_c) >> 4

            operand_a = self.ram_read(
                self.pc + 1) if self.pc + 1 < 257 else None
            operand_b = self.ram_read(
                self.pc + 2) if self.pc + 2 < 257 else None

            try:
                self.op_sorter(alu_or_pc, IR, operand_a,
                               operand_b, num_operands)
            except Exception:
                print(
                    f'\noperation: {IR} was not recoginized at pc: {self.pc}\nexiting program...\n')
                sys.exit(1)

            # handles where pc is pointing to in respective functions
            if IR is not CALL and IR is not RET and IR is not JMP and IR is not JNE and IR is not JEQ:
                self.pc += instruction_length
            # print("program_counter: ", self.pc)

    def handle_ldi(self, register_index, value):
        '''
        set the value of a register to an integer
        '''
        if register_index < len(self.reg):
            self.reg[register_index] = value
        else:
            raise IndexError

    def handle_push(self, value):
        try:
            self.reg[self.sp] -= 1
            self.ram_write(self.reg[self.sp], self.reg[value])
        except Exception:
            print('Stack is full.')

    def handle_pop(self, value):
        try:
            self.reg[value] = self.ram_read(self.reg[self.sp])
            self.reg[self.sp] += 1
        except Exception:
            print('Stack is empty.')

    def handle_call(self, reg_index):
        self.reg[self.sp] -= 1
        self.ram_write(self.reg[self.sp], self.pc + 2)
        self.set_pc(self.reg[reg_index])

    def handle_ret(self):
        self.set_pc(self.ram_read(self.reg[self.sp]))
        self.reg[self.sp] += 1

    def handle_cmp(self, a, b):
        self.fl = ~self.fl # reset fl before setting
        if a is b:
            self.set_flag(equal)
        elif a > b:
            self.set_flag(greater_than)
        else:
            self.set_flag(less_than)

    def set_flag(self, flag):
        self.fl = flag

    def set_pc(self, address):
        self.pc = address

    def handle_jeq(self, reg_index):
        if self.fl is equal:
            self.set_pc(self.reg[reg_index])
        else:
            self.pc += 2

    def handle_jne(self, reg_index):
        if self.fl is not equal:
            self.set_pc(self.reg[reg_index])
        else:
            self.pc += 2


if __name__ == '__main__':
    cpu = CPU()
    cpu.load()
    cpu.run()